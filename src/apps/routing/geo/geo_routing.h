/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_GEO_GEO_ROUTING_H
#define __SHAWN_APPS_ROUTING_GEO_GEO_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/vec.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/misc/observable.h"

#include "apps/routing/routing.h"

namespace routing
{
	namespace geo
	{
		class GeoRouting;
		class GeoRoutingBeaconMessage;
		class GeoRoutingGreedyMessage;
		class GeoRoutingPerimeterMessage;

		class GeoRoutingNeighborhood
		{
			// Friends
			friend class GeoRouting;
			friend class GeoRoutingNodeInfo;
		public:
			struct GeoRoutingNeighborhoodInfo
			{
				// Friends
				friend class GeoRouting;
				friend class GeoRoutingNeighborhood;
				//
				GeoRoutingNeighborhoodInfo(double last_heard_time, const shawn::Vec& position);
				GeoRoutingNeighborhoodInfo( const GeoRoutingNeighborhoodInfo& );
				virtual ~GeoRoutingNeighborhoodInfo();
				double last_heard_time() const throw();
				const shawn::Vec& position() const throw();
				bool blacklisted() const throw();
			protected:
				void last_heard_time(double t) throw();
				void position(const shawn::Vec& p) throw();
				void blacklisted(bool b) throw();
				double last_heard_time_;
				shawn::Vec position_;
				bool blacklisted_;
			};// End of GeoRoutingNeighborhoodInfo
			typedef std::map<const shawn::Node*, GeoRoutingNeighborhoodInfo> GeoRoutingNeighborhoodMap;
			typedef std::map<const shawn::Node*, GeoRoutingNeighborhoodInfo>::iterator GeoRoutingNeighborhoodMapIterator;
			typedef std::map<const shawn::Node*, GeoRoutingNeighborhoodInfo>::const_iterator GeoRoutingNeighborhoodMapConstIterator;

			GeoRoutingNeighborhood();
			GeoRoutingNeighborhood( const GeoRoutingNeighborhood& );
			virtual ~GeoRoutingNeighborhood();
			GeoRoutingNeighborhoodMapConstIterator begin_neighborhood() const throw();
			GeoRoutingNeighborhoodMapConstIterator end_neighborhood() const throw();
			GeoRoutingNeighborhoodMapConstIterator find_neighborhood( const shawn::Node& neighbor ) const throw();
			const GeoRoutingNeighborhoodInfo* neighborhood_info( const shawn::Node& neighbor ) const throw();
		protected:		
			GeoRoutingNeighborhoodMapIterator begin_neighborhood_w() throw();
			GeoRoutingNeighborhoodMapIterator end_neighborhood_w() throw();
			GeoRoutingNeighborhoodMapIterator find_neighborhood_w( const shawn::Node& neighbor ) throw();
			GeoRoutingNeighborhoodInfo* neighborhood_info_w( const shawn::Node& neighbor ) throw();
			void neighborhood_insert( const shawn::Node& neighbor, const GeoRoutingNeighborhoodInfo& info ) throw();
			bool neighborhood_erase( const shawn::Node& neighbor ) throw();
			void neighborhood_erase( GeoRoutingNeighborhoodMapIterator it ) throw();
			//
			GeoRoutingNeighborhoodMap neighborhood_;
		};// End of GeoRoutingNeighborhood

		typedef GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapIterator GeoRoutingNeighborhoodIterator;
		typedef GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapConstIterator GeoRoutingNeighborhoodConstIterator;
		typedef GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo GeoRoutingNeighborhoodInfo;

		struct GeoRoutingAddress
		{
			// Friends
			friend std::ostream& operator<<(std::ostream& os, const GeoRoutingAddress& gra);
			//
			GeoRoutingAddress( const shawn::Vec& destination_position, 
							   double radius, 
							   const shawn::Node* designated_receiver = NULL );
			GeoRoutingAddress( const GeoRoutingAddress& other );
			virtual ~GeoRoutingAddress();
			const shawn::Vec& destination_position() const throw();
			double radius() const throw();
			const shawn::Node* designated_receiver() const throw();
		private:
			const shawn::Vec destination_position_;
			double radius_;
			const shawn::Node* designated_receiver_;
		};// End of GeoRoutingAddress

