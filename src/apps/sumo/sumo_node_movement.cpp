/**
 * @file sumo_node_movement.cpp
 * Implements the sumo::SumoNodeMovementCreator.
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 517 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_node_movement.cpp $
 * $Id: sumo_node_movement.cpp 517 2011-03-23 15:02:59Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010 Josef Schlittenlacher<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "sumo_node_movement.h"
#ifdef ENABLE_SUMO

#include "sys/xml/expat/expat.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/tokenize.h"
#include "sys/transmission_model.h"
#include <stdexcept>
#include <cstring>

using namespace std;
using namespace shawn;

namespace sumo
{

SumoNodeMovementCreator::SumoNodeMovementCreator( SimulationController& sc ) :
   sc_(sc), last_parsed_round_(-2), net_(
            sc.environment_w().required_string_param("net_file")),
            parse_status_(XML_STATUS_OK)
{
   // Open the dump file for input
   dump_file_name_ = sc_.environment_w().required_string_param("dump_file");
   init_dump_file();

   // Generate the set of processor factories as configured by the user
   StrTok processors_tok_(sc_.environment_w().optional_string_param(
            "processors", ""), ",");
   for ( StrTok::iterator iter = processors_tok_.begin(); iter
            != processors_tok_.end(); ++iter )
   {
      ProcessorFactoryHandle pfh = sc_.processor_keeper_w().find_w(*iter);
      assert( pfh != NULL );
      processor_factories_.push_back(pfh);
   }
}

// -----------------------------------------------------------------------------

SumoNodeMovementCreator::~SumoNodeMovementCreator()
{
   if ( xml_parser_ )
   XML_ParserFree(xml_parser_);
}

// -----------------------------------------------------------------------------

MovementInfo *
SumoNodeMovementCreator::next_movement()
{
   double current_time = sc_.world().current_time();
   double parser_time = static_cast<double> (last_parsed_round_);

   if ( current_time >= parser_time )
   {
      next_round();
   }

   if ( remaining_movements_.size() == 0 )
   {
      MovementInfo* mi = new MovementInfo();
      mi->set_urgency(MovementInfo::Delayed);
      mi->set_dispatch_time(parser_time);
      return mi;
   }
   else
   {
      MovementInfo* mi = remaining_movements_.back();
      remaining_movements_.pop_back();
      return mi;
   }
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::reset()
{
   if ( ifs_dump_file_.is_open() )
   {
      ifs_dump_file_.close();
      if ( ifs_dump_file_.failbit )
      {
         throw(runtime_error("Could not close the SUMO dump file "
                  + dump_file_name_
                  + " while reseting the SumoNodeMovementCreator"));
      }
   }
   XML_ParserFree(xml_parser_);
   init_dump_file();

   current_positions_.clear();
   last_positions_.clear();
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::init_dump_file()
{
   ifs_dump_file_.open(dump_file_name_.c_str());
   ifs_dump_file_.exceptions(ifstream::badbit);

   // Initialise the parser
   xml_parser_ = XML_ParserCreate(NULL);
   if ( !xml_parser_ )
   throw(runtime_error("Error while parsing the SUMO dump file: "
                     "Could not create the parser"));

   XML_SetUserData(xml_parser_, static_cast<void*> (this));
   XML_SetStartElementHandler(xml_parser_,
            SumoNodeMovementCreator::dump_reader_start);

   // Go to the first stop point (an opening timestep tag)
   parse_status_ = XML_STATUS_OK;
   parse_dump_file();
   next_round();
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::next_round()
{
   round_data_from_dumpfile();
   add_new_nodes();
   delete_nodes();
   generate_movements();
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::generate_movements()
{
   for ( map<string, Vec>::iterator iter = current_positions_.begin(); iter
            != current_positions_.end(); iter++ )
   {
      map<string, Vec>::iterator iter_last = last_positions_.find(iter->first);

      if ( iter_last != last_positions_.end() ) // existing node -> linear movement
      {
         MovementInfo * movement_info = new MovementInfo();
         string node_label = route_id_to_label(iter->first);
         Node * n = sc_.world_w().find_node_by_label_w(node_label);
         if ( !n )
         {
            throw(runtime_error(
                     "Error while generating a node movement: Could not find node "
                              + node_label));
         }
         movement_info->set_node(n);
         movement_info->set_urgency(MovementInfo::Delayed);
         movement_info->set_dispatch_time(
                  static_cast<double> (last_parsed_round_ - 1));

         LinearMovement * linear_movement = new LinearMovement();
         Vec speed = iter->second - iter_last->second;
         linear_movement->set_parameters(speed.euclidean_norm(), iter->second,
                  sc_.world_w());
         movement_info->set_node_movement(linear_movement);
         remaining_movements_.push_back(movement_info);
      }

   }
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::add_new_nodes()
{
   for ( map<string, Vec>::iterator iter = current_positions_.begin(); iter
            != current_positions_.end(); iter++ )
   {
      if ( last_positions_.find(iter->first) == last_positions_.end() )
      {
         Node * n = new Node;
         n->set_label(route_id_to_label(iter->first)); // label of nodes is trip id
         n->set_real_position(iter->second);

         for ( vector<ProcessorFactoryHandle>::const_iterator iter2 =
                  processor_factories_.begin(); iter2
                  != processor_factories_.end(); iter2++ )
         {
            ProcessorHandle ph = iter2->get()->create();
            n->add_processor(ph);
         }

         sc_.world_w().add_node(*n);
      }

   }
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::delete_nodes()
{
   for ( map<string, Vec>::iterator iter = last_positions_.begin(); iter
            != last_positions_.end(); iter++ )
   {
      if ( current_positions_.find(iter->first) == current_positions_.end() )
      {
         string node_label = route_id_to_label(iter->first);
         sc_.world_w().transmission_model_w().deliver_messages();
         Node * n = sc_.world_w().find_node_by_label_w(node_label);
         if ( n )
         {
            sc_.world_w().edge_model_w().node_removed(*n);
            sc_.world_w().remove_node(*n);
         }
         else
         {
            throw(runtime_error("Could not delete the node with the label "
                     + node_label + ": Node not found"));
         }
      }
   }
}

// -----------------------------------------------------------------------------

string
SumoNodeMovementCreator::route_id_to_label( const string& route_id )
{
   string::size_type pointPos = route_id.find('.');
   return route_id.substr(0, pointPos);
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::round_data_from_dumpfile()
{
   // Throw away data of second last round,
   // save positions of last round, and
   // create new container for new positions.
   last_positions_.clear();
   last_positions_.swap(current_positions_);
   int tmp_round = last_parsed_round_ + 1;

   while ( last_parsed_round_ < tmp_round )
   {
      parse_status_ = XML_ResumeParser(xml_parser_);
      // parse_dump_file evaluates the parse_status,
      // even of the previous resume call.
      parse_dump_file();
   }
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::parse_dump_file()
{
   char* dump_buffer;
   const int dump_buffer_size = 65536;

   while ( parse_status_ == XML_STATUS_OK )
   {
      dump_buffer = static_cast<char*> (XML_GetBuffer(xml_parser_,
               dump_buffer_size));
      if ( !dump_buffer )
      {
         ostringstream oss;
         oss << "Error while parsing the SUMO dump file: "
                  << "Could not allocate memory (Expat Code: "
                  << XML_GetErrorCode(xml_parser_) << ")";
         throw(runtime_error(oss.str()));
      }

      // An exception is thrown if badbit is set (see init_dump_file).
      ifs_dump_file_.read(dump_buffer, dump_buffer_size);

      parse_status_ = XML_ParseBuffer(xml_parser_, ifs_dump_file_.gcount(),
               ifs_dump_file_.eof());
   }

   if ( parse_status_ == XML_STATUS_ERROR )
   {
      ostringstream oss;
      oss << "Error while parsing the SUMO dump file: "
               << "Parser error (Expat code: " << XML_GetErrorCode(xml_parser_)
               << ")";
      throw(runtime_error(oss.str()));
   }
   // else: XML_STATUS_SUSPENDED
}

// -----------------------------------------------------------------------------

void XMLCALL
SumoNodeMovementCreator::dump_reader_start( void *userdata,
         const XML_Char *name, const XML_Char **atts )
{
   SumoNodeMovementCreator& self =
            *static_cast<SumoNodeMovementCreator*> (userdata);
   XML_Status parse_status;

   if ( strcmp(name, "timestep") == 0 )
   {
      parse_status = XML_StopParser(self.xml_parser_, 1);
      if ( parse_status == XML_STATUS_ERROR )
      {
         self.emit_parsing_error("Could not stop the parser");
      }
      istringstream timeStream(self.find_attribute(atts, "time"));
      timeStream >> self.last_parsed_round_;
      --self.last_parsed_round_;
   }
   else if ( strcmp(name, "vehicle") == 0 )
   {
      string vehicle_id = self.find_attribute(atts, "id");
      istringstream posStream(self.find_attribute(atts, "pos"));
      posStream >> self.last_vehicle_position_.lane_position;
      Vec position_vector = self.net_.sumo_pos_to_vec(
               self.last_vehicle_position_);
      self.current_positions_.insert(make_pair(vehicle_id, position_vector));
   }
   else if ( strcmp(name, "edge") == 0 )
   {
      self.last_vehicle_position_.edge_id = self.find_attribute(atts, "id");
   }
   else if ( strcmp(name, "lane") == 0 )
   {
      self.last_vehicle_position_.lane_id = self.find_attribute(atts, "id");
   }
}

// -----------------------------------------------------------------------------

void
SumoNodeMovementCreator::emit_parsing_error( const string& msg )
{
   enum XML_Error error_code = XML_GetErrorCode(xml_parser_);
   XML_Size line_number = XML_GetCurrentColumnNumber(xml_parser_);
   ostringstream error_msg;
   error_msg << "Error while parsing the SUMO dump file " << dump_file_name_
            << ": " << msg << " (Expat code: " << error_code
            << ", line number: " << line_number << ")";
   throw(runtime_error(error_msg.str()));

}

// -----------------------------------------------------------------------------

string
SumoNodeMovementCreator::find_attribute( const XML_Char **atts,
         const string& name )
{
   for ( const XML_Char** i = atts; *i != 0; i += 2 )
   {
      if ( name == *i )
      {
         return *(i + 1);
      }
   }

   {
      // Else: The attribute has not been found. The return value cannot
      // indicate that, so I throw an exception.
      XML_Size line_number = XML_GetCurrentLineNumber(xml_parser_);
      ostringstream error_msg;
      error_msg << "Error while parsing the SUMO dump file " << dump_file_name_
               << ": " + name
                        + " not found in the XML attribute list (line number: "
               << line_number << ")";
      throw(out_of_range(error_msg.str()));
   }
}

}

#endif /* ENABLE_SUMO */
