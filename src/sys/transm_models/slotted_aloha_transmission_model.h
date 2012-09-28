/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_SLOTTED_ALOHA_H
#define __SHAWN_SYS_TRANS_MODELS_SLOTTED_ALOHA_H

#include "sys/transmission_model.h"
#include "sys/transm_models/aloha_transmission_model.h"

#include <vector>
#include <functional>

namespace shawn
{

    /**
	 *@brief Slotted aloha transmission model where each message arrives at the beginning of a slot 
	 *with a given $slot (0.0,1.0]
	 *
	 *Messages whose source nodes are in the same collision domain and send in the same slot will 
	 *interfere with each other
	 */
    class SlottedAlohaTransmissionModel : public AlohaTransmissionModel 
    {
    public:
         ///@name Construction, destruction and lifecycle support
        ///@{
        SlottedAlohaTransmissionModel();
        virtual ~SlottedAlohaTransmissionModel();
        virtual void reset( void ) throw();
		void init( void ) throw();
        ///@}

		///The slot time period which has a default value of 1.0 in which case the model behaves as a aloha model
		static double slot_;
		
	protected:
		/**
		 *@brief Drop frames that collide
		 *
		 *Set the valid_ tag of the destinations of both mi and other messages in the aired_messages_ 
		 *to be false if collision occurs
		 *@param mi the new-coming message
		 */
		virtual void drop_collided_frames( TransmissionModel::MessageInfo& mi ) throw();

		/**
		 *@brief Set the valid_ tag of destinations where messages collide to be false 
		 *
		 *@return true if any valid_ tag is set false
		 */
		virtual bool remove_collided_destinations( msg_delivery* msg1, msg_delivery* msg2 );

		///Unary function to determine if two messages are of the same slot
		struct same_slot : public std::unary_function< msg_delivery*,bool>{
			TransmissionModel::MessageInfo& mi1;
			explicit same_slot(TransmissionModel::MessageInfo& mi) : mi1(mi) {}
			bool operator() ( msg_delivery* mi2 )
			{	
				// return true if two frames are in the same slot of same simulation round
				return ( (int)mi1.time_ == (int)mi2->pmi_->time_ )
					&& ( (int)(mi1.time_/slot_) == (int)(mi2->pmi_->time_/slot_) );
			};
		};

    };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/slotted_aloha_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: slotted_aloha_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
