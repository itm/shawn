/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_NS2_NODE_MOVEMENT_CREATOR
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_NS2_NODE_MOVEMENT_CREATOR

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include <fstream>
#include <string>
#include "sys/simulation/simulation_environment.h"


namespace shawn
{
   /// This class provides an implementation of the NodeMovementCreator
   /** This NodeMovementCreator reads input from a file containing movement orders adhering to
     *  the NS2 standard. The following syntax can be parsed: [$ns_ at var_time] ($node_(var_nodeid)|var_nodelabel)
     *  (set (X_|Y_|Z_) var_position|setdest var_x var_y var_z)
     */
   class NS2NodeMovementCreator:
      public NodeMovementCreator
   {

//    friend double shawn::SimulationEnvironment::string2double( const std::string &, const std::string &)
//       const throw ( std::runtime_error );

   public:

      ///@name construction / destruction
      ///@{
      ///
      NS2NodeMovementCreator(SimulationController& sc);
      ///
      virtual ~NS2NodeMovementCreator();
      ///@}


      ///@name methods inherited from NodeMovementCreator
      ///@{
      /** This method reads a line from a file designated in shawnconf.h.
       *  The movement order from that line is returned as a MovementInfo object.
       */
      virtual MovementInfo * next_movement();

      /** This method causes the NodeMovementCreator to start parsing
       *  from the beginning of the inputfile.
       *
       */
      virtual void reset();

   private:

      virtual bool parse_line(std::string);

      virtual bool parse_instant(std::string);

      virtual bool parse_scheduled();

      virtual bool parse_rest(int, std::string, double);

      SimulationController * sc_;
      //NodeMovement * node_movement_;
      Node * node_;
      MovementInfo * movement_info_;
      std::ifstream * inputfile_;
      std::istream * sstream_;
      std::stringstream * istream_;
      std::stringbuf * sbuf_;
      int line_nr_;
      MovementInfo::Urgency urgency_;
    };

 }

 #endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/ns2_node_movement_creator.h,v $
 * Version $Revision: 248 $
 * Date    $Date: 2008-09-23 16:28:10 +0200 (Tue, 23 Sep 2008) $
 *-----------------------------------------------------------------------
 * $Log: ns2_node_movement_creator.h,v $
 *-----------------------------------------------------------------------*/
