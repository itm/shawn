/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "flood_routing.h"
#ifdef ENABLE_ROUTING

#include <algorithm>

#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/misc/random/basic_random.h"

#include "apps/routing/flood/flood_routing_factory.h"
#include "apps/routing/flood/flood_routing.h"
#include "apps/routing/flood/flood_routing_message.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace flood
	{
		// FloodRoutingMessageHistory:

		FloodRoutingMessageHistory::
			FloodRoutingMessageHistory()
		{}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::
			FloodRoutingMessageHistory( const FloodRoutingMessageHistory& other ) :
		history_(other.history_)
		{}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::
			~FloodRoutingMessageHistory()
		{}
		// ----------------------------------------------------------------------
		int
			FloodRoutingMessageHistory::
			history_size()
			const
			throw()
		{
			return history_.size();
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListConstIterator
			FloodRoutingMessageHistory::
			find_history( const shawn::ConstMessageHandle& mh )
			const
			throw()
		{
			return find(history_.begin(),history_.end(),mh);
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListConstIterator
			FloodRoutingMessageHistory::
			begin_history()
			const
			throw()
		{
			return history_.begin();
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListConstIterator
			FloodRoutingMessageHistory::
			end_history()
			const
			throw()
		{
			return history_.end();
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListIterator
			FloodRoutingMessageHistory::
			find_history_w( const shawn::ConstMessageHandle& mh )
			throw()
		{
			return find(history_.begin(),history_.end(),mh);
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListIterator
			FloodRoutingMessageHistory::
			begin_history_w()
			throw()
		{
			return history_.begin();
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory::MessageHistoryListIterator
			FloodRoutingMessageHistory::
			end_history_w()
			throw()
		{
			return history_.end();
		}
		// ----------------------------------------------------------------------
		void
			FloodRoutingMessageHistory::
			history_push_back( const shawn::ConstMessageHandle& mh, int history_max_size )
			throw()
		{
			if( history_max_size > 0 )
			{
				// cast history_max_size to avoid implicit integer conversion
				if( history_.size() == static_cast<unsigned long>(history_max_size) )
				{
					history_.pop_front();
				}
				history_.push_back( mh );
			}
		}

		// FloodRoutingNodeInfo:

		FloodRoutingNodeInfo::
			FloodRoutingNodeInfo()
		{}
		// ----------------------------------------------------------------------
		FloodRoutingNodeInfo::
			FloodRoutingNodeInfo( const FloodRoutingNodeInfo& other ) :
		message_history_(other.message_history_)
		{}
		// ----------------------------------------------------------------------
		FloodRoutingNodeInfo::
			~FloodRoutingNodeInfo()
		{}
		// ----------------------------------------------------------------------
		const
			FloodRoutingMessageHistory&
			FloodRoutingNodeInfo::
			message_history()
			const
			throw()
		{
			return message_history_;
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessageHistory&
			FloodRoutingNodeInfo::
			message_history_w()
			throw()
		{
			return message_history_;
		}
		// ----------------------------------------------------------------------
		const
			FloodRoutingNodeInfo::FloodRoutingEventHandleSet&
			FloodRoutingNodeInfo::sending_jitter_timer()
			const
			throw()
		{
			return sending_jitter_timer_;
		}
		// ----------------------------------------------------------------------
		FloodRoutingNodeInfo::FloodRoutingEventHandleSet&
			FloodRoutingNodeInfo::sending_jitter_timer_w()
			throw()
		{
			return sending_jitter_timer_;
		}

		// FloodRouting:

		FloodRouting::
			FloodRouting( const string& name,
						  int history_max_size,
						  double probability,
						  double message_sending_jitter_lower_bound,
						  double message_sending_jitter_upper_bound ) :
		name_(name),
		history_max_size_(history_max_size),
		probability_(probability)
		{
			//// urv_sending_decision_
			//urv_sending_decision_.set_lower_bound(0.0);
			//urv_sending_decision_.set_upper_bound(1.0);
			//urv_sending_decision_.set_lower_bound_inclusive(true);
			//urv_sending_decision_.set_upper_bound_inclusive(true);
			//urv_sending_decision_.init();
			// urv_message_sending_jitter_
			urv_.set_lower_bound(message_sending_jitter_lower_bound);
			urv_.set_upper_bound(message_sending_jitter_upper_bound);
			urv_.set_lower_bound_inclusive(true);
			urv_.set_upper_bound_inclusive(true);
			urv_.init();
		}
		//-----------------------------------------------------------------------
		FloodRouting::~FloodRouting()
		{}
		//-----------------------------------------------------------------------
		bool
			FloodRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const int& hops )
		{
			if( hops < 0 )
			{
				return false;
			}
			FloodRoutingMessage* nfrm = new FloodRoutingMessage(*this, mh, hops);
			add_message_to_history(sender,mh);
			INFORM_ROUTING_OBSERVERS( initial_send_flood_routing_message(*this,sender,*nfrm) );
			INFORM_ROUTING_OBSERVERS( send_flood_routing_message(*this,sender,*nfrm) );
			sender.send( nfrm );
			return true;
		}
		//-----------------------------------------------------------------------
		std::string
			FloodRouting::
			name()
			const
			throw()
		{
			return name_;
		}
		//-----------------------------------------------------------------------
		string
			FloodRouting::
			description()
			const
			throw()
		{
			return "This is a decentralized flood routing algorithm. It's associated processor is '"
				+ routing::flood::FloodRoutingProcessorFactory::PROCESSOR_NAME + "'.";
		}
		//-----------------------------------------------------------------------
		void
			FloodRouting::
			timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh,
					 double time, shawn::EventScheduler::EventTagHandle& eth )
			throw()
		{
			//FloodRoutingEventTag* et = static_cast<FloodRoutingEventTag*>( eth.get() );
			RoutingEventTag* et = static_cast<RoutingEventTag*>( eth.get() );
			Node& owner = et->node_w();
			FloodRoutingNodeInfo& rni = node_info_w(owner);

			if( eh == find_sending_jitter_timer( owner, eh ) )
			{
				// Shrink the jitter timer set
				bool erased = static_cast<bool>( rni.sending_jitter_timer_w().erase( eh ) );
				assert( erased );
				// Fetch the message ...
				FloodRoutingMessage* nfrm = static_cast<FloodRoutingMessage*>( et->message_handle().get() );
				// .. and send it!
				INFORM_ROUTING_OBSERVERS( send_flood_routing_message(*this,owner,*nfrm) );
				owner.send( nfrm );
			}
		}
		//-----------------------------------------------------------------------
		void
			FloodRouting::
			observers_added(FloodRoutingObserver& obs)
			throw()
		{
			// Ignore
		}
		//-----------------------------------------------------------------------
		int
			FloodRouting::
			history_max_size()
			const
			throw()
		{
			return history_max_size_;
		}
		//-----------------------------------------------------------------------
		void
			FloodRouting::
			boot( shawn::Node& owner )
			throw()
		{
			init( owner.world_w() );
		}
		//-----------------------------------------------------------------------
		bool
			FloodRouting::
			process_flood_routing_message( shawn::Node& owner, const FloodRoutingMessage& frm )
			throw()
		{
			double now = owner.current_time();
			FloodRoutingNodeInfo& rni = node_info_w(owner);

			INFORM_ROUTING_OBSERVERS( receive_flood_routing_message(*this,owner,frm) );
			//  Already seen? --> ignore
			if( message_in_history( owner, frm.application_message() ) )
			{
				return true;
			}
			INFORM_ROUTING_OBSERVERS( receive_flood_routing_message_for_me(*this,owner,frm) );
			owner.receive( frm.application_message() );
			add_message_to_history( owner, frm.application_message() );
			// Resend?
			if( frm.time_to_live() == 0 )
			{
				return true;
			}
			else if( probabilistic_sending_decision() )
			{
				FloodRoutingMessage* nfrm = new FloodRoutingMessage(frm);
				EventScheduler& es = owner.world_w().scheduler_w();
				//FloodRoutingEventTag* et = new FloodRoutingEventTag(owner,nfrm);
				RoutingEventTag* et = new RoutingEventTag(owner,nfrm);
				EventScheduler::EventHandle eh = es.new_event(*this, now + urv_, et );
				rni.sending_jitter_timer_w().insert( eh );
			}
			return true;
		}
		//-----------------------------------------------------------------------
		inline bool
			FloodRouting::
			probabilistic_sending_decision()
			throw()
		{
			return ( probability_ == 1.0 ? true : ( shawn::uniform_random_0i_1i() <= probability_ ) );
		}
		//-----------------------------------------------------------------------
		void
			FloodRouting::
			add_message_to_history( const shawn::Node& owner, const shawn::ConstMessageHandle& mh )
			throw()
		{
			FloodRoutingNodeInfo& rni = node_info_w(owner);
			rni.message_history_w().history_push_back( mh, history_max_size_ );
		}
		//-----------------------------------------------------------------------
		bool
			FloodRouting::
			message_in_history( const shawn::Node& owner, const shawn::ConstMessageHandle& mh )
			const
			throw()
		{
			const FloodRoutingNodeInfo& rni = node_info(owner);
			FloodRoutingMessageHistoryConstIterator it = rni.message_history().find_history( mh );
			if( it == rni.message_history().end_history() )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		//-----------------------------------------------------------------------
		const shawn::EventScheduler::EventHandle
			FloodRouting::
			find_sending_jitter_timer( const shawn::Node& owner, const shawn::EventScheduler::EventHandle eh )
			const
			throw()
		{
			const FloodRoutingNodeInfo& rni = node_info(owner);
			FloodRoutingEventHandleSetConstIterator it = rni.sending_jitter_timer().find( eh );
			if( it != rni.sending_jitter_timer().end() )
			{
				return *it;
			}
			return NULL;
		}
		//-----------------------------------------------------------------------
		shawn::EventScheduler::EventHandle
			FloodRouting::
			find_sending_jitter_timer_w( const shawn::Node& owner, shawn::EventScheduler::EventHandle eh )
			throw()
		{
			FloodRoutingNodeInfo& rni = node_info_w(owner);
			FloodRoutingEventHandleSetIterator it = rni.sending_jitter_timer_w().find( eh );
			if( it != rni.sending_jitter_timer().end() )
			{
				return *it;
			}
			return NULL;
		}
		//-----------------------------------------------------------------------
		void
			FloodRouting::
			delete_all_sending_jitter_timer( shawn::Node& owner )
			throw()
		{
			FloodRoutingNodeInfo& rni = node_info_w( owner );
			EventScheduler& es = owner.world_w().scheduler_w();
			for( FloodRoutingEventHandleSetIterator it = rni.sending_jitter_timer_w().begin();
				 it != rni.sending_jitter_timer_w().end(); ++it)
			{
				es.delete_event( *it );
			}
			rni.sending_jitter_timer_w().clear();
		}
	}
}
#endif

