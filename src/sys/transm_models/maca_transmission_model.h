/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_TRANS_MODELS_MACA_H
#define __SHAWN_SYS_TRANS_MODELS_MACA_H

#include "sys/transmission_model.h"

#include <vector>
#include <functional>

namespace shawn
{
	/**
	 *@brief A MACA transmission model delays the messages if two packets may interfere at some node.
	 *
	 *The model assumes that a node sends a RTS packet before it sends the message, and a destination 
	 *node replies a CTS packet. Any other node who hears the RTS or CTS packet cannot sends a message 
	 *in the same simulation round, so there is no packet loss.
	 */
	class MacaTransmissionModel : public TransmissionModel
	{
	public:
		///@name construction, destruction and support for lifecycle
		///@{
		MacaTransmissionModel();

		~MacaTransmissionModel();

		virtual void init() throw();

		virtual void reset() throw();
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
		 *A new structure of maca_msg will be build and inserted into aired_messages_
		 */
        virtual void send_message( MessageInfo& mi ) throw();

        /**
		 *@brief Delivers all messages which are in the vector 
		 *
		 **Call deliver_one_message( maca_msg* msg ) for each element in aired_messages_
		 */
        virtual void deliver_messages() throw();
		///@}

	protected:
		///The number of messages received
		int received_;

		///The average delay of the packets
		double average_delay_;

		///The average variation of delay
		double jitter_;

		///Number of messages delivered so far
		int deliver_num_;

		///The delay of last message
		double last_delay_;

		/**
		 *@brief Message structure used in MACA transmission model
		 *
		 *The structure includes the pointer of TransmissionModel::MessageInfo and
		 *the delivery round the message.
		 */
		struct maca_msg
		{
			TransmissionModel::MessageInfo *pmi;///<pointer to the message received
			int deliver_round;///<the simulation round when the message will be delivered

			/**
			 *@brief construction
			 *
			 *The deliver_round is initialized as the sending round
			 *@param mi the new-coming message
			 */
			maca_msg( TransmissionModel::MessageInfo* mi)
			{
				pmi = mi;
				deliver_round = (int)mi->time_;
			}

			///destruction
			~maca_msg()
			{
				delete pmi;
			}
		};

		///Unary function to determine if two messages are of the same simulation round
		struct same_round : public std::unary_function<maca_msg*, bool>
		{
			int round_;
			same_round( maca_msg* msg1)
			{
				round_ = msg1->deliver_round;
			}
			bool operator() ( maca_msg* msg2 )
			{
				return round_==msg2->deliver_round;
			}
		};

		/**
		 *@brief Insert a new-coming message into aired_messages_
		 *
		 *@see aired_messages_
		 */
		void insert_message( TransmissionModel::MessageInfo& mi ) throw();

		/**
		 *@brief See if new_msg will interfere with msg
		 *
		 *@return true if new_msg need delay
		 */
		bool need_delay( maca_msg* new_msg, maca_msg* msg ) throw();

		/**
		 *@brief Delivers one message
		 *
		 *Deliver a message to all the neighbours of its source node
		 */
		void deliver_one_message( maca_msg* msg ) throw();

		///The messages that have been sent by the nodes and are waiting for delivery
		std::vector<maca_msg*> aired_messages_;

	};
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/maca_transmission_model.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: maca_transmission_model.h,v $
 *-----------------------------------------------------------------------*/

