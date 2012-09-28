/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_LINEAR_MOVEMENT
#define __SHAWN_SYS_NODE_MOVEMENTS_LINEAR_MOVEMENT

#include "sys/node_movement.h"
#include "sys/world.h"
#include "sys/event_scheduler.h"

namespace shawn
{
   /** The LinearMovement simulates a Node moving on a direct line from its current position
    *  to a destination position at a given speed. 
    */
   class LinearMovement
      : public NodeMovement, public EventScheduler::EventHandler
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
      LinearMovement();
      ///
      virtual ~LinearMovement();
      ///
      virtual void init( Node& ) throw();
      ///
      virtual void init_from( Node&, NodeMovement& ) throw();
      ///@}

      ///@name inherited from NodeMovement
      ///@{
      ///
      /** Sets the new Position to be identical to the given position Vec
        */
      virtual bool set_position( const Vec& ) throw();
      ///
      /** Returns the Position according to the current simulation time.
        */
      virtual Vec position( void ) const throw();
      ///
      /** Returns the speed of the Node according to the current simulation time.
        */
      virtual Vec velocity( void ) const throw();
	  ///
	  /** Returns the destination.
	  */
	  virtual Vec destination( void ) const throw();
      ///
      /** Returns the type of Movement, in this case LinearMovement
        */
      virtual std::string name( void ) const throw();
      ///@}

      /**  This method sets the scalar velocity and the destination vector
        *  for the LinearMovement
        */
      virtual void set_parameters(double velocity, Vec destination, World &world);

      ///Internal.
      /**  This method is called by the EventScheduler when the timeout of a registered Event is reached
        * This is used to inform subscribers when a Box has been reached
        */
      virtual void timeout( EventScheduler&, EventScheduler::EventHandle, double, EventScheduler::EventTagHandle& ) throw ();
   protected:

      /** This method checks at what time a node is going to leave the first of the subscribed boxes and generates
        * a corresponding Event for the Scheduler.
        */
		virtual void boxes_changed() throw();

   private:

      /// The time when the movement reaches its target destination.
      virtual double arrival_time() const throw();

      /// Sets the internal velocity vector. 
      /** It is set according to current position, destination position and scalar velocity */
      virtual void adjust_velocity() throw();

      /** Calculates the DURATION time until the movement will leave the provided box
      */
      virtual double box_exit_duration(const Box&) const throw();

      ///Starting position
      Vec pos_;         
      ///Destination position
      Vec dest_;
      ///Velocity vector
      Vec velocity_;
      ///Scalar velocity used to adjust the velocity vector using adjust_velocity()
      double skalar_velocity_;
      ///Starting time of the movement
      double starting_time_;
      ///The calculated arrival time, which can be calculated using arrival_time()
      double arrival_time_;
      ///A reference of the world instance
      //World * world_; -> now protected in class node_movement
      ///The current event which is used to calculate bounding box crossings
      EventScheduler::EventHandle event_handle_;
   };

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/linear_movement.h,v $
 * Version $Revision: 233 $
 * Date    $Date: 2008-08-18 13:54:18 +0200 (Mon, 18 Aug 2008) $
 *-----------------------------------------------------------------------
 * $Log: linear_movement.h,v $
 *-----------------------------------------------------------------------*/
