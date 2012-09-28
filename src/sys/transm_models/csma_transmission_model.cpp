/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "sys/transmission_model.h"
#include "sys/transm_models/csma_transmission_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <cmath>

//#define CSMA_DEBUG

#ifdef CSMA_DEBUG
	#define CSMA_DEBUG_OUT( x ) cout << x << endl << flush;
#else
	#define CSMA_DEBUG_OUT( x )
#endif

//Was before 0.00001 since it was not clear whether new_event( now() ) is ok. Well, it is.
#define CSMA_SCHED_EPSILON 0.0

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	CsmaTransmissionModel::
		CsmaTransmissionModel
			(
				double short_inter_frame_spacing,
				double long_inter_frame_spacing,
				int max_short_inter_frame_spacing_size,
				int bandwidth,
				bool slotted_backoff,
				double backoff,
				int max_sending_attempts,
				int backoff_factor_base,
				int min_backoff_exponent,
				int max_backoff_exponent
			)
		:
		short_inter_frame_spacing_(short_inter_frame_spacing),
		long_inter_frame_spacing_(long_inter_frame_spacing),
		max_short_inter_frame_spacing_size_(max_short_inter_frame_spacing_size),
		slotted_backoff_(slotted_backoff),
		backOff_(backoff),
		bandwidth_(bandwidth),
		max_sending_attempts_(max_sending_attempts),
		backoff_factor_base_(backoff_factor_base),
		min_backoff_exponent_(min_backoff_exponent),
		max_backoff_exponent_(max_backoff_exponent),
		nodes_(NULL)
	{}

	// ----------------------------------------------------------------------
	CsmaTransmissionModel::
		~CsmaTransmissionModel()
	{}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		init()
		throw()
	{
		TransmissionModel::init();

		if( nodes_ != NULL )
		{
			delete nodes_;
			nodes_ = NULL;
		}

		//This gives us an Array of Messages for all nodes which can be received in O(1)
		nodes_ = new DynamicNodeArray<CsmaState>(world_w());
		CSMA_DEBUG_OUT("csma: Initialised!");
	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		reset()
		throw()
	{
		TransmissionModel::reset();
		if( nodes_ != NULL )
		{
			delete nodes_;
			nodes_ = NULL;
		}
	}

	// ----------------------------------------------------------------------
	bool
		CsmaTransmissionModel::
		supports_mobility( void )
		const throw(logic_error)
	{
		return world().edge_model().supports_mobility();
	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		send_message( MessageInfo &mi )
		throw()
	{
		csma_msg* new_msg = new csma_msg(&mi, (double) mi.msg_->size() / bandwidth_);
		CSMA_DEBUG_OUT("[id: "<<mi.src_->id()<< "]: new msg=" << new_msg << " at node " << mi.src_->id() << ((mi.msg_->is_ack())?(" ack"):(" msg")) );

		CsmaState& csma_state = (*nodes_)[*(mi.src_)];

		if (mi.msg_->is_ack())
		{
			//acks are treated differently from other message:
			// - they are scheduled for immediate delivery (ignoring IFS
			// - They are not queued, i.e. they can "overtake" regular messages in beeing sent

			csma_state.outgoing_messages_.push_front(new_msg);

			#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh =
			#endif
				world_w().scheduler_w().new_event(*this, world().current_time(), new NodeInfo(mi.src_) );
				CSMA_DEBUG_OUT("[id: "<<mi.src_->id()<< "]: " << eh << " sched in send message at "<< eh->time() << " for ack");
		}
		else
		{
			csma_state.outgoing_messages_.push_back(new_msg);

			if (!csma_state.busy_)
			{
				csma_state.busy_= true;

				#ifdef CSMA_DEBUG
					EventScheduler::EventHandle eh =
				#endif
					world_w().scheduler_w().new_event(*this, world().current_time() + CSMA_SCHED_EPSILON, new NodeInfo(mi.src_) );
					CSMA_DEBUG_OUT("[id: "<<mi.src_->id()<< "]:  " << eh << " sched in send message at " << eh->time() << " for next msg");

			}
		}
	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		timeout(EventScheduler & event_scheduler, EventScheduler::EventHandle event_handle, double time, EventScheduler::EventTagHandle & event_tag_handle)
		throw()
	{

		NodeInfo* ni = dynamic_cast<NodeInfo* >(event_tag_handle.get());
		if (ni != NULL) // This means that a new message must be scheduled
		{
			CsmaState& csma_state = (*nodes_)[*(ni->n_)];
			CSMA_DEBUG_OUT("[id: "<<ni->n_->id()<< "]: " << event_handle << " fired (handle next message) at " << event_handle->time() << " at node " << ni->n_->id() );

			if (!csma_state.outgoing_messages_.empty()) // get next outgoing message
			{
				csma_msg *new_msg = csma_state.outgoing_messages_.front();
				csma_state.outgoing_messages_.pop_front();
				handle_next_message(new_msg);
			}
			else
			{
				//no more messages to be sent
				csma_state.busy_ = false;
			}

		}
		else
		{
			csma_msg* msg = dynamic_cast<csma_msg* >(event_tag_handle.get());

			if(msg != NULL)
			{
				if(!msg->sending_)
				{	//message not on air yet. We should now check wether the medium is free, and eventually send
					CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" << event_handle << " fired (mac access) at " << event_handle->time() << " msg=" << msg );
					CsmaState& csma_state = (*nodes_)[*(msg->pmi_->src_)];

					// Soften assert
					if (csma_state.busy_until_ > ((csma_state.ifs_end_)+(0.001)))
						cout << "csma_transmission_model::timeout (MAC access): busy_until_>ifs_end_! (" << csma_state.busy_until_ << " > " << csma_state.ifs_end_ << ") This must never happen" << endl;
					assert((csma_state.busy_until_ <= ((csma_state.ifs_end_)+(0.001))));
					while( (!msg->pmi_->msg_->is_ack()) && (csma_state.ifs_end_ > msg->deliver_time_) && (msg->sending_attempts_< max_sending_attempts_))
					{

						double wait_periods = shawn::uniform_random_0i_1i()* (int)(pow((float)(backoff_factor_base_),min(min_backoff_exponent_+msg->sending_attempts_, max_backoff_exponent_))-1);
						if (slotted_backoff_)
							wait_periods = int(wait_periods + 0.5);

						msg->deliver_time_ += (backOff_ *  wait_periods);

						//New Event to now + backoff
			            ++msg->sending_attempts_;
						CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: Waiting in timeout (src: " << msg->pmi_->src_->id() << ") " << wait_periods  << ", send attempts: "<< msg->sending_attempts_);
					}

					if (msg->deliver_time_ <= world().current_time() && msg->sending_attempts_< max_sending_attempts_)
					{
						start_send(msg); // If the message has not been sent yet.
					}
					else
					{

						// Reschedule for next media acess
						if (msg->sending_attempts_< max_sending_attempts_)
						{
							#ifdef CSMA_DEBUG
								EventScheduler::EventHandle eh =
							#endif
								world_w().scheduler_w().new_event(*this, msg->deliver_time_ ,msg);
								CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" <<  eh << " sched for new mac access at " << eh->time() << " msg=" << msg );
						}
						else // media access finally failed
						{
							const Message* m = msg->pmi_->msg_.get();
							if (m->has_sender_proc())
							{
			                	(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_CHANNEL_ACCESS_FAILURE, msg->sending_attempts_ );

							}

							#ifdef CSMA_DEBUG
								EventScheduler::EventHandle eh =
							#endif
								world_w().scheduler_w().new_event(*this, world().current_time() + CSMA_SCHED_EPSILON, new NodeInfo(msg->pmi_->src_));
								CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" <<  eh<<" sched in media access at "<< eh->time()<<" for next msg" );
						}

					}

				}
				else
				{	//Otherwise the transmission has terminated
					CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]" << event_handle<<" fired (end_send) at "<< event_handle->time()<<" msg="<<msg );
	                end_send(msg);
				}

			}
		}

	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		deliver_messages()
		throw()
	{}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		start_send(csma_msg* msg)
		throw()
	{
		CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: start sending at " << world().current_time() << " deliver_time_="<< msg->deliver_time_ );

		CsmaState& csma_state = (*nodes_)[*(msg->pmi_->src_)];


		//csma_state.current_message_ = msg;
		if (csma_state.current_message_!=NULL)
		{
			cout << "csma_transmission_model::start_send: current msg was not NULL --> start send though currently receiving.. This must never happen" << endl;
			cout << "csma_transmission_model::start_send: msg->pmi_->src_->id() = "<< msg->pmi_->src_->id() << endl;
			cout << "csma_transmission_model::start_send: csma_state.busy_until_ = " << csma_state.busy_until_ << endl;
			cout << "csma_transmission_model::start_send: csma_state.ifs_end_ = " << csma_state.ifs_end_ << endl;
			cout << "csma_transmission_model::start_send: csma_state.busy_ = " << csma_state.busy_ << endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::pmi_->src_->id() = " << csma_state.current_message_->pmi_->src_->id() << endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::pmi_->time_ = " << csma_state.current_message_->pmi_->time_ << endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::deliver_time_ = " << csma_state.current_message_->deliver_time_<< endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::duration_ = " << csma_state.current_message_->duration_<< endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::sending_ = " << csma_state.current_message_->sending_<< endl;
			cout << "csma_transmission_model::start_send: csma_state.current_message_::sending_attempts_ = " << csma_state.current_message_->sending_attempts_ << endl;
			cout << "csma_transmission_model::start_send: csma_state.size(outgoing_messages_) = " << csma_state.outgoing_messages_.size() << endl;
			for(set<Node*>::iterator it = csma_state.destinations_.begin(), end = csma_state.destinations_.end(); it!=end; ++it)
				cout << "csma_transmission_model::start_send: csma_state::dest = " << (**it).id() << endl;
		}
		assert(csma_state.current_message_ == NULL);

		csma_state.busy_until_ = msg->deliver_time_ + msg->duration_;

		if(!msg->pmi_->msg_->is_ack())  //ack is not followed by its own IFS, but "defers" the IFS of messages it belongs to
		{
			double ifs_time = (msg->pmi_->msg_->size() <=max_short_inter_frame_spacing_size_) ? short_inter_frame_spacing_ : long_inter_frame_spacing_;
			csma_state.ifs_end_ = max(msg->deliver_time_ + msg->duration_ + ifs_time, csma_state.ifs_end_);
			CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: msg clean_busy=" << msg->deliver_time_ + msg->duration_<<" IFS_time " << ifs_time <<" IFS_end " << (*nodes_)[*(msg->pmi_->src_)].ifs_end_ );
		}
		else
		{
			csma_state.ifs_end_ += msg->duration_;
			CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: ack IFS_end " << csma_state.ifs_end_);
		}

		msg->sending_ = true;
		//int count = 0;
		for(EdgeModel::adjacency_iterator it = world_w().begin_adjacent_nodes_w( *msg->pmi_->src_ , EdgeModel::CD_OUT), endit = world_w().end_adjacent_nodes_w( *msg->pmi_->src_ ); it != endit; ++it)
		{
			if (*it != *(msg->pmi_->src_))
			{
            Node *cur_dst = &(*it);
            if(transmission_in_range(msg->pmi_->src_, cur_dst, msg->pmi_))
            {
				   csma_state.destinations_.insert(&(*it));
				   start_receive(&(*it), msg);
            }
		//		count++;
			}
		}
		//if (count==0)
		//	cout << "start send empty receiver set" << endl;
		CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: deliver time: "<< msg->deliver_time_ << " , duration " << msg->duration_ << "sched at "<< msg->deliver_time_ + msg->duration_ << " msg=" << msg);
		#ifdef CSMA_DEBUG
			EventScheduler::EventHandle eh =
		#endif
			world_w().scheduler_w().new_event(*this, msg->deliver_time_ + msg->duration_, msg);

		CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" <<  eh << " sched in start send at " << eh->time() << " msg=" << msg );
	}


	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		end_send(csma_msg* msg)
		throw()
	{
		CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]: end sending at " << world().current_time()  <<" deliver time: "<< msg->deliver_time_ << " , duration " << msg->duration_  << " endtime="<< msg->deliver_time_ + msg->duration_<< " msg="<<msg);
		assert( world().current_time() + 0.00001 >= msg->deliver_time_ + msg->duration_);

		CsmaState& csma_state = (*nodes_)[*(msg->pmi_->src_)];
		//int c = 0;
		//if (csma_state.current_message_ == msg)
		{
			csma_state.current_message_ = NULL;

			for(set<Node*>::iterator it = csma_state.destinations_.begin(), endit = csma_state.destinations_.end(); it != endit; ++it )
			{
				end_receive(*it, msg);
		//		c++;
			}
			csma_state.destinations_.clear();
		}

		//if (c==0)
		//	cout << "end send empty receiver set" << endl;
		const Message* m = msg->pmi_->msg_.get();

		if (m->has_sender_proc())
			(m->sender_proc_w()).process_sent_indication( ConstMessageHandle(msg->pmi_->msg_), shawn::Processor::SHAWN_TX_STATE_SUCCESS, msg->sending_attempts_ );

		// Acks are scheduled separately, so do not schedule next message if this one was an ack
		if (!msg->pmi_->msg_->is_ack())
		{
			#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh =
			#endif
				world_w().scheduler_w().new_event(*this, world().current_time() + CSMA_SCHED_EPSILON, new NodeInfo(msg->pmi_->src_));
				CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" << eh<<" sched in end send at "<< eh->time() << " for next msg");
		}
		else
		{
			CSMA_DEBUG_OUT("[id: "<< msg->pmi_->src_->id() << "]:" << "no sched in end send, was ack");
		}

	}

	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		start_receive(Node* target, csma_msg* msg)
		throw()
	{

		CSMA_DEBUG_OUT("[id: "<< target->id() << "]: start receive " << target->id() << " at " << world().current_time() << " delivertime_=" << msg->deliver_time_);
		CsmaState& csma_state = (*nodes_)[*target];

		// If target is not busy --> no collision (so far)
		if ( csma_state.busy_until_ <= msg->deliver_time_ )
		{

			csma_state.current_message_ = msg;
			if(!msg->pmi_->msg_->is_ack()) // calc end of inter frame spacing
			{
				double ifs_time = (msg->pmi_->msg_->size() <=max_short_inter_frame_spacing_size_) ? short_inter_frame_spacing_ : long_inter_frame_spacing_;
				csma_state.ifs_end_ = max( msg->deliver_time_ + msg->duration_ + ifs_time, csma_state.ifs_end_ );
				CSMA_DEBUG_OUT("[id: "<< target->id() << "]: msg clean_busy=" << msg->deliver_time_ + msg->duration_ << " IFS_time " << ifs_time << " IFS_end " << csma_state.ifs_end_ );
			}
			else
			{
				//Ack is not followed by its own IFS, but "defers" the IFS of messages it belongs to
				csma_state.ifs_end_ = max(csma_state.ifs_end_ + msg->duration_, msg->deliver_time_ + msg->duration_);
				CSMA_DEBUG_OUT("[id: "<< target->id() << "]: ack IFS_end " << csma_state.ifs_end_ );
			}

		}
		else
		{
			// this is collision --> always long interframe spacing
			csma_state.ifs_end_ = max( msg->deliver_time_ + msg->duration_ + long_inter_frame_spacing_, csma_state.ifs_end_ );
			CSMA_DEBUG_OUT("[id: "<< target->id() << "] COLLISION:  msg clean_busy=" << msg->deliver_time_ + msg->duration_ << " IFS_time " << long_inter_frame_spacing_ << " IFS_end " << csma_state.ifs_end_ );

			csma_state.current_message_ = NULL;
		}

		// busy until latest packet is delivered
		if (csma_state.busy_until_ < msg->deliver_time_ + msg->duration_)
			csma_state.busy_until_ = msg->deliver_time_ + msg->duration_;
	}


	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		end_receive(Node* target, csma_msg* msg)
		throw()
	{
		CsmaState& csma_state = (*nodes_)[*target];

		if (csma_state.current_message_ == msg) // original message is still set --> no collision
		{
			csma_state.current_message_ = NULL;
			CSMA_DEBUG_OUT("[id: "<< target->id() << "]:" << target->id() << " receives msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() << " IFS_end " << csma_state.ifs_end_ );
			target->receive(ConstMessageHandle(msg->pmi_->msg_));
		}
		else // collision
		{
			CSMA_DEBUG_OUT("[id: "<< target->id() << "]:" << "---- COLLISION at "<< target->id()<< " of msg from " <<  msg->pmi_->src_->id() << " at " << world().current_time() );
			target->receive_dropped(ConstMessageHandle(msg->pmi_->msg_));
		}

	}


	// ----------------------------------------------------------------------
	void
		CsmaTransmissionModel::
		handle_next_message(csma_msg *new_msg)
		throw()
	{
		CsmaState& csma_state = (*nodes_)[*(new_msg->pmi_->src_)];

		if(new_msg->pmi_->msg_->is_ack())
		{
			//send ack without csma

			#ifdef CSMA_DEBUG
				if ( csma_state.ifs_end_ <= world().current_time() )
				{
					CSMA_DEBUG_OUT("[id: "<< new_msg->pmi_->src_->id() << "]: curtime=" << world().current_time() << " ifs_end="<< csma_state.ifs_end_ << " at " << new_msg->pmi_->src_->id() );
				}
			#endif

			assert(csma_state.ifs_end_ > world().current_time());
			assert(csma_state.ifs_end_ > world().current_time() + new_msg->duration_);

			new_msg->pmi_->time_ = world().current_time();
			new_msg->deliver_time_ = new_msg->pmi_->time_;
			start_send(new_msg);
		}
		else
		{
			new_msg->pmi_->time_ = max(csma_state.ifs_end_, world().current_time());

			    //New Event to now + backoff
			double wait_periods = shawn::uniform_random_0i_1i()* (int)(pow((float)(backoff_factor_base_),min_backoff_exponent_)-1);

			if (slotted_backoff_)
				wait_periods = int(wait_periods + 0.5);

			new_msg->pmi_->time_ += backOff_ *  wait_periods;
            ++new_msg->sending_attempts_;

			CSMA_DEBUG_OUT("[id: "<< new_msg->pmi_->src_->id() << "]: Waiting in handle_next_message (src: " << new_msg->pmi_->src_->id() << ") " << wait_periods << ", send attempts: "<< new_msg->sending_attempts_ );

			new_msg->deliver_time_ = new_msg->pmi_->time_;

			// Create a new Event. Important is the MessageTag (new_msg). It defines which message will be send
			assert(!new_msg->sending_);

			#ifdef CSMA_DEBUG
				EventScheduler::EventHandle eh =
			#endif
				world_w().scheduler_w().new_event(*this, new_msg->pmi_->time_,new_msg);
				CSMA_DEBUG_OUT("[id: "<< new_msg->pmi_->src_->id() << "]: " <<eh << " sched in handle next (2) at " << eh->time() << " msg=" << new_msg );
		}
	}


}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/csma_transmission_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: csma_transmission_model.cpp,v $
*-----------------------------------------------------------------------*/
