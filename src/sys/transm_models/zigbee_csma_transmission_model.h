/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_TRANS_MODELS_ZIGBEE_CSMA_H
#define __SHAWN_SYS_TRANS_MODELS_ZIGBEE_CSMA_H

#include "sys/transmission_model.h"

#include <vector>
#include <functional>
#include <algorithm>

namespace shawn
{
	/**
	 *@brief A ZigbeeCsmaTransmissionmodel is a CSMA transmission model.
	 *
	 *It involves the initial backoff, congestion backoff and contention window. 
	 *Its configuration includes \b max_backoff_num, \b min_backoff_exponent and 
	 *\b max_backoff_exponent, \b backoff_period. For slotted CSMA, slot and 
	 *contention_window can also be set through parameters.
	 */
	class ZigbeeCsmaTransmissionModel : public TransmissionModel
	{
	public:
		///@name construction, destruction and support for life cycle
		///@{
		ZigbeeCsmaTransmissionModel();
		~ZigbeeCsmaTransmissionModel();
		/**
		 *@brief Initialize the csma transmission model
		 *
		 *Get the configuration of parameters.
		 */
		virtual void init() throw();
		/**
		 *@brief Reset the csma transmission model
		 *
		 *Clear the statistic vriables and set the parameters to its default value.
		 */
		virtual void reset() throw();
		///@}

		///@name Transmission Model Implementation
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
		 *A new structure of csma_msg will be build and inserted into aired_messages_ 
		 *@param mi The received message
		 *@see aired_messages_
		 */
        virtual void send_message( MessageInfo& mi ) throw();

        /**
		 *@brief Delivers all messages which are in the vector
		 *
		 *The delivery sequence of messages is arranged here, then call deliver_one_message( csma_msg* msg ) 
		 *for each element in aired_messages_
		 *@see aired_messages_
		 */ 
        virtual void deliver_messages() throw();
		///@}

	protected:
		///Number of received messages
		int received_;

		///Number of messages dropped
		int dropped_;

		///Number of messages delivered so far
		int deliver_num_;

		///The delay of last message
		double last_delay_;

		// The number of transmission failure
		int packet_failure_;

		///Number of packets failed to reach the destination
		double average_delay_;

		///The average variation of delay
		double jitter_;

		///The maximum value of the number of backoff before a message failure
		int max_backoff_num_;

		///The minimum value of backoff exponent, a backoff period is random(pow(2,exponent))*backoff_period
		int min_backoff_exponent_;

		///The maximum value of backoff exponent
		int max_backoff_exponent_;

		///The duration of one backoff period
		double backoff_period_;

		///The time slot which should be set a positive value in a slotted CSMA context
		double slot_;

		///The length of the contention window
		int contention_window_;

		/**
		 *@brief Message structure used in csma transmission model
		 *
		 *The structure includes the pointer of TransmissionModel::MessageInfo, its destinations, 
		 *the delivery time and the remaining backoff time.
		 */
		struct csma_msg
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

			bool operator < ( csma_msg msg2 )
			{
				return deliver_time < msg2.deliver_time;
			}
			

			/**
			 *@brief Push a destination node into the vector destination_
			 *
			 *@param pNode pointer to the destination node
			 */
			void push_new_destination( shawn::Node* pNode )
			{
				destinations_.push_back( new msg_destination(pNode) );
			}

			TransmissionModel::MessageInfo *pmi;

			///The vector which stores the neighbours of the source node
			std::vector<msg_destination*> destinations_;

			///The deliver time of the msg., but it is also used as the sense time before the msg. is delivered
			double deliver_time;

			///The backoff exponent, its value is between min_backoff_exponent and max_backoff_exponent
			int BE;

			/**
			 *@brief The number of backoff, its value is initialized to be zero and increased by one each time the msg.
			 *is asked to delay. But a value larger then max_backoff_num means a message failure.
			 */
			int NB;

			/**
			 *@brief The contention window decreased by one if the channel is idle when the node senses the carrier. 
			 *And a zero value means the node wins the channel.
			 */
			int CW;

