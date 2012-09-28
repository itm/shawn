/**
 * @file sumo_net.cpp
 * Implements the class sumo::SumoNet
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 623 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_net.cpp $
 * $Id: sumo_net.cpp 623 2011-08-19 12:29:20Z simsiem $
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

#include "sumo_net.h"
#ifdef ENABLE_SUMO

#include <boost/algorithm/string/split.hpp>
#include <list>
#include <sstream>

using namespace std;
using namespace shawn;

namespace sumo
{

map<string, vector<Vec> >*
SumoNet::SUMONetReader::extract_lanes(
         map<string, vector<Vec> > * lane_list_ptr ) throw ( runtime_error )
{
   lanes_ptr_ = lane_list_ptr;
   SAXSimpleSkipReader::set_skip_target("lane");
   SAXSimpleSkipReader::parse();
   return lanes_ptr_;
}

// -----------------------------------------------------------------------------

void
SumoNet::SUMONetReader::skip_target_reached( string, xml::AttList& atts )
         throw ( runtime_error )
{
   // Verify that the id and the shape attribute is available
   string lane_id = xml::attribute("id", atts, "");
   if ( lane_id.size() == 0 )
   {
      throw(runtime_error(
               "Error while reading the net file: The lane id is missing."));
   }

   string shape_string = xml::attribute("shape", atts, "");
   if ( shape_string.size() == 0 )
   {
      ostringstream ss;
      ss << "Error while reading the net file: The shape of lane " << lane_id
               << " is missing.";
      throw(runtime_error(ss.str()));
   }

   // Parse the shape. It is given as a list of coordinates
   // in the form "x,y x,y x,y x,y". Firstly, split shape_string
   // at the spaces and store the list in positions. Secondly, split
   // the positions at the commas and store the list in coordinates_as_strings.
   list<string> positions;
   boost::algorithm::split(positions, shape_string, bind2nd(equal_to<char> (),
            ' '));

   vector<string> coordinates_as_strings;
   // In this vector, all positions are stored as floating point tuples.
   // The length of this vector is already known.
   vector<Vec> lane_shape = vector<Vec> (positions.size());
   vector<Vec>::iterator out_it = lane_shape.begin();
   for ( list<string>::iterator in_it = positions.begin(); in_it
            != positions.end(); ++in_it, ++out_it )
   {
      boost::algorithm::split(coordinates_as_strings, *in_it, bind2nd(equal_to<
               char> (), ','));
      if ( coordinates_as_strings.size() != 2 )
      {
         ostringstream ss;
         ss
                  << "Error while reading the net file: Wrong value in the shape of lane "
                  << lane_id << ".";
         throw(runtime_error(ss.str()));
      }
      *out_it = Vec(atof(coordinates_as_strings[0].c_str()), atof(
               coordinates_as_strings[1].c_str()), 0.0);
   }

   // Parsing has been successful. Copy the results into the map.
   lanes_ptr_->insert(make_pair(lane_id, lane_shape));

   // The skip reader clears itself after a tag has been found.
   // So it must be re-set to "lane" again.
   SAXSimpleSkipReader::set_skip_target("lane");
}

// -----------------------------------------------------------------------------

SumoNet::SumoNet( const string& path_net_file )
{
   SUMONetReader(path_net_file).extract_lanes(&map_lanes_);
}

// -----------------------------------------------------------------------------

Vec
SumoNet::sumo_pos_to_vec( const SumoPosition& sumo_position ) const
{
   double seen_length = 0.0;

   const std::vector<Vec> & veclane_shape = map_lanes_.find(
            sumo_position.lane_id)->second;
   std::vector<Vec>::const_iterator iter = veclane_shape.begin();
   std::vector<Vec>::const_iterator iter_last = veclane_shape.begin();
   iter++;
   for ( ; iter < veclane_shape.end(); iter++ )
   {
      double distance = (*iter - *iter_last).euclidean_norm();
      if ( seen_length + distance < sumo_position.lane_position )
      {
         seen_length += distance;
      }
      else
      {
         return Vec(iter_last->x()
                  + (sumo_position.lane_position - seen_length) / distance
                           * (iter->x() - iter_last->x()), iter_last->y()
                  + (sumo_position.lane_position - seen_length) / distance
                           * (iter->y() - iter_last->y()), 0.0);
      }
      iter_last = iter;
   }

   // if lane position is nearly or exactly lane length, rounding errors may prevent the loop to finish -> return end of lane
   if ( veclane_shape.size() > 0 )
   {
      return Vec(veclane_shape.back().x(), veclane_shape.back().y(), 0.0);
   }

   return Vec(0.0, 0.0, 0.0);
}

}

#endif /* ENABLE_SUMO */
