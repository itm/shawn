/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "centralized_flood_routing.h"
#ifdef ENABLE_ROUTING

#include <vector>
#include <limits.h>

#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/basic_random.h"
#include "sys/world.h"

#include "apps/routing/centralized_flood/centralized_flood_routing_factory.h"
#include "apps/routing/centralized_flood/centralized_flood_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace flood
	{
		CentralizedFloodRouting::
			CentralizedFloodRouting( const std::string& name,
								double probability,
								double message_sending_jitter_lower_bound,
								double message_sending_jitter_upper_bound,
								double message_delay_per_hop,
								double link_fail_probability ) :
		name_(name),
		probability_(probability),
		message_delay_per_hop_(message_delay_per_hop),
		link_fail_probability_(link_fail_probability)
		{
			// urv_sending_decision_
			/*urv_sending_decision_.set_lower_bound(0.0);
			urv_sending_decision_.set_upper_bound(1.0);
			urv_sending_decision_.set_lower_bound_inclusive(true);
			urv_sending_decision_.set_upper_bound_inclusive(true);
			urv_sending_decision_.init();*/
			// urv_message_sending_jitter_
			urv_.set_lower_bound(message_sending_jitter_lower_bound);
			urv_.set_upper_bound(message_sending_jitter_upper_bound);
			urv_.set_lower_bound_inclusive(true);
			urv_.set_upper_bound_inclusive(true);
			urv_.init();
			// urv_link_fail_
			/*urv_link_fail_.set_lower_bound(0.0);
			urv_link_fail_.set_upper_bound(1.0);
			urv_link_fail_.set_lower_bound_inclusive(true);
			urv_link_fail_.set_upper_bound_inclusive(true);*/
		}
		//-----------------------------------------------------------------------
		CentralizedFloodRouting::
			~CentralizedFloodRouting()
		{}
		//-----------------------------------------------------------------------
		std::string 
			CentralizedFloodRouting::
			name() 
			const 
			throw()
		{
			return name_;
		}
		//-----------------------------------------------------------------------
		std::string 
			CentralizedFloodRouting::
			description() 
			const 
			throw()
		{
			return "This is a centralized flood routing algorithm. It has no associated processor.";
		}
		//-----------------------------------------------------------------------
		void 
			CentralizedFloodRouting::
			observers_added( CentralizedFloodRoutingObserver& obs ) 
			throw()
		{
			// Ignore
		}	
		//-----------------------------------------------------------------------
		void 
			CentralizedFloodRouting::
			timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
					 double time, shawn::EventScheduler::EventTagHandle& eth ) 
			throw()
		{
			RoutingEventTag* et = static_cast<RoutingEventTag*>( eth.get() );
			Node& receiver = et->node_w();
			
			CentralizedFloodRoutingEventHandleSetIterator it = delivery_timer_.find( eh );
			if( it != delivery_timer_.end() && eh == *it )
			{
				bool erased = static_cast<bool>( delivery_timer_.erase( eh ) );
				assert( erased );
				// Fetch the message ...
				Message* rm = et->message_handle().get();
				// ... and give it to the receiver
				INFORM_ROUTING_OBSERVERS( deliver_centralized_flood_transmission( *this, receiver, rm ) );
				receiver.receive( rm );
			}
		}
		//-----------------------------------------------------------------------
		bool 
			CentralizedFloodRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const int& max_hops )
		{
			if( max_hops < 0 )
			{
				return false;
			}
			Message* message = const_cast<shawn::Message*>( mh.get() );
			EventScheduler& es = sender.world_w().scheduler_w();
			vector<int> hop_range(sender.world().node_count(),INT_MAX);
			NodesToExamineMap nodes_to_examine;
			// Hops from source to source are 0
			hop_range[sender.id()] = 0;
			// Start from the source
			nodes_to_examine.insert( NodesToExamineMapValueType(0,&sender) );
			while( ! nodes_to_examine.empty() )
			{
				NodesToExamineMapIterator min_it = nodes_to_examine.begin();
				Node* min_node = min_it->second;
				nodes_to_examine.erase( min_it );
				if( ! probabilistic_sending_decision() )
				{
					// Leave the node out
					continue;
				}
				for( World::adjacency_iterator adj_it = min_node->begin_adjacent_nodes_w();
					 adj_it != min_node->end_adjacent_nodes_w(); ++adj_it )
				{
					assert( hop_range[min_node->id()] < INT_MAX );
					int hops = hop_range[min_node->id()] + 1;
					if( hops < hop_range[adj_it->id()] )
					{
						assert( hop_range[adj_it->id()] == INT_MAX );
						hop_range[adj_it->id()] = hops;
						if( ! link_fail() )
						{
							double t = delivery_time_offset( hops ) + es.current_time();
							RoutingEventTag* et = new RoutingEventTag( *adj_it, message );
							EventScheduler::EventHandle eh = es.new_event( *this, t, et );
							INFORM_ROUTING_OBSERVERS( schedule_centralized_flood_transmission( *this, *adj_it, message ) );
							delivery_timer_.insert( eh );
							if( hops < max_hops )
							{
								nodes_to_examine.insert( NodesToExamineMapValueType( hops, &(*adj_it) ) );
							}
						}
					}
				}
			}
			return true;
		}
		//-----------------------------------------------------------------------
		inline bool 
			CentralizedFloodRouting::
			probabilistic_sending_decision() 
			throw()
		{
			return ( probability_ == 1.0 ? true : ( shawn::uniform_random_0i_1i() <= probability_ ) );
		}
		//-----------------------------------------------------------------------
		inline bool 
			CentralizedFloodRouting::
			link_fail() 
			throw()
		{
			return ( link_fail_probability_ == 0.0 ? false : ( shawn::uniform_random_0i_1i() <= link_fail_probability_ ) );
		}
		//-----------------------------------------------------------------------
		inline double 
			CentralizedFloodRouting::
			delivery_time_offset( int hops ) 
			throw()
		{
			double d = 0.0;
			for( int i = 0; i < hops; ++i )
			{
				d += message_delay_per_hop_ + urv_;
			}
			return d;
		}
		//-----------------------------------------------------------------------
		void 
			CentralizedFloodRouting::
			delete_delivery_timer( shawn::SimulationController& sc ) 
			throw()
		{
			EventScheduler& es = sc.world_w().scheduler_w();
			for( CentralizedFloodRoutingEventHandleSetIterator it = delivery_timer_.begin(); it != 
				 delivery_timer_.end(); ++it )
			{
				es.delete_event( *it );
			}
			delivery_timer_.clear();
		}
	}
}

#endif
