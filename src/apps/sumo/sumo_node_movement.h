/**
 * @file sumo_node_movement.h
 * Defines the class sumo::SumoNodeMovementCreator.
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 546 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_node_movement.h $
 * $Id: sumo_node_movement.h 546 2011-04-15 10:40:27Z simsiem $
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

#ifndef __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H
#define __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

#include "sumo_net.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/processors/processor_factory.h"
#include "sys/vec.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>

namespace shawn
{
class SimulationController;
}

namespace sumo
{

/**
 * Moves nodes based on a SUMO dump file.
 *
 * This class reads the SUMO dump file block by block. One block contains all
 * movements within one time step. The movements are compared with those of the
 * previous time step to find new and deleted nodes. Such nodes are created
 * or deleted from the simulation. The block by block reading schema has the
 * advantage that only a small part of the SUMO dump file is kept in memory.
 * As a consequence, even large SUMO dump files (several gigabytes) can easily
 * be processed.
 */
class SumoNodeMovementCreator : public shawn::NodeMovementCreator
{
public:

   /**
    * Gets the configuration, opens the dump file and creates the SumoNet.
    *
    * @param sc the SimulationController to access the simulation environment
    */
   SumoNodeMovementCreator( shawn::SimulationController& sc );

   /**
    * Cleans up.
    */
   virtual
   ~SumoNodeMovementCreator();

   /**
    * Returns the next node movement to be executed
    * (inherited from shawn::NodeMovementCreator).
    *
    * shawn::NodeMovementCreator calls this method whenever it needs a new
    * movement. It does so as long as next_movment does not return a NULL
    * pointer or a movement must be executed at a later point in time.
    * Therefore, next_movement returns movements for every node in each round.
    * At the end of each round, a shawn::MovementInfo with a dispatch time of
    * the next round is returned. It references no node (NULL). This way, the
    * movement controller waits until the next round to go on asking for new
    * movements.
    *
    * The function also calls private functions to delete and add nodes.
    *
    * @return the next shawn::MovementInfo to be executed
    */
   virtual shawn::MovementInfo *
   next_movement();

   /**
    * Resets the class to a state that the next call of next_movement provides
    * the same shawn::MovementInfo like the first call.
    * It cleans up the data structures and reopens the dump file.
    */
   virtual void
   reset();

private:

   /**
    * SUMO net that is associated with this dump file. It is used
    * for coordinate transformations.
    */
   SumoNet net_;

   /**
    * Path to the SUMO dump file.
    * Must be saved for a reset and for debugging messages.
    */
   std::string dump_file_name_;

   /**
    * Input stream for SUMO dump file.
    */
   std::ifstream ifs_dump_file_;

   /**
    * The simulation controller.
    * Necessary to access the world to add and delete nodes and to request
    * the current time.
    */
   shawn::SimulationController& sc_;

   /**
    * Last round that has been parsed.
    */
   int last_parsed_round_;

   /**
    * The edge, lane and position within the lane that the parse have been read
    * recently.
    */
   SumoNet::SumoPosition last_vehicle_position_;

   /**
    * Factories of all processors that should be created for a new node.
    * The processors are specified in the configuration file.
    */
   std::vector<shawn::ProcessorFactoryHandle> processor_factories_;

   /**
    * Table with the current position of every node.
    */
   std::map<std::string, shawn::Vec> current_positions_;

   /**
    * Table with the position of every node in the last round.
    * Together with current_positions_, the movement can be computed.
    * In addition, this list helps to detect whether a node has disappeared or
    * has come up.
    */
   std::map<std::string, shawn::Vec> last_positions_;

   /**
    * All movements of this round that have not been executed yet.
    *
    * If next_movement is executed for the first time in a Shawn round, this
    * vector is filled with all movements that have to be executed in this round.
    * All further calls of next_movement then just pick the next remaining
    * movement until all movements are processed.
    */
   std::vector<shawn::MovementInfo*> remaining_movements_;

   /**
    * The Expat parser used to process the SUMO dump file.
    */
   XML_Parser xml_parser_;

