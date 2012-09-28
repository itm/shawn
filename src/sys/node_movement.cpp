/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/event_scheduler.h"
#include "sys/world.h"
#include "sys/node_movements/movement_observer.h"

#include <iostream>

namespace shawn
{

	NodeMovement::
		NodeMovement()
		: world_( NULL ),
		  node_( NULL )
	{}
	// ----------------------------------------------------------------------
	NodeMovement::
		~NodeMovement()
	{}
	// ----------------------------------------------------------------------
	int
		NodeMovement::
		simulation_round( void )
		const throw()
	{
		return world().simulation_round();
	}
	// ----------------------------------------------------------------------
	double
		NodeMovement::
		current_time( void ) 
		const throw()
	{
		return world().scheduler().current_time();
	}
	// ----------------------------------------------------------------------
	const World&
		NodeMovement::
		world( void )
		const throw()
	{
		assert( world_ != NULL );
		return *world_;
	}
	// ----------------------------------------------------------------------
	World& 
		NodeMovement::
		world_w( void )
		throw()
	{
		assert( world_ != NULL );
		return *world_;
	}
	// ----------------------------------------------------------------------
	void
		NodeMovement::
		set_node( Node& v )
		throw()
	{
		node_ = &v;
		world_=&v.world_w();
	}
	// ----------------------------------------------------------------------
	Node& 
		NodeMovement::
		node_w( void )
		throw()
	{
		assert( node_ != NULL );
		return *node_;
	}
	// ----------------------------------------------------------------------
	const Node& 
		NodeMovement::
		node( void )
		throw()
	{
		assert( node_ != NULL );
		return *node_;
	}
	// ----------------------------------------------------------------------
	void     
		NodeMovement::
		movement_initial_boxes() 
		throw()
	{
		MovementObservable::movement_initial_boxes(node_w(), position(), velocity());
	}
	// ----------------------------------------------------------------------
	void     
		NodeMovement::
		movement_update_boxes() 
		throw()
	{
		MovementObservable::movement_update_boxes(node_w(), position(), velocity());
	}

   // ----------------------------------------------------------------------
   void
	   NodeMovement::
	   observers_added(MovementObserver& obs) 
	   throw()
   {
	   obs.observer_initial_zone(node_w(), position(), velocity() );
	   movement_update_boxes();
	   boxes_changed();
   }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movement.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: node_movement.cpp,v $
 *-----------------------------------------------------------------------*/
