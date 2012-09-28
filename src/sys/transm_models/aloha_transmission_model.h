/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_ALOHA_H
#define __SHAWN_SYS_TRANS_MODELS_ALOHA_H

#include "sys/transmission_model.h"

#include <vector>
#include <functional>

namespace shawn
{

   ///Aloha transmission model where each message is delivered without delay but there is packet loss.
    class AlohaTransmissionModel : public TransmissionModel 
    {
    public:
        ///@name Construction, destruction and lifecycle support
        ///@{ 
        AlohaTransmissionModel();
		virtual ~AlohaTransmissionModel();
        virtual void init( void ) throw();
		virtual void reset( void ) throw();       
        ///@}

        ///@name Transmission model implementation
        ///@{

        /** 
		 *@brief Mobility is depending on mobility support from the edge model
		 *
		 *The edge model is used to determine the 1-hop neighbours 
         *which will receive the message 
		 */
        virtual bool supports_mobility( void ) const throw(std::logic_error);
        
        /**
		 *@brief Stores each message in a vector for delivery at the next simulation round start
		 *
		 *A new structure of msg_delivery will be build and inserted into aired_messages_ 
		 *@see aired_messages_
		 */
        virtual void send_message( TransmissionModel::MessageInfo& mi ) throw();
        
        /**
		 *@brief Delivers all messages which are in the vector
		 *
		 *Call deliver_one_message( msg_delivery* msg ) for each element in aired_messages_
		 *@see aired_messages_
		 */
        virtual void deliver_messages() throw();
    
        ///@}

    protected:
		///Number of received messages
        int received_;

        ///Number of dropped messages
        int dropped_;

		///Number of packets failed to reach the destination
		int packet_failure_;

		/**
		 *@brief Message structure used in Aloha transmission model
		 *
		 *The structure includes the pointer of TransmissionModel::MessageInfo and
		 *the neighbours of the source node who sent the message.
		 *@see msg_destination
		 */
		struct msg_delivery
		{
			/**
			 *@brief The structure for information of a destination node
			 *
			 *The structure includes the pointer of shawn::Node and a tag of this node 
			 *@see shawn::Node
			 */
			struct msg_destination
			{
				shawn::Node* dest_node_;///<pointer to the destination node

				bool valid_;///<\c true if there is no collision at the node

				///Construction
				msg_destination( shawn::Node* pNode )
				{
					dest_node_ = pNode;
					valid_ = true;
				}
			};
			
			///The message received
			TransmissionModel::MessageInfo* pmi_;

			///The vector which stores the neighbours of the source node
			std::vector<msg_destination*> destinations_;

			msg_delivery( TransmissionModel::MessageInfo* mi )
			{
				pmi_ = mi;
			}
			
			///Destruction which deletes all the elements of msg_destination structure in destinations_
			~msg_delivery()
			{
				delete pmi_;
				for(std::vector<msg_destination*>::iterator it=destinations_.begin();
					it!=destinations_.end(); it++)
				{
					delete (*it);
				}
			}

			/**
			 *@brief Push a destination node into the vector destination_
			 *
			 *@param pNode pointer to the destination node
			 */
			void push_new_destination( shawn::Node* pNode )
			{
				destinations_.push_back(new msg_destination(pNode));
			}
		};

		/**
		 *@brief Delivers one message
		 *
		 *Deliver one message to the destinations of the msg with tag valid_ having a \c true value
		 *@param msg message to be delivered
		 */
		virtual void deliver_one_message( msg_delivery* msg ) throw();

		/**
		 *@brief Drop frames that collide
		 *
		 *Set the valid_ tag of the destinations of both mi and other messages in the aired_messages_ 
		 *to be false if collision occurs
		 *@param mi the new-coming message
		 */
		virtual void drop_collided_frames( TransmissionModel::MessageInfo& mi ) throw();
		
		/**
		 *@brief Find the neighbors of the node who send a message
		 *
		 *Find all the destinations of a new-coming messages that are the neighbours of its source node
		 *@param pmsg the message whose source node's neighbours to be determined
		 */
		void find_destinations( msg_delivery* pmsg );

		/**
		 *@brief Set the valid_ tag of destinations where messages collide to be false 
		 *
		 *@return true if any valid_ tag is set false
		 */
		virtual bool remove_collided_destinations( msg_delivery* msg1, msg_delivery* msg2 );

		///Unary function to determine if two messages are of the same simulation round
		struct same_round : public std::unary_function< msg_delivery*,bool>
		{
			TransmissionModel::MessageInfo& mi1;
			explicit same_round(TransmissionModel::MessageInfo& mi) : mi1(mi) {}
			bool operator() ( msg_delivery* mi2 )
			{
				return (int)mi1.time_ == (int)mi2->pmi_->time_;
			};
		};

		///The messages that have been sent by the nodes and are waiting for delivery
		std::vector<msg_delivery*> aired_messages_; 

    };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/aloha_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: aloha_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