		class GeoRoutingNodeInfo
		{
			// Friends
			friend class Routing<GeoRoutingAddress,GeoRoutingNodeInfo>;
			friend class GeoRouting;
		public:
			GeoRoutingNodeInfo();
			GeoRoutingNodeInfo( const GeoRoutingNodeInfo& );
			virtual ~GeoRoutingNodeInfo();
			///
			const GeoRoutingNeighborhood& geo_routing_neighborhood() const throw();
			const shawn::EventScheduler::EventHandle beacon_timer() const throw();
			shawn::EventScheduler::EventHandle beacon_timer_w() throw();
		protected:
			GeoRoutingNeighborhood& geo_routing_neighborhood_w() throw();
			void beacon_timer( shawn::EventScheduler::EventHandle eh ) throw();
			///
			GeoRoutingNeighborhood geo_routing_neighborhood_;
			shawn::EventScheduler::EventHandle beacon_timer_;
			//shawn::Vec last_position_;
		};// End of GeoRoutingNodeInfo

		struct GeoRoutingDirectedLink
		{
			// Friends
			friend std::ostream& operator<<(std::ostream& os, const GeoRoutingDirectedLink& grdl);
			friend int operator==(const routing::geo::GeoRoutingDirectedLink& left, const routing::geo::GeoRoutingDirectedLink& right);
			//
			GeoRoutingDirectedLink( const shawn::Node& u, const shawn::Node& v );
			GeoRoutingDirectedLink( const GeoRoutingDirectedLink& other );
			virtual ~GeoRoutingDirectedLink();
		private:
			const shawn::Node* u_;
			const shawn::Node* v_;
		};// End of GeoRoutingDirectedLink

