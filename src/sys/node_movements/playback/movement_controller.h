/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_MOVEMEMENT_CONTROLLER
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_MOVEMEMENT_CONTROLLER

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include <fstream>
#include <string>
#include <sstream>

namespace shawn
{
   /// A controller handling the generation and assignment of NodeMovement objects to their respective Nodes
   /** This controller is responsible for the assignment of NodeMovement
    *  objects to their respective Nodes. It holds a NodeMovementCreator
    *  that can be called to generate MovementInfo objects containing a
    *  NodeMovement, a Node, a scheduling time and an urgency indicator.
    *  If the indicator is set to "Urgent" the NodeMovement Object is passed
    *  on to its Node immediately. If it is set to "Delayed" the EventScheduler
    *  is notified and execution is halted until the EventScheduler calls timeout()
    *  on the MovementController.
    */
   class MovementController
      : public EventScheduler::EventHandler
   {
   public:

      ///@name construction / destruction
      ///@{
      ///
      MovementController(SimulationController&);
      ///
      virtual ~MovementController();
      ///@}

      ///@name basic methods inherited from EventHandler
      ///@{
      /** This method should be exclusively called by the EventScheduler.
       *  It calls the private method timeout_action().
       *  \sa EventHandler::timeout()
       */
      virtual void timeout( EventScheduler&, EventScheduler::EventHandle, double, EventScheduler::EventTagHandle& ) throw ();

      /** This method sets the NodeMovementCreator.
        */
      virtual void set_node_movement_creator(NodeMovementCreator*);

      /** This method starts the distribution of NodeMovement objects
        */
      virtual void start();

      /** This method starts the distribution of NodeMovement objects
        */
      virtual void reset();

      private:

      /** This method calls the NodeMovementCreator and passes NodeMovements
        * to their respective Nodes at the correct time.
        */
      virtual void timeout_action();

      SimulationController * sc_;
      NodeMovementCreator * nmc_;
      MovementInfo * movement_info_;
      EventScheduler::EventHandle event_handle_;
      int command_nr_;

   };

 }

 #endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_controller.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: movement_controller.h,v $
 *-----------------------------------------------------------------------*/
