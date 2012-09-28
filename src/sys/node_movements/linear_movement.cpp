/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/linear_movement.h"
#include "sys/util/defutils.h"
#include "sys/misc/box.h"
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

namespace shawn
{
//#define LM_DEBUG(x)  std::cerr << "LinearMovement::" << x << std::endl;
#define LM_DEBUG(x);

    LinearMovement::
		LinearMovement() : event_handle_(NULL)
    {
        //event_handle_ = NULL;
    }
    // ----------------------------------------------------------------------
    LinearMovement::
        ~LinearMovement()
    {
//       bool has_event_handle = (event_handle_ != NULL);
        if ((event_handle_ != NULL)&&(!world_->scheduler_w().empty()))
        {
            world_->scheduler_w().delete_event(event_handle_);
            event_handle_ = NULL;
        }
/*
        if ((event_handle_ = world_->scheduler().find_event(*this)) != NULL)
        {
            world_->scheduler_w().delete_event(event_handle_);
        }
         assert( world_->scheduler().find_event(*this) == NULL );
*/
    }
    // ----------------------------------------------------------------------
    void
        LinearMovement::
        init( Node& v )
        throw()
    {
        LM_DEBUG("init: Entering");

        set_node(v);
        pos_ = Vec(0,0,0);
        adjust_velocity();

        starting_time_ = world_->current_time();
        arrival_time_ = arrival_time();

        movement_initial_boxes();
        boxes_changed();

        LM_DEBUG("init: Done" );
    }
    // ----------------------------------------------------------------------
    void
        LinearMovement::
        init_from( Node& v,
        NodeMovement& nm )
        throw()
    {
        LM_DEBUG("init_from: Entering" );

        set_node(v);
        pos_ = nm.position();

        adjust_velocity();

        starting_time_ = world_->current_time();
        arrival_time_ = arrival_time();

        movement_boxes_from(nm);
        boxes_changed();


        LM_DEBUG("init_from: Done" );
    }
    // ----------------------------------------------------------------------
    bool
        LinearMovement::
        set_position( const Vec& p )
        throw()
    {
        LM_DEBUG("set_position("<<p<<"): Entering" );

        pos_ = p;
        starting_time_ = world_->current_time();
        arrival_time_ = arrival_time();
		adjust_velocity();

        if(movement_boxes_violated(p))
            movement_update_boxes();

        LM_DEBUG("set_position("<<p<<"): Done" );
        return true;
    }
    // ----------------------------------------------------------------------
    Vec
        LinearMovement::
        position( void )
        const throw()
    {
        if (world_->current_time() >= arrival_time_)
		{
			LM_DEBUG("position: dest: " << dest_ << ", because current_time("<< world_->current_time()
				<<") >= arrival_time_("<< arrival_time_ <<")" );
            return dest_;
		}

		Vec p = pos_ + velocity_ * (world_->current_time() - starting_time_);
		LM_DEBUG("position: calculated: " << p );
		return p;
    }
    // ----------------------------------------------------------------------
    Vec
        LinearMovement::
        velocity( void )
        const throw()
    {
//         if (world_->current_time() >= arrival_time_)
//             return Vec(0,0,0);
         if (world_->current_time() > arrival_time_)
            return Vec(0,0,0);

        return velocity_;
    }

    // ----------------------------------------------------------------------
	Vec
		LinearMovement::
		destination( void )
		const throw()
	{
		return dest_;
	}
	// ----------------------------------------------------------------------
    std::string
        LinearMovement::
        name( void )
        const throw()
    {
        return std::string("LinearMovement");
    }
    // ----------------------------------------------------------------------
    void
        LinearMovement::
        set_parameters(double velocity, Vec destination, World &world)
    {
        LM_DEBUG("set_parameters(velocity: "<<velocity<<", destination:"<< destination <<")" );

        skalar_velocity_ = velocity;
        dest_ = destination;
        world_ = &world;
    }

