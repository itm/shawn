/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <cstring>
#include <algorithm>
#include "sys/transm_models/aloha_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"

namespace shawn
{

    // ----------------------------------------------------------------------
	AlohaTransmissionModel::
        AlohaTransmissionModel()
		:received_(0), dropped_(0), packet_failure_(0)
    {
    }

    // ----------------------------------------------------------------------
    AlohaTransmissionModel::
        ~AlohaTransmissionModel()
    {
    }

    // ----------------------------------------------------------------------
    bool
        AlohaTransmissionModel::
        supports_mobility( void )
        const throw(std::logic_error)
    {
        return world().edge_model().supports_mobility();
    }

    // ----------------------------------------------------------------------
    void
        AlohaTransmissionModel::
        init( void )
        throw()
    {
        TransmissionModel::init();
		std::cout << "aloha model is initialized." << std::endl;

		received_ = 0;
		dropped_ = 0;
		packet_failure_ = 0;
    }

    // ----------------------------------------------------------------------
    void
        AlohaTransmissionModel::
        reset( void )
        throw()
    {
		std::cout<<"Aloha transmission model: "
			<< received_ <<" msgs. to be sent." 
			<< dropped_ << " of them dropped, and " 
			<< packet_failure_ << " packets fail." << std::endl;

        TransmissionModel::reset();

        //TODO: isn't there a better way for this?
        while( !aired_messages_.empty() )
        {
			msg_delivery *msg = aired_messages_.back();
			aired_messages_.pop_back();
			MessageInfo* mi = msg->pmi_;
            delete mi;
			delete msg;
        }

		received_ = 0;
		dropped_ = 0;
		packet_failure_ = 0;
    }

    // ----------------------------------------------------------------------
    void
        AlohaTransmissionModel::
        send_message( TransmissionModel::MessageInfo& mi )
        throw()
    {
		received_++;
		drop_collided_frames( mi );
    }

	// ----------------------------------------------------------------------
	void 
		AlohaTransmissionModel::
		find_destinations( msg_delivery* pmsg )
	{
		EdgeModel::adjacency_iterator it, endit;
		for( it = world_w().begin_adjacent_nodes_w( *pmsg->pmi_->src_ ),
             endit = world_w().end_adjacent_nodes_w( *pmsg->pmi_->src_ );
            it != endit;
            ++it )
		{
         Node *cur_dst = &(*it);
         if(transmission_in_range(pmsg->pmi_->src_, cur_dst, pmsg->pmi_))
         {
			   pmsg->push_new_destination( &(*it) );
         }
		}

		//print destination info and othe info of a msg.
		std::cout << "\nSource Node: " << pmsg->pmi_->src_->id() << " send time: " << pmsg->pmi_->time_ <<std::endl;
		std::cout << "The neighbours: "  << std::endl;
		shawn::Node* pNode;
		for( unsigned int i=0; i<pmsg->destinations_.size(); i++)
		{
			pNode = pmsg->destinations_[i]->dest_node_;
			std::cout << "[neighbour" << i << "]: " << pNode->id() << "; ";
		}
		std::cout << std::endl;
		//
	}

	// ----------------------------------------------------------------------
	bool 
		AlohaTransmissionModel::
		remove_collided_destinations( msg_delivery* msg1, msg_delivery* msg2 )
	{
		if( (int)msg1->pmi_->time_ != (int)msg2->pmi_->time_ )
			return false;

		bool removed_ = false;

		for( std::vector<msg_delivery::msg_destination*>::iterator it1=msg1->destinations_.begin(); 
			it1 != msg1->destinations_.end(); it1++)
		{
			// Find the common destination node and set the valid_ tag of collided destination to be false
			for( std::vector<msg_delivery::msg_destination*>::iterator it2=msg2->destinations_.begin();
				it2 != msg2->destinations_.end(); it2++ )
			{
				if( (*it1)->dest_node_==(*it2)->dest_node_ && ((*it1)->valid_||(*it2)->valid_))
				{
					packet_failure_ = (*it1)->valid_? packet_failure_+1 : packet_failure_;
					packet_failure_ = (*it2)->valid_? packet_failure_+1 : packet_failure_;
					(*it1)->valid_ = (*it2)->valid_ = false;
					removed_ = true;
					break;
				}
			}
		}

		//If the two source nodes can communicate with each other, 
		//then they should remove the other one from the destination list.
		//if( world_w().can_communicate_bidi( *msg1->pmi_->src_, *msg2->pmi_->src_ ) )
		//{
		//	std::vector<shawn::Node*>::iterator vc_it;
		//	for( vc_it=msg1->destinations_.begin(); vc_it!=msg1->destinations_.end(); vc_it++)
		//	{
		//		//Compare the address of the Node structure
		//		if( (*vc_it)==msg2->pmi_->src_ )
		//		{
		//			msg1->destinations_.erase( vc_it );
		//			removed_ = true;

		//			packet_failure_++;
		//			break;
		//			
		//		}
		//	}
		//	for( vc_it=msg2->destinations_.begin(); vc_it!=msg2->destinations_.end(); vc_it++)
		//	{
		//		//Compare the address of the Node structure
		//		if( (*vc_it)==msg1->pmi_->src_)
		//		{
		//			msg2->destinations_.erase( vc_it );
		//			removed_ = true;
		//			packet_failure_++;
		//			break;
		//			
		//		}
		//	}
		//}
		return removed_;

	}

	// ----------------------------------------------------------------------
	void
		AlohaTransmissionModel::
		drop_collided_frames( TransmissionModel::MessageInfo& mi )
		throw()
	{
		msg_delivery* new_msg = new msg_delivery( &mi );
		find_destinations( new_msg );

		// Check if the new msg. will interfere with the msgs. already in aired_messages_
		if( aired_messages_.size()>0 )
		{
			std::vector<msg_delivery *>::iterator start_it = aired_messages_.begin(), 
				end_it = aired_messages_.end();
			std::vector<msg_delivery *>::iterator it = find_if(start_it, end_it, same_round(mi));	

			while( it != aired_messages_.end() && (int)(*it)->pmi_->time_==(int)mi.time_ )
			{
				remove_collided_destinations( new_msg, *it ); 
				it++;
			}
			aired_messages_.insert( it, new_msg );
		}
		else
			aired_messages_.push_back( new_msg );
	}


    // ----------------------------------------------------------------------
    void
        AlohaTransmissionModel::
        deliver_messages()
        throw()
    {
		for( std::vector<msg_delivery*>::iterator it=aired_messages_.begin(); it!=aired_messages_.end();)
		{
			msg_delivery* msg = *it;
			it = aired_messages_.erase( it );
			deliver_one_message( msg );
			delete msg;
		}
    }

    // ----------------------------------------------------------------------
    void
        AlohaTransmissionModel::
        deliver_one_message( msg_delivery* msg )
        throw()
    {
		if( msg->pmi_->msg_->is_unicast() )
        {
            ABORT_INCONSISTENT_CONFIGURATION("Unicast is not supported by the aloha transmission model. Implement it -> NOW.");
        }

		bool no_destination = true;
		for( std::vector<msg_delivery::msg_destination*>::iterator it=msg->destinations_.begin();
			it!=msg->destinations_.end();
			it++ )
		{
			if( (*it)->valid_ ){
				(*it)->dest_node_->receive( ConstMessageHandle( msg->pmi_->msg_) );
				no_destination = false;
			}
		}
		dropped_ = no_destination? dropped_+1 : dropped_;
    }
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/aloha_transmission_model.cpp,v $
* Version $Revision: 370 $
* Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
*-----------------------------------------------------------------------
* $Log: aloha_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