   /**
    * The last return value of an Expat parsing call.
    */
   XML_Status parse_status_;

   /**
    * Performs all actions necessary at the beginning of each round.
    * It parses one round of the dump file, adds new nodes, deletes disappeared
    * nodes, and computes the movements.
    */
   void
   next_round();

   /**
    * Read data needed for current round from dumpfile.
    * Store this data to the map of current positions
    * Called by next_round()
    */
   void
   round_data_from_dumpfile();

   /**
    * Computes the movements for the current round.
    * It stores them in the list remaining_movements_.
    */
   void
   generate_movements();

   /**
    * Adds new nodes to the world.
    * If a node appears in current_positions_ and is not in last_positions_,
    * it is added.
    */
   void
   add_new_nodes();

   /**
    * Removes nodes from the world.
    * If a node is in last_positions_ and not in current_positions_,
    * it is deleted.
    */
   void
   delete_nodes();

   /**
    * Extracts the node label from the trip/route id of the dump file. More
    * specifically, it returns the substring until the first point in route_id
    * or until the end of route_id.
    *
    * The SUMO definition files cannot encode that one vehicle travels
    * several trips. Every trip and route respectively has its own id. This
    * app supports the naming convention vehicleId.sequenceNumber for the route
    * id. This way, a vehicle that goes several routes can be identified by the
    * vehicleId, while the route id in total is still unique.
    *
    * This app saves the vehicle id in the node label, if there is a point in
    * the route id. Otherwise the whole route id makes up the vehicle id. This
    * method returns the substring from the route id that makes up the label.
    *
    * @param route_id the string from which the label should be extracted
    * @return the node label
    */
   static std::string
   route_id_to_label( const std::string& route_id );

   /**
    * Opens the dump file, configures the stream and sets up the Expat.
    * This method contains the common code of the constructor and
    * the reset method.
    */
   void
   init_dump_file();

   /**
    * Reads data from the dump file and passes it to the Expat until the Expat
    * is suspended.
    *
    * @throws std::runtime_error All errors of the Expat are converted to
    * std::runtime_error exceptions
    *
    * This method throws all exceptions from the handler methods, since they are
    * not caught. The input stream (std::fstream) is configured to throw an
    * exception in case of an error. The exception is also handed over.
    */
   void
   parse_dump_file();

   /**
    * Evaluates XML start elements when parsing a SUMO dump file.
    *
    * This method is an Expat handler function. It is called by the Expat
    * whenever the parser has processed an XML start element.
    *
    * @param userdata the processing SumoNodeMovementCreator object
    * @param name the name of the XML tag that is opened
    * @param atts the list of name-value pairs of all attributes
    * in the start tag
    *
    * User data is the SumoNodeMovementCreator object (this) here. It makes
    * the handler functions stateful. atts is the list of attributes in form
    * of an array of C strings. It is a sequence of attribute names and
    * attribute values. The list is 0 terminated (as well as every string).
    *
    * @throws std::runtime_error All errors of the Expat are converted to
    * std::runtime_error exceptions.
    *
    * This method throws all exceptions of find_attribute, since they are not
    * caught.
    */
   static void XMLCALL
   dump_reader_start( void *userdata, const XML_Char *name,
            const XML_Char **atts );

   /**
    * Emits a runtime_error exception reporting an error from the Expat.
    *
    * This method is called from various locations of the parsing code. It
    * makes the error handling easier. It contains the name of the dump file,
    * the Expat error code and the message provided as an argument.
    *
    * @param msg a text that is integrated in the exception message
    * @throws std::runtime_error the exception as configured by the arguments
    */
   void
   emit_parsing_error( const std::string& msg );

   /**
    * Extracts the value of an attribute with the given name.
    * If the attribute name is not in atts, this method throws an exception.
    *
    * @param atts list of name-value pairs from the Expat
    * @param name name of the attribute for which the value should be found
    * @return the value of the attribute
    * @throws std::out_of_range name is not in atts
    */
   std::string
   find_attribute( const XML_Char **atts, const std::string& name );
};

}

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H */
