/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <algorithm>
#include "sys/transm_models/slotted_aloha_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace shawn
{
	double SlottedAlohaTransmissionModel::slot_;

    // ----------------------------------------------------------------------
	SlottedAlohaTransmissionModel::
        SlottedAlohaTransmissionModel()
    {
		slot_ = 1.0;
    }

    // ----------------------------------------------------------------------
    SlottedAlohaTransmissionModel::
        ~SlottedAlohaTransmissionModel()
    {
    }

	// ----------------------------------------------------------------------
	void 
		SlottedAlohaTransmissionModel::
		init( void )
		throw()
	{
		AlohaTransmissionModel::init();

		const SimulationEnvironment& se = world().simulation_controller().environment();
		slot_ = se.optional_double_param( "slot", 1.0 );
		if( slot_<=0 || slot_>1 )
			slot_ = 1.0;

        std::cout << "slotted_aloha: Initialised. Slot is " <<  slot_ << std::endl;
	}

    // ----------------------------------------------------------------------
    void
        SlottedAlohaTransmissionModel::
        reset( void )
        throw()
    {
		std::cout<<"slotted aloha transmission model[slot=" << slot_ << "]: "
			<< received_ <<" msg to be sent." 
			<< dropped_ << " of them dropped."
			<< packet_failure_ << " packets failed." << std::endl;

        TransmissionModel::reset();

		packet_failure_ = 0;
		received_ = 0;
		dropped_ = 0;
		slot_ = 1.0;
    }

	// ----------------------------------------------------------------------
	bool 
		SlottedAlohaTransmissionModel::
		remove_collided_destinations( msg_delivery *msg1, msg_delivery *msg2 )
	{
		same_slot ss( *msg1->pmi_ );
		if( !ss( msg2 ) )
			return false;

		for( std::vector<msg_delivery::msg_destination*>::iterator it1=msg1->destinations_.begin(); 
			it1 != msg1->destinations_.end(); it1++ )
		{
			for (std::vector<msg_delivery::msg_destination*>::iterator it2=msg2->destinations_.begin();
				it2 != msg2->destinations_.end(); it2++ )
			{
				if( (*it1)->dest_node_ == (*it2)->dest_node_ && ((*it1)->valid_||(*it2)->valid_) )
				{
					packet_failure_ = (*it1)->valid_? packet_failure_+1 : packet_failure_;
					packet_failure_ = (*it2)->valid_? packet_failure_+1 : packet_failure_;
					(*it1)->valid_ = (*it2)->valid_ = false;
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
		//			break;
		//			///////////////how to count dropped_?///////////////////////
		//		///////////////////////////////////////////////////////////
		//		}
		//	}
		//	for( vc_it=msg2->destinations_.begin(); vc_it!=msg2->destinations_.end(); vc_it++)
		//	{
		//		//Compare the address of the Node structure
		//		if( (*vc_it)==msg1->pmi_->src_)
		//		{
		//			msg2->destinations_.erase( vc_it );
		//			removed_ = true;
		//			break;
		//			///////////////how to count dropped_?///////////////////////
		//		///////////////////////////////////////////////////////////
		//		}
		//	}
		//}
		return true;
	}

	// ----------------------------------------------------------------------
	void
		SlottedAlohaTransmissionModel::
		drop_collided_frames( TransmissionModel::MessageInfo& mi )
		throw()
	{
		msg_delivery *new_msg = new msg_delivery( &mi );
		find_destinations( new_msg );

		if( aired_messages_.size()>0 )
		{
			std::vector<msg_delivery*>::iterator start_it = aired_messages_.begin(), 
				end_it = aired_messages_.end();
			std::vector<msg_delivery*>::iterator it = find_if(start_it, end_it, same_slot(mi));

			same_slot ss( mi );

			while( it != aired_messages_.end() && ss( *it ) )
			{
				remove_collided_destinations( new_msg, *it );
				it++;
			}

			aired_messages_.insert( it, new_msg );

		}
		else
			aired_messages_.push_back( new_msg );
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/slotted_aloha_transmission_model.cpp,v $
* Version $Revision: 182 $
* Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: slotted_aloha_transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