    // ----------------------------------------------------------------------
    void LinearMovement::timeout(EventScheduler & event_scheduler,
        EventScheduler::EventHandle event_handle,
        double time,
        EventScheduler::EventTagHandle & event_tag_handle)
        throw()
    {
        LM_DEBUG("timeout(time:"<<time<<"): Entering" );
         assert (node_ != NULL);
        event_handle_ = NULL;
        movement_update_boxes();
        boxes_changed();

        LM_DEBUG("timeout(time:"<<time<<"): Done" );
    }

    // ----------------------------------------------------------------------
    void
        LinearMovement::
		boxes_changed()
		throw()
    {
        LM_DEBUG("boxes_changed: Entering" );

        //Find the earliest time, when a box will be left
        double box_exit_time = std::numeric_limits<double>::max(), tmp_time;
		for(BoxContainer::BoxIterator it = movement_boxes_begin(), end = movement_boxes_end(); it!=end; ++it)
            if( (tmp_time = box_exit_duration(*it)) < box_exit_time)
                box_exit_time = tmp_time;

        box_exit_time += world_->current_time();
        LM_DEBUG("boxes_changed: Earliest grid violation @ " << box_exit_time );

        //Only if the node will leave the grid until its destination, generate an event
        if (box_exit_time <= arrival_time_ && box_exit_time != std::numeric_limits<double>::max())
        {
            LM_DEBUG("boxes_changed: New event created" );
            assert(node_ != NULL);
            event_handle_ = (world_->scheduler_w().new_event(*this, box_exit_time, NULL));
        }
        else
        {
            LM_DEBUG("boxes_changed: No event created (arrival_time("<< arrival_time_ <<
                ") > box_exit_time("<< box_exit_time <<"))" );
        }
    }
    // ----------------------------------------------------------------------
    double
        LinearMovement::
        arrival_time()
        const throw()
    {
        double at = starting_time_ + (dest_ - pos_).euclidean_norm() / skalar_velocity_;
        LM_DEBUG("arrival_time: " << at << " (pos: " << pos_ << ", dest: " << dest_ << ", vel:" << velocity_);
        return at;
    }
    // ----------------------------------------------------------------------
    void
        LinearMovement::
        adjust_velocity()
        throw()
    {
        if ( (dest_-pos_) == Vec(0,0,0) || skalar_velocity_ == 0)
        {
            velocity_ = Vec(0,0,0);
            skalar_velocity_ = 0;
        }
        else
        {
            Vec vel = dest_ - pos_;
            velocity_ = (vel / vel.euclidean_norm())  * skalar_velocity_;
        }

        LM_DEBUG("adjust_velocity: New velocity is: " << velocity_ );

    }
    // ----------------------------------------------------------------------
    double
        LinearMovement::
        box_exit_duration(const Box& b)
        const throw()
    {
        double duration = std::numeric_limits<double>::max(), tmptime;
        Vec pos = position();
        Vec vel = velocity();

        if (!EQDOUBLE(vel.x(), 0))
        {
            tmptime = (b.lower().x() - pos.x())/vel.x();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;

            tmptime = (b.upper().x() - position().x())/velocity().x();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;
        }

        if (!EQDOUBLE(vel.y(), 0))
        {
            tmptime = (b.lower().y() - pos.y())/vel.y();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;

            tmptime = (b.upper().y() - pos.y())/vel.y();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;
        }

        if (!EQDOUBLE(vel.z(), 0))
        {
            tmptime = (b.lower().z() - pos.z())/vel.z();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;

            tmptime = (b.upper().z() - pos.z())/vel.z();
            if ((tmptime > 0)&&(tmptime < duration)&&(!EQDOUBLE(tmptime,0)))
                duration = tmptime;
        }

        LM_DEBUG("box_exit_duration: Position: "<< pos << ", Velocity: " << vel );
        LM_DEBUG("box_exit_duration: Exit box("<<b<<") in " << duration << " time units" );

        return duration;
    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_movements/linear_movement.cpp,v $
* Version $Revision: 388 $
* Date    $Date: 2010-01-22 18:17:17 +0100 (Fri, 22 Jan 2010) $
*-----------------------------------------------------------------------
* $Log: linear_movement.cpp,v $
 *-----------------------------------------------------------------------*/