		struct GeoRoutingObserver
		{
			// Track GeoRoutingBeaconMessage
			virtual void receive_geo_routing_beacon_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingBeaconMessage& bm ) throw() = 0;
			virtual void send_geo_routing_beacon_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingBeaconMessage& nbm ) throw() = 0;
			// Track GeoRoutingGreedyMessage
			virtual void receive_geo_routing_greedy_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void send_geo_routing_greedy_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& ngm ) throw() = 0;
			// Track GeoRoutingPerimeterMessage
			virtual void receive_geo_routing_perimeter_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingPerimeterMessage& pm ) throw() = 0;
			virtual void send_geo_routing_perimeter_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingPerimeterMessage& npm ) throw() = 0;
			virtual void geo_routing_destination_unreachable( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingPerimeterMessage& pm ) throw() = 0;
			// 
			virtual void initial_send_geo_routing_message( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& ngm ) throw() = 0;
			virtual void receive_geo_routing_message_for_me( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void no_neighbors_while_initial_sending( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& gm ) throw() = 0;
			virtual void message_time_to_live_over( const routing::geo::GeoRouting& routing_instance, const shawn::Node& owner, const routing::geo::GeoRoutingGreedyMessage& gm ) throw() = 0;
		};// End of GeoRoutingObserver

		/*! \brief This implementation is based of Greedy Perimeter Stateless Routing by Brad Karp.
		 *         See: "GPSR: Greedy Perimeter Stateless Routing for Wireless Networks" OR
		 *		   "Geographic Routing for Wireless Networks"
		 */
		class GeoRouting : public Routing<GeoRoutingAddress,GeoRoutingNodeInfo>,
						   public shawn::EventScheduler::EventHandler,
						   public shawn::Observable<GeoRoutingObserver>
		{
			friend class GeoNeighborhoodCreationTask;
			friend class GeoRoutingProcessor;
		public:
			GeoRouting( const std::string& name, 
						double beacon_interval, 
						double magnitude_in_beacon_interval,
						double beacon_expiration_interval, 
						int messages_time_to_live,
						bool route_always_in_perimeter_mode );
			virtual ~GeoRouting();
			// Satisfy the Routing interface
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const geo::GeoRoutingAddress& destination_address );
			// Satisfy the KeeperManaged interface
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			// For setting messages_time_to_live by the application for example
			void messages_time_to_live(int t) throw();
			//
			int messages_time_to_live() const throw();
			// Satisfy the EventHandler interface
			virtual void timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
								  double time, shawn::EventScheduler::EventTagHandle& eth ) throw();
			// Satisfy the Observable interface
			virtual void observers_added(GeoRoutingObserver& obs) throw();
			///
			bool delete_beacon_timer( shawn::Node& owner ) throw();
			bool delete_all_beacon_timer( shawn::SimulationController& sc ) throw();
			/// 
			bool start_beacon_timer( shawn::Node& owner ) throw();
			bool start_all_beacon_timer( shawn::SimulationController& sc ) throw();
		protected:
			GeoRouting( const GeoRouting& other );
			// Methods invoked by the GeoRoutingProcessor
			virtual void boot( shawn::Node& owner ) throw();
			virtual bool process_beacon_message( shawn::Node& owner, const GeoRoutingBeaconMessage& bm ) throw();
			virtual bool process_greedy_message( shawn::Node& owner, const GeoRoutingGreedyMessage& gm ) throw();
			virtual bool process_perimeter_message( shawn::Node& owner, const GeoRoutingPerimeterMessage& pm ) throw();
			// GPSR releted methods
			const shawn::Node* perimeter_init_forward( const shawn::Node& owner, const GeoRoutingGreedyMessage& gm ) throw();
			const shawn::Node* face_change( const shawn::Node& owner, GeoRoutingPerimeterMessage& npm, const shawn::Node& out_node ) throw();
			const shawn::Node* right_hand_forward( const shawn::Node& owner, const shawn::Node& in_node ) throw();
			bool greedy_forward_and_send( shawn::Node& owner, const GeoRoutingGreedyMessage& gm, bool initial_send = false ) throw();
			void update_comm_graph_gg( const shawn::Node& owner ) throw();
			bool intersect( const shawn::Vec& neighbor_position, const shawn::Vec& self_position, 
							const shawn::Vec& location_entering_perimeter_mode, const shawn::Vec& destination_position, 
							shawn::Vec& intersection_position ) throw();
			double norm( double d ) throw();
			const shawn::Vec midpoint( const shawn::Vec&, const shawn::Vec& ) throw();
			//
			virtual bool receive_message_for_me( shawn::Node& owner, const GeoRoutingGreedyMessage& gm ) throw();
			virtual void send_routing_message( shawn::Node& owner, const shawn::MessageHandle& mh ) throw();
			///
			// Every time, a message is received OR sended, geo_routing_neighborhood_update is invoked. 
			// It returns true if the neighborhood changes.
			// This is the case if:
			// - a new neighbor is inserted
			// - the neighbors position changed
			// - a neighbor is "out of date". This is the case if no (beacon) messages were received from this neighbor.
			//   Thus, last heard time is not updated and the neighbor gets out of date.
			//
			// Method checks for out-of-date neighbors
			bool geo_routing_neighborhood_update( const shawn::Node& owner ) throw();
			// Method checks for changed neighbors and calls the one for out-of-date neighbors above
			bool geo_routing_neighborhood_update( const shawn::Node& owner, const shawn::Node& neighbor, const GeoRoutingNeighborhoodInfo& info ) throw();
			///
			void blacklist_neighbor( const shawn::Node& owner, const shawn::Node& neighbor, bool blacklisted = true ) throw();
		private:
			const std::string name_;
			const double beacon_interval_; // B
			const double magnitude_in_beacon_interval_;
			const double beacon_expiration_interval_; // T 
			int messages_time_to_live_;
			const bool route_always_in_perimeter_mode_;
			shawn::UniformRandomVariable urv_;
			static const double PI;
		};// End of GeoRouting
	}
}
#endif
#endif
