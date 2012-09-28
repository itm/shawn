/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_SIMPLIFIED_GEO_SIMPLIFIED_GEO_ROUTING_H
#define __SHAWN_APPS_ROUTING_SIMPLIFIED_GEO_SIMPLIFIED_GEO_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/vec.h"
#include "sys/misc/observable.h"

#include "apps/routing/routing.h"
#include "apps/routing/misc/routing_misc.h"

#include "apps/routing/geo/geo_routing.h"

namespace routing
{
	namespace geo
	{
		class SimplifiedGeoRouting;
		class SimplifiedGeoRoutingGreedyMessage;
		class SimplifiedGeoRoutingPerimeterMessage;

		struct SimplifiedGeoRoutingObserver
		{
			// Track SimplifiedGeoRoutingGreedyMessage
			virtual void receive_geo_routing_greedy_message( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void send_geo_routing_greedy_message( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& ngm ) throw() = 0;
			// Track SimplifiedGeoRoutingPerimeterMessage
			virtual void receive_geo_routing_perimeter_message( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingPerimeterMessage& pm ) throw() = 0;
			virtual void send_geo_routing_perimeter_message( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingPerimeterMessage& npm ) throw() = 0;
			virtual void geo_routing_destination_unreachable( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingPerimeterMessage& pm ) throw() = 0;
			// 
			virtual void initial_send_geo_routing_message( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& ngm ) throw() = 0;
			virtual void receive_geo_routing_message_for_me( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void no_neighbors_while_initial_sending( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void message_time_to_live_over( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::SimplifiedGeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void destination_unreachable( const routing::geo::SimplifiedGeoRouting& routing_instance, const shawn::Node& owner, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
		};// End of SimplifiedGeoRoutingObserver

		/*! \brief This implementation is based of Greedy Perimeter Stateless Routing by Brad Karp.
		 *         See: "GPSR: Greedy Perimeter Stateless Routing for Wireless Networks" OR
		 *		   "Geographic Routing for Wireless Networks"
		 */
		class SimplifiedGeoRouting : public Routing<GeoRoutingAddress>,
									 public shawn::Observable<SimplifiedGeoRoutingObserver>
		{
			friend class SimplifiedGeoRoutingProcessor;
		public:
			SimplifiedGeoRouting( const std::string& name,  
								  int messages_time_to_live,
								  bool route_always_in_perimeter_mode/*,
								  bool neighborhood_update*/ );
			virtual ~SimplifiedGeoRouting();
			// Satisfy the Routing interface
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const geo::GeoRoutingAddress& destination_address );
			//
			virtual void determine_partitions( shawn::SimulationController& sc ) throw();
			// Satisfy the KeeperManaged interface
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			// For setting messages_time_to_live by the application for example
			void messages_time_to_live(int t) throw();
			//
			int messages_time_to_live() const throw();
			// Satisfy the Observable interface
			virtual void observers_added(SimplifiedGeoRoutingObserver& obs) throw();
		protected:
			SimplifiedGeoRouting( const SimplifiedGeoRouting& other );
			// Methods invoked by the SimplifiedGeoRoutingProcessor
			virtual void boot( shawn::Node& owner ) throw();
			virtual bool process_greedy_message( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) throw();
			virtual bool process_perimeter_message( shawn::Node& owner, const SimplifiedGeoRoutingPerimeterMessage& pm ) throw();
			// GPSR releted methods
			const shawn::Node* perimeter_init_forward( const shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) throw();
			const shawn::Node* face_change( const shawn::Node& owner, SimplifiedGeoRoutingPerimeterMessage& npm, const shawn::Node& out_node ) throw();
			const shawn::Node* right_hand_forward( const shawn::Node& owner, const shawn::Node& in_node ) throw();
			bool greedy_forward_and_send( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm, bool initial_send = false ) throw();
			void update_comm_graph_gg( const shawn::Node& owner, std::set<const shawn::Node*>& blacklisted_neighbors) throw();
			bool intersect( const shawn::Vec& neighbor_position, const shawn::Vec& self_position, 
							const shawn::Vec& location_entering_perimeter_mode, const shawn::Vec& destination_position, 
							shawn::Vec& intersection_position ) throw();
			double norm( double d ) throw();
			const shawn::Vec midpoint( const shawn::Vec&, const shawn::Vec& ) throw();
			//
			virtual bool receive_message_for_me( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) throw();
			///
		private:
			const std::string name_;
			int messages_time_to_live_;
			const bool route_always_in_perimeter_mode_;
			static const double PI;
			routing::misc::Partitions partitions_;
		};// End of SimplifiedGeoRouting
	}
}
#endif
#endif
