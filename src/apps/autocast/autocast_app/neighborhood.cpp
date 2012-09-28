/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 **                                                                    **
 ************************************************************************/

#include "neighborhood.h"
#include "sys/event_scheduler.h"
#include <iostream>
#ifdef ENABLE_AUTOCAST

// ----------------------------------------------------------------------
namespace autocast {

	using namespace std;

	Neighborhood::Neighborhood(double time_out) : owner_(NULL),time_out_(time_out)
	{
		//timeOut_ = timeOut;
	}

	// ----------------------------------------------------------------------

	void
		Neighborhood::
		set_owner(const shawn::Node& owner)
		throw()
	{
		owner_ = &owner;
	}

	// ----------------------------------------------------------------------

	int Neighborhood::neighborhood_size()
	{
		shrink_neighborhood_table();
		return neighborhood_.size();
	}

	// ----------------------------------------------------------------------

	int Neighborhood::neighborhood_size(double duration)
	{
		if (owner_ == NULL) return 0;
		double now = owner_->world().scheduler().current_time();
		int nh=0;
		for (map< int,NeighborInfo >::iterator it=neighborhood_.begin(); it!=neighborhood_.end(); ++it)
		{
			if (now - duration < it->second.last_heard_time) ++nh;
		}
		return nh;
	}

	// ----------------------------------------------------------------------

	int Neighborhood::get_neighborhood_changed(double duration)
	{
		if (owner_ == NULL) return 0;
		double now = owner_->world().scheduler().current_time();
		int nh=0;
		for (map<int, NeighborInfo >::iterator it=neighborhood_.begin(); it!=neighborhood_.end(); ++it)
		{
			if (now - duration < it->second.first_heard_time) ++nh;
		}
		return nh;
	}
	// ----------------------------------------------------------------------
	void Neighborhood::append_to_neighborhood(int node_id, double next_update_time)
	{
		if (owner_ == NULL) return;
		map<int, NeighborInfo >::iterator it;
		if ((it = neighborhood_.find(node_id)) == neighborhood_.end())
		{
			// new neighbor
			NeighborInfo ni;
			ni.first_heard_time = owner_->world().scheduler().current_time();
			ni.last_heard_time = ni.first_heard_time;
			ni.next_update_time = next_update_time;
			neighborhood_.insert(make_pair(node_id, ni));
		} else {
			it->second.last_heard_time = owner_->world().scheduler().current_time();
			it->second.next_update_time = next_update_time;
		}
		shrink_neighborhood_table();
	}

	// ----------------------------------------------------------------------

	void Neighborhood::append_to_neighborhood(int node_id)
	{
		append_to_neighborhood(node_id, time_out_);
	}

	// ----------------------------------------------------------------------

	void Neighborhood::shrink_neighborhood_table()
	{
		if (owner_ == NULL) return;
      //assert(owner_);
		double now = owner_->world().scheduler().current_time();
		for (map<int, NeighborInfo >::iterator it=neighborhood_.begin(); it!=neighborhood_.end(); )
		{
			if ( now > it->second.last_heard_time + (it->second.next_update_time) ) //* 1.05
			{
				neighborhood_.erase(it++);
			} else {
				++it;
			}
		}
	}

	// ----------------------------------------------------------------------

	void Neighborhood::clear()
	{
		neighborhood_.clear();
	}
}
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/