			/**
			 *@brief construction of structure csma_msg
			 *
			 *The deliver_time of the message is initialized as the sending time, and backoff is given 
			 *a negative value to show that the message has not got any backoff time yet.
			 */
			csma_msg( TransmissionModel::MessageInfo* mi, int backoff_exponent, int contention_window )
			{
				pmi = mi;
				deliver_time = mi->time_;
				BE = backoff_exponent;
				NB = 0;
				CW = contention_window;
			}

			/**
			 *@brief destruction of structure csma_msg
			 *
			 *Free the occupied memory of the message and its destinations
			 */
			~csma_msg()
			{
				delete pmi;
				for( std::vector<msg_destination*>::iterator dest_it=destinations_.begin();
					dest_it!=destinations_.end(); dest_it++ )
				{
					delete (*dest_it);
				}
			}
		};

		struct msg_pre : public std::binary_function< csma_msg*, csma_msg*, bool>
		{
			bool operator()( csma_msg* pmsg1, csma_msg* pmsg2 )
			{
				return pmsg1->deliver_time < pmsg2->deliver_time;
			}
		};

		struct later_than : public std::unary_function<csma_msg*, bool>
		{
			double deliver_time;
			later_than( double time_stamp )
			{
				deliver_time = time_stamp;
			}
			bool operator()( csma_msg* msg2 )
			{
				return deliver_time<msg2->deliver_time;
			}
		};

		///Unary function to determine if two messages are of the same simulation round
		struct same_round : public std::unary_function<csma_msg*, bool>
		{
			int round_;
			same_round( int round )
			{
				round_ = round;
			}
			bool operator() ( csma_msg* msg2 )
			{
				return round_==(int)msg2->deliver_time;
			}
		};

		/**
		 *@brief This method produces a random backoff time for the given message
		 *
		 *@param msg the message that will be given a backoff time
		 */
		void random_backoff( csma_msg* msg ) throw();

		/**
		 *@brief This method decide the backoff boundary of the deliver_time
		 *
		 *@param slot the slot time
		 *@param deliver_time the time whose backoff boundary to be computed
		 *@return the time of the backoff boundary
		 */
		double get_backoff_boundary( double slot, double deliver_time );

		/**
		 *@brief This method is used in unslotted context to move the messages which should be delivered at this round 
		 *from aired_messages_ to cur_round_msg
		 *
		 *@param round the concerning simulation round
		 *@param cur_round_msg the vector that holds the messages which will be delivered at this round
		 *@see aired_messages_
		 **/
		void arrange_one_round_unslotted( int round, std::vector<csma_msg*>& cur_round_msg ) throw();

		/**
		 *@brief This method is used in slotted context to move the messages which should be delivered at this round 
		 *from aired_messages_ to cur_round_msg
		 *
		 *@param round the concerning simulation round
		 *@param cur_round_msg the vector that holds the messages which will be delivered at this round
		 *@see aired_messages_
		 **/
		void arrange_one_round_slotted( int round, std::vector<csma_msg*>& cur_round_msg ) throw();

		/**
		 *@brief See if new_msg will interfere with msg
		 *
		 *@return true if new_msg need delay
		 */
		bool need_delay( csma_msg* new_msg, csma_msg* msg ) throw();

		/**
		 *@brief Deliver one message
		 *
		 *@return true if delivery succeed, false if the message is dropped
		 */
		bool deliver_one_message( csma_msg* msg ) throw();

		/**
		 *@brief Find the neighbors of the node who send a message
		 *
		 *Find all the destinations of a new-coming messages, which are the neighbours of its source node
		 *@param pmsg the message whose source node's neighbours to be determined
		 */
		void find_destinations( csma_msg* pmsg );

		/**
		 *@brief Set the valid_ tags of destinations where messages collide to be false 
		 *
		 *@return true if any valid_ tag is set false
		 */
		bool remove_collided_destinations( csma_msg* msg1, csma_msg* msg2 ) throw();

		///The messages that have been sent by the nodes and are waiting for delivery
		std::vector<csma_msg*> aired_messages_;

	};
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/zigbee_csma_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: zigbee_csma_transmission_model.h,v $
 *-----------------------------------------------------------------------*/
