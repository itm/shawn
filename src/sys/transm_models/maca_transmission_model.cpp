/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <algorithm>
#include "sys/transm_models/maca_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace shawn
{
	// ----------------------------------------------------------------------
	MacaTransmissionModel::
		MacaTransmissionModel()	:
			received_(0), average_delay_(0), jitter_(0), deliver_num_(0)
	{}

	// ----------------------------------------------------------------------
	MacaTransmissionModel::
		~MacaTransmissionModel()
	{}

	// ----------------------------------------------------------------------
	void 
		MacaTransmissionModel::
		init()
		throw()
	{
		TransmissionModel::init();
		std::cout << "MACA(RTS/CTS) model is initialized." << std::endl;
	}

	// ----------------------------------------------------------------------
	void
		MacaTransmissionModel::
		reset()
		throw()
	{
		average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) : 0;

		 std::cout << "MACA(RTS/CTS): "<<  
            received_ <<" msgs. to be sent. "<< "Average delay is " <<
            average_delay_ << ". Jitter is "<< jitter_ << std::endl;

		 TransmissionModel::reset();
		 received_ = 0;
		 average_delay_ = 0;
		 jitter_ = 0;
		 deliver_num_ = 0;
	}

	// ----------------------------------------------------------------------
    bool
        MacaTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

	// ----------------------------------------------------------------------
	void 
		MacaTransmissionModel::
		send_message( MessageInfo &mi )
		throw()
	{
		received_++;
		insert_message( mi );
	}

	// ----------------------------------------------------------------------
	bool 
		MacaTransmissionModel::
		need_delay( maca_msg *new_msg, maca_msg* msg )
		throw()
	{
		same_round sr( new_msg );
		if( !sr( msg ) )
			return false;

		// The new message need to be delayed if the source nodes of new_msg and msg can communicate with each other,
		// that means the source node of new_msg(N) should have received a RTS message from the source node of msg(O).
		if( world_w().can_communicate_bidi( *new_msg->pmi->src_, *msg->pmi->src_ ) )
			return true;

		// new_msg should also be delayed if N and O have common neighbor, that means N should have received a CTS
		// message from that neighbor.
		for( EdgeModel::adjacency_iterator it=world_w().begin_adjacent_nodes_w( *new_msg->pmi->src_ ), 
			endit=world_w().end_adjacent_nodes_w( *new_msg->pmi->src_ );
			it != endit;
			++it)
		{
			for( EdgeModel::adjacency_iterator it1=world_w().begin_adjacent_nodes_w( *msg->pmi->src_ ), 
			endit1=world_w().end_adjacent_nodes_w( *msg->pmi->src_ );
			it1 != endit1;
			++it1)
			{
				// compare the address of Node structure
				if( &(*it) == &(*it1))
					return true;
			}
		}
		return false;
	}

	// ----------------------------------------------------------------------
	void
		MacaTransmissionModel::
		insert_message( TransmissionModel::MessageInfo &mi )
		throw()
	{
		maca_msg *new_msg = new maca_msg( &mi);

		std::vector<maca_msg*>::iterator it = aired_messages_.begin(), endit = aired_messages_.end();
		it = find_if( it, endit, same_round(new_msg) );

		while( it!=endit )
		{
			if( need_delay( new_msg, *it) )
			{
				new_msg->deliver_round++;
			}
			it++;
		}

		it = find_if( aired_messages_.begin(), aired_messages_.end(), same_round(new_msg) );
		aired_messages_.insert( it, new_msg );
	}

	// ----------------------------------------------------------------------
	void
		MacaTransmissionModel::
		deliver_messages()
		throw()
	{
		if( aired_messages_.empty() )
			return;

		for( std::vector<maca_msg*>::iterator it=aired_messages_.begin(); 
			it != aired_messages_.end() && (*it)->deliver_round<world().simulation_round();)
		{
			maca_msg* msg = (*it);
			it = aired_messages_.erase( it );
			deliver_one_message( msg );
			deliver_num_++;

			double cur_delay = (msg->deliver_round>(int)msg->pmi->time_)? msg->deliver_round-msg->pmi->time_ : 0;
			
			average_delay_ += cur_delay;
			if( deliver_num_>1 )
				jitter_ += (cur_delay>last_delay_)? cur_delay-last_delay_ : last_delay_-cur_delay;;
			last_delay_ = cur_delay;

			delete msg;
		}

		//average_delay_ = deliver_num_>0? average_delay_/deliver_num_ : 0;
		//jitter_ = deliver_num_>1? jitter_/(deliver_num_-1) : 0;
	}

	// ----------------------------------------------------------------------
	void
		MacaTransmissionModel::
		deliver_one_message( maca_msg* msg )
		throw()
	{
		if( msg->pmi->msg_->is_unicast() )
        {
            ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the maca transmission model. Implement it -> NOW.");
        }

		// print msg. info.
		std::cout << "Source Node: " <<  msg->pmi->src_->id() 
			<< ". Msg. send time: " <<  msg->pmi->time_ 
			<< ". Msg. deliver time: " <<  msg->deliver_round
			<< std::endl;
		std::cout << "The neighbours:\n";

        for( EdgeModel::adjacency_iterator it = world_w().begin_adjacent_nodes_w( *msg->pmi->src_ ),
             endit = world_w().end_adjacent_nodes_w( *msg->pmi->src_ );
            it != endit;
            ++it )
		{
         Node *cur_dst = &(*it);
         if(transmission_in_range(msg->pmi->src_, cur_dst, msg->pmi))
         {
            it->receive( ConstMessageHandle(msg->pmi->msg_) );
			   std::cout << it->id() << std::endl;
         }
		}
	}

}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/maca_transmission_model.cpp,v $
* Version $Revision: 370 $
* Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
*-----------------------------------------------------------------------
* $Log: maca_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
