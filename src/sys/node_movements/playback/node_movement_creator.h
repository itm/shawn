/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_NODE_MOVEMENT_CREATOR
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_NODE_MOVEMENT_CREATOR

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include <fstream>
#include <string>
#include <sstream>

namespace shawn
{
   /// This class provides an Interface for any class that implements NodeMovement creation
   /** This processor executes different localization algorithms to estimate
    */

   class NodeMovementCreator

   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      virtual ~NodeMovementCreator();
      ///@}

      ///@name Interface
      ///@{
      /** This method is called whenever the MovementController needs a new
       *  command. It should build a complete MovementInfo object, i.e. implement
       *  a certain mobility model.
       */
      virtual MovementInfo * next_movement() = 0;

      /** This method should reset the MovementController in a way that the next
       *  call to next_movement() produces the same results as the very first call
       *  (assuming no randomization is used)
       */
      virtual void reset() = 0;

    };

 }

 #endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/node_movement_creator.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: node_movement_creator.h,v $
 *-----------------------------------------------------------------------*/
