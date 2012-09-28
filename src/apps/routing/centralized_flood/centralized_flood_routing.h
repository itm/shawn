/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_centralized_flood_centralized_flood_ROUTING_H
#define __SHAWN_APPS_ROUTING_centralized_flood_centralized_flood_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <set>

#include "sys/misc/observable.h"
#include "sys/misc/random/uniform_random_variable.h"

#include "apps/routing/routing.h"

#include "apps/routing/flood/flood_routing.h"

namespace shawn{ 
	class Node; 
	class Message;
	typedef class shawn::RefcntPointer<const Message> ConstMessageHandle;
	typedef class shawn::RefcntPointer<Message> MessageHandle;
}

namespace routing
{
	namespace flood
	{
		class CentralizedFloodRouting;

		//typedef routing::RoutingEventTag<> CentralizedFloodRoutingEventTag;

		struct CentralizedFloodRoutingObserver
		{
			virtual void deliver_centralized_flood_transmission( const routing::flood::CentralizedFloodRouting& routing_instance, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
			virtual void schedule_centralized_flood_transmission( const routing::flood::CentralizedFloodRouting& routing_instance, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
		};

		class CentralizedFloodRouting : public Routing<int>,
								   public shawn::EventScheduler::EventHandler,
								   public shawn::Observable<CentralizedFloodRoutingObserver>
		{
		public:
			typedef std::set<shawn::EventScheduler::EventHandle> CentralizedFloodRoutingEventHandleSet;
			typedef std::set<shawn::EventScheduler::EventHandle>::iterator CentralizedFloodRoutingEventHandleSetIterator;
			typedef std::set<shawn::EventScheduler::EventHandle>::const_iterator CentralizedFloodRoutingEventHandleSetConstIterator;
			///
			CentralizedFloodRouting( const std::string& name,
								double probability,
								double message_sending_jitter_lower_bound,
								double message_sending_jitter_upper_bound,
								double message_delay_per_hop_,
								double link_fail_probability );
			virtual ~CentralizedFloodRouting();
			///
			virtual std::string name() const throw();
			///
			virtual std::string description() const throw();
			/// Satisfy the Observable interface
			virtual void observers_added( CentralizedFloodRoutingObserver& obs ) throw();
			///
			virtual void timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, double time, shawn::EventScheduler::EventTagHandle& eth ) throw();
			///
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const int& max_hops );
			///
			void delete_delivery_timer( shawn::SimulationController& sc ) throw();
		protected:
			typedef std::multimap<int,shawn::Node*> NodesToExamineMap; 
			typedef std::multimap<int,shawn::Node*>::iterator NodesToExamineMapIterator; 
			typedef std::multimap<int,shawn::Node*>::value_type NodesToExamineMapValueType; 
			CentralizedFloodRouting( const CentralizedFloodRouting& );
			///
			double delivery_time_offset( int hops ) throw();
			virtual bool probabilistic_sending_decision() throw();
			virtual bool link_fail() throw();
		private:
			const std::string name_;
			/*shawn::UniformRandomVariable urv_sending_decision_;*/
			shawn::UniformRandomVariable urv_;
			/*shawn::UniformRandomVariable urv_link_fail_;*/
			const double probability_;
			const double message_delay_per_hop_;
			const double link_fail_probability_;
			CentralizedFloodRoutingEventHandleSet delivery_timer_;
		};
	}
}

#endif
#endif
