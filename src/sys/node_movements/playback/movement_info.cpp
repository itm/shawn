/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "sys/node_movements/playback/movement_info.h"
#include "sys/vec.h"

using namespace std;

namespace shawn
{

	MovementInfo::MovementInfo() :
    urgency_(Immediately),
	node_(NULL),
	//node_movement_(NULL),
	node_movement_(),
	dispatch_time_(0.0),
	vector_(Vec()),
	velocity_(0.0)
	{
		//vector_ = Vec();
		//velocity_ = 0;
		//urgency_ = Urgency(Immediately);
	}

	// ----------------------------------------------------------------------

	MovementInfo:: ~MovementInfo()
	{}

	// ----------------------------------------------------------------------

	void MovementInfo::set_urgency(Urgency urgency) {
		urgency_ = urgency;
	}

	// ----------------------------------------------------------------------

	void MovementInfo::set_dispatch_time(double dispatch_time) {
		dispatch_time_ = dispatch_time;
	}

	// ----------------------------------------------------------------------

	void MovementInfo::set_node(Node * node) {
		node_ = node;
	}
	// ----------------------------------------------------------------------


	/*void MovementInfo::set_node_movement(NodeMovement * node_movement) {
		node_movement_ = node_movement;
	}*/
	void MovementInfo::set_node_movement(const NodeMovementHandle& node_movement) {
		node_movement_ = node_movement;
	}

	// ----------------------------------------------------------------------

	MovementInfo::Urgency MovementInfo::urgency() {
  		return urgency_;
	}

	// ----------------------------------------------------------------------

	double MovementInfo::dispatch_time() {
		return dispatch_time_;
	}

	// ----------------------------------------------------------------------

	Node* MovementInfo::node() {
		return node_;
	}

	// ----------------------------------------------------------------------

	NodeMovement* MovementInfo::node_movement() {
		return node_movement_.get();
	}
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/movement_info.cpp,v $
 * Version $Revision: 248 $
 * Date    $Date: 2008-09-23 16:28:10 +0200 (Tue, 23 Sep 2008) $
 *-----------------------------------------------------------------------
 * $Log: movement_info.cpp,v $
 *-----------------------------------------------------------------------*/
