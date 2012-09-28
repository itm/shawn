/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "geo_routing.h"
#ifdef ENABLE_ROUTING

#include <float.h>
#include <algorithm>

#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"

#include "apps/routing/geo/geo_routing_factory.h"
#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/geo_routing_message.h"
#include "apps/routing/geo/geo_routing_processor.h"

using namespace std;
using namespace shawn;

const double routing::geo::GeoRouting::PI = 3.14159265;

namespace routing
{
	namespace geo
	{
		// GeoRoutingNeighborhoodInfo:

		GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			GeoRoutingNeighborhoodInfo(double last_heard_time, const shawn::Vec& position) :
		last_heard_time_(last_heard_time),
		position_(position),
		blacklisted_(false)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			GeoRoutingNeighborhoodInfo( const GeoRoutingNeighborhoodInfo& other ) :
		last_heard_time_(other.last_heard_time_),
		position_(other.position_),
		blacklisted_(other.blacklisted_)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			~GeoRoutingNeighborhoodInfo()
		{}
		// ----------------------------------------------------------------------
		double
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			last_heard_time()
			const
			throw()
		{
			return last_heard_time_;
		}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			position()
			const
			throw()
		{
			return position_;
		}
		// ----------------------------------------------------------------------
		bool
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			blacklisted()
			const
			throw()
		{
			return blacklisted_;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			last_heard_time(double t)
			throw()
		{
			last_heard_time_ = t;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			position(const shawn::Vec& p)
			throw()
		{
			position_ = p;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo::
			blacklisted(bool b)
			throw()
		{
			blacklisted_ = b;
		}

		// GeoRoutingNeighborhood:

		GeoRoutingNeighborhood::
			GeoRoutingNeighborhood()
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNeighborhood::
			GeoRoutingNeighborhood( const GeoRoutingNeighborhood& other ) :
		neighborhood_(other.neighborhood_)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNeighborhood::
			~GeoRoutingNeighborhood()
		{}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapConstIterator
			GeoRoutingNeighborhood::
			begin_neighborhood()
			const
			throw()
		{
			return neighborhood_.begin();
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapConstIterator
			GeoRoutingNeighborhood::
			end_neighborhood()
			const
			throw()
		{
			return neighborhood_.end();
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapConstIterator
			GeoRoutingNeighborhood::
			find_neighborhood( const shawn::Node& neighbor )
			const
			throw()
		{
			return neighborhood_.find(&neighbor);
		}
		// ----------------------------------------------------------------------
		const geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo*
			GeoRoutingNeighborhood::
			neighborhood_info( const shawn::Node& neighbor )
			const
			throw()
		{
			GeoRoutingNeighborhoodMapConstIterator it = neighborhood_.find(&neighbor);
			const GeoRoutingNeighborhoodInfo* info = NULL;
			if( it != neighborhood_.end() )
			{
				info = &it->second;
			}
			return info;
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapIterator
			GeoRoutingNeighborhood::
			begin_neighborhood_w()
			throw()
		{
			return neighborhood_.begin();
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapIterator
			GeoRoutingNeighborhood::
			end_neighborhood_w()
			throw()
		{
			return neighborhood_.end();
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodMapIterator
			GeoRoutingNeighborhood::
			find_neighborhood_w( const shawn::Node& neighbor )
			throw()
		{
			return neighborhood_.find(&neighbor);
		}
		// ----------------------------------------------------------------------
		geo::GeoRoutingNeighborhood::GeoRoutingNeighborhoodInfo*
			GeoRoutingNeighborhood::
			neighborhood_info_w( const shawn::Node& neighbor )
			throw()
		{
			GeoRoutingNeighborhoodMapIterator it = neighborhood_.find(&neighbor);
			GeoRoutingNeighborhoodInfo* info = NULL;
			if( it != neighborhood_.end() )
			{
				info = &it->second;
			}
			return info;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNeighborhood::
			neighborhood_insert( const shawn::Node& neighbor, const GeoRoutingNeighborhoodInfo& info )
			throw()
		{
			GeoRoutingNeighborhoodMapIterator it = neighborhood_.find(&neighbor);
			if(it == neighborhood_.end())
			{
				neighborhood_.insert( GeoRoutingNeighborhoodMap::value_type( &neighbor, info ) );
			}
			else
			{
				it->second.last_heard_time(info.last_heard_time());
				it->second.position(info.position());
				// ... and leave blacklisted unchanged!
			}
		}
		// ----------------------------------------------------------------------
		bool
			GeoRoutingNeighborhood::
			neighborhood_erase( const shawn::Node& neighbor )
			throw()
		{
			GeoRoutingNeighborhoodMapIterator it = neighborhood_.find(&neighbor);
			if(it != neighborhood_.end())
			{
				neighborhood_.erase(it);
				return true;
			}
			return false;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNeighborhood::
			neighborhood_erase( GeoRoutingNeighborhoodMapIterator it )
			throw()
		{
			neighborhood_.erase(it);
		}

		// GeoRoutingAddress:

		GeoRoutingAddress::
			GeoRoutingAddress( const shawn::Vec& destination_position,
							   double radius,
							   const shawn::Node* designated_receiver ) :
		destination_position_(destination_position),
		radius_(radius),
		designated_receiver_(designated_receiver)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingAddress::
			GeoRoutingAddress( const GeoRoutingAddress& other ) :
		destination_position_(other.destination_position_),
		radius_(other.radius_),
		designated_receiver_(other.designated_receiver_)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingAddress::
			~GeoRoutingAddress()
		{}
		// ----------------------------------------------------------------------
		const shawn::Vec&
			GeoRoutingAddress::
			destination_position()
			const
			throw()
		{
			return destination_position_;
		}
		// ----------------------------------------------------------------------
		double
			GeoRoutingAddress::
			radius()
			const
			throw()
		{
			return radius_;
		}
		// ----------------------------------------------------------------------
		const shawn::Node*
			GeoRoutingAddress::
			designated_receiver()
			const
			throw()
		{
			return designated_receiver_;
		}
		// ----------------------------------------------------------------------
		ostream&
			operator<<(ostream& os, const GeoRoutingAddress& gra)
		{
			if( gra.designated_receiver_ )
			{
				return os << "destination_position_ : " << gra.destination_position_
					<< ", radius_ : " << gra.radius_ << ", designated_receiver_/ID : " << gra.designated_receiver_
					<< "/" << gra.designated_receiver_->id() << endl;
			}
			else
			{
				return os << "destination_position_ : " << gra.destination_position_ << ", radius_ : " << gra.radius_;
			}
		}

		// GeoRoutingNodeInfo:

		GeoRoutingNodeInfo::
			GeoRoutingNodeInfo() :
		beacon_timer_(NULL)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNodeInfo::
			GeoRoutingNodeInfo( const GeoRoutingNodeInfo& other ) :
		geo_routing_neighborhood_(other.geo_routing_neighborhood_),
		beacon_timer_(other.beacon_timer_)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingNodeInfo::
			~GeoRoutingNodeInfo()
		{}
		// ----------------------------------------------------------------------
		const GeoRoutingNeighborhood&
			GeoRoutingNodeInfo::
			geo_routing_neighborhood()
			const
			throw()
		{
			return geo_routing_neighborhood_;
		}
		// ----------------------------------------------------------------------
		GeoRoutingNeighborhood&
			GeoRoutingNodeInfo::
			geo_routing_neighborhood_w()
			throw()
		{
			return geo_routing_neighborhood_;
		}
		// ----------------------------------------------------------------------
		void
			GeoRoutingNodeInfo::
			beacon_timer( shawn::EventScheduler::EventHandle eh )
			throw()
		{
			beacon_timer_ = eh;
		}
		// ----------------------------------------------------------------------
		const shawn::EventScheduler::EventHandle
			GeoRoutingNodeInfo::
			beacon_timer()
			const
			throw()
		{
			return beacon_timer_;
		}
		// ----------------------------------------------------------------------
		shawn::EventScheduler::EventHandle
			GeoRoutingNodeInfo::
			beacon_timer_w()
			throw()
		{
			return beacon_timer_;
		}

		// GeoRoutingDirectedLink:

		GeoRoutingDirectedLink::
			GeoRoutingDirectedLink( const shawn::Node& u, const shawn::Node& v ) :
		u_(&u),
		v_(&v)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingDirectedLink::
			GeoRoutingDirectedLink( const GeoRoutingDirectedLink& other ) :
		u_(other.u_),
		v_(other.v_)
		{}
		// ----------------------------------------------------------------------
		GeoRoutingDirectedLink::
			~GeoRoutingDirectedLink()
		{}
		// ----------------------------------------------------------------------
		int
			operator==(const routing::geo::GeoRoutingDirectedLink& left, const routing::geo::GeoRoutingDirectedLink& right)
		{
			return left.u_ == right.u_ && left.v_ == right.v_;
		}
		// ----------------------------------------------------------------------
		ostream&
			operator<<(ostream& os, const GeoRoutingDirectedLink& grdl)
		{
			return os << "(" << grdl.u_ << ", " << grdl.v_ << ")" << "/"
					  << "(" << grdl.u_->id() << ", " << grdl.v_->id() << ")";
		}

		// GeoRouting:

		/*! \brief This implementation is based of Greedy Perimeter Stateless Routing by Brad Karp.
		 *         See: "GPSR: Greedy Perimeter Stateless Routing for Wireless Networks" OR
		 *		   "Geographic Routing for Wireless Networks"
		 */

		GeoRouting::
			GeoRouting( const string& name,
						double beacon_interval,
					    double magnitude_in_beacon_interval,
						double beacon_expiration_interval,
					    int messages_time_to_live,
						bool route_always_in_perimeter_mode ) :
		name_(name),
		beacon_interval_(beacon_interval),
		magnitude_in_beacon_interval_(magnitude_in_beacon_interval),
		beacon_expiration_interval_(beacon_expiration_interval),
		messages_time_to_live_(messages_time_to_live),
		route_always_in_perimeter_mode_(route_always_in_perimeter_mode)
		{
			urv_.set_lower_bound(magnitude_in_beacon_interval_);
			urv_.set_upper_bound(1.0 + magnitude_in_beacon_interval_);
			urv_.set_lower_bound_inclusive(true);
			urv_.set_upper_bound_inclusive(true);
			urv_.init();
		}
		// ----------------------------------------------------------------------
		GeoRouting::
			~GeoRouting()
		{}
		//-----------------------------------------------------------------------
		bool
			GeoRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const GeoRoutingAddress& destination_address )
		{
			GeoRoutingProcessor* grp = sender.get_processor_of_type_w<GeoRoutingProcessor>();
			if( grp == NULL )
			{
				// No GeoRouting node
				return false;
			}

//			GeoRoutingNodeInfo& rni = node_info_w( sender );
			geo_routing_neighborhood_update(sender);

			GeoRoutingGreedyMessage gm =
				GeoRoutingGreedyMessage(*this,mh,destination_address,sender.real_position(),messages_time_to_live_);
			/// Loopback
			if( receive_message_for_me(sender,gm) )
			{
				return true;
			}

			if( !greedy_forward_and_send(sender,gm,true) )
			{
				const Node* right_hand_receiver = perimeter_init_forward(sender,gm);
				if(right_hand_receiver)
				{
					GeoRoutingPerimeterMessage* npm = new GeoRoutingPerimeterMessage(gm,*right_hand_receiver,sender);
					INFORM_ROUTING_OBSERVERS( initial_send_geo_routing_message( *this, sender, *npm ) );
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message( *this, sender, *npm ) );
					send_routing_message( sender, npm );
				}
				else
				{
					INFORM_ROUTING_OBSERVERS( no_neighbors_while_initial_sending( *this, sender, gm ) );
					return false;
				}
			}
			return true;
		}
		// ----------------------------------------------------------------------
		std::string
			GeoRouting::
			name()
			const
			throw()
		{
			return name_;
		}
		// ----------------------------------------------------------------------
		string
			GeoRouting::
			description()
			const
			throw()
		{
			return "This is a decentralized geo routing algorithm. It's associated processor is '"
				+ routing::geo::GeoRoutingProcessorFactory::PROCESSOR_NAME + "'.";
		}
		// ----------------------------------------------------------------------
		void
			GeoRouting::
			messages_time_to_live(int t)
			throw()
		{
			messages_time_to_live_ = t;
		}
		// ----------------------------------------------------------------------
		int
			GeoRouting::
			messages_time_to_live()
			const
			throw()
		{
			return messages_time_to_live_;
		}
		// ----------------------------------------------------------------------
		void
			GeoRouting::
			timeout( EventScheduler& es, EventScheduler::EventHandle eh, double time, EventScheduler::EventTagHandle& eth )
			throw()
		{
			RoutingEventTag* et = static_cast<RoutingEventTag*>( eth.get() );
			Node& owner = et->node_w();
			GeoRoutingNodeInfo& rni = node_info_w(owner);

			if( eh == rni.beacon_timer() )
			{
				// Send a beacon and start the timer for the next beacon
				GeoRoutingBeaconMessage* nbm = new GeoRoutingBeaconMessage( *this, owner.real_position() );
				INFORM_ROUTING_OBSERVERS( send_geo_routing_beacon_message( *this, owner, *nbm ) );

				rni.beacon_timer( NULL );
				RoutingEventTag* net = new RoutingEventTag( *et );
				EventScheduler::EventHandle neh =  es.new_event( *this, time + urv_ * beacon_interval_, net );
				rni.beacon_timer( neh );
				owner.send( nbm );
			}
		}
		// ----------------------------------------------------------------------
		void
			GeoRouting::
			observers_added(GeoRoutingObserver& obs)
			throw()
		{
			// Ignore
		}
		// ----------------------------------------------------------------------
		void
			GeoRouting::
			boot( shawn::Node& owner )
			throw()
		{
			init( owner.world_w() );

			//double now = owner.current_time();
			//GeoRoutingNodeInfo& rni = node_info_w(owner);
			///*rni.last_position( owner.real_position() );*/

			//EventScheduler& es = owner.world_w().scheduler_w();
			//RoutingEventTag* et = new RoutingEventTag(owner);

			//EventScheduler::EventHandle eh = es.new_event( *this, now + urv_ * beacon_interval_, et );
			//rni.beacon_timer( eh );
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			process_beacon_message( shawn::Node& owner, const GeoRoutingBeaconMessage& bm )
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_geo_routing_beacon_message(*this,owner,bm) );
			// Prevent owner from adding himself to his neighbors
			if( &bm.source() != &owner )
			{
//				GeoRoutingNodeInfo& rni = node_info_w(owner);
				// Update neighborhood
				if( geo_routing_neighborhood_update( owner, bm.source(),
					GeoRoutingNeighborhoodInfo( owner.current_time(), bm.source_position() ) ) /*||
					rni.last_position() != owner.real_position()*/ )
				{
					// Only update if neighborhood or own position changes
					update_comm_graph_gg(owner);
				}
			}
			return true;
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			process_greedy_message( shawn::Node& owner, const GeoRoutingGreedyMessage& gm )
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_geo_routing_greedy_message(*this,owner,gm) );
			// Prevent owner from adding himself to his neighbors
			if( &gm.source() != &owner )
			{
//				GeoRoutingNodeInfo& rni = node_info_w(owner);
				// Update neighborhood
				if( geo_routing_neighborhood_update( owner, gm.source(),
					GeoRoutingNeighborhoodInfo(owner.current_time(), gm.source_position() ) ) )
				{
					// Only update if neighborhood or own position changes
					update_comm_graph_gg(owner);
				}
			}
			// Am I the next hop?
			assert( gm.next_hop() );
			if( &owner != gm.next_hop() )
			{
				return true;
			}
			// Message for me?
			if( receive_message_for_me(owner,gm) )
			{
				return true;
			}
			// Message TTL over?
			if( gm.time_to_live() == 0 )
			{
				INFORM_ROUTING_OBSERVERS( message_time_to_live_over(*this,owner,gm) );
				return true;
			}
			if( !greedy_forward_and_send(owner,gm) )
			{
				const Node* right_hand_receiver = perimeter_init_forward(owner,gm);
				if(right_hand_receiver)
				{
					GeoRoutingPerimeterMessage* npm = new GeoRoutingPerimeterMessage(gm,*right_hand_receiver,owner);
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message(*this,owner,*npm) );
					send_routing_message( owner, npm );
				}
			}
			return true;
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			process_perimeter_message( shawn::Node& owner, const GeoRoutingPerimeterMessage& pm )
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_geo_routing_perimeter_message(*this,owner,pm) );
			// Prevent owner from adding himself to his neighbors
			if( &pm.source() != &owner )
			{
//				GeoRoutingNodeInfo& rni = node_info_w(owner);
				// Update neighborhood
				if( geo_routing_neighborhood_update( owner, pm.source(),
					GeoRoutingNeighborhoodInfo(owner.current_time(), pm.source_position() ) ) )
				{
					// Only update if neighborhood or own position changes
					update_comm_graph_gg(owner);
				}
			}
			// Am I the next hop?
			assert( pm.next_hop() );
			if( &owner != pm.next_hop() )
			{
				return true;
			}
			// Message for me?
			if( receive_message_for_me(owner,pm) )
			{
				return true;
			}
			// Message TTL over?
			if( pm.time_to_live() == 0 )
			{
				INFORM_ROUTING_OBSERVERS( message_time_to_live_over(*this,owner,pm) );
				return true;
			}
			bool greedy_forward_successful = false;
			if( (owner.real_position() - pm.destination_address().destination_position()).euclidean_norm() <
				(pm.location_entering_perimeter_mode() - pm.destination_address().destination_position()).euclidean_norm() )
			{
				greedy_forward_successful =
					greedy_forward_and_send(owner,static_cast<const GeoRoutingGreedyMessage&>( pm ));
			}
			// Different to Brad Karp's specification that drops messages sometimes ...
			if( ! greedy_forward_successful )
			{
				const Node* right_hand_receiver = right_hand_forward(owner,pm.source());
				assert( right_hand_receiver != &owner );
				if( pm.first_edge_on_current_face() == GeoRoutingDirectedLink(owner,*right_hand_receiver) )
				{
					// Destination unreachable
					INFORM_ROUTING_OBSERVERS( geo_routing_destination_unreachable(*this,owner,pm) );
					return true;
				}
				else
				{
					GeoRoutingPerimeterMessage* npm = new GeoRoutingPerimeterMessage( pm, owner.real_position() );
					right_hand_receiver = face_change( owner, *npm, *right_hand_receiver );
					assert( right_hand_receiver != &owner );
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message(*this,owner,*npm) );
					send_routing_message( owner, npm );
				}
			}
			return true;
		}
		// ---------------------------------------------------------------------
		const shawn::Node*
			GeoRouting::
			face_change( const shawn::Node& owner, GeoRoutingPerimeterMessage& npm, const shawn::Node& out_node )
			throw()
		{
			Vec intercept;
			const GeoRoutingNodeInfo& rni = node_info(owner);
			const GeoRoutingNeighborhoodInfo* neighborhood_info =
				rni.geo_routing_neighborhood().neighborhood_info( out_node );
			bool i = false;
			if( neighborhood_info )
			{
				// Keep in mind: this is a distributed algorithm.
				// You can only ask the owner for his real_position. DO NOT ask a neighbor for his real_position!
				Vec out_node_pos = neighborhood_info->position();
				i = intersect( out_node_pos, owner.real_position(),
							   npm.location_entering_perimeter_mode(),
							   npm.destination_address().destination_position(),
							   intercept );
			}
			else
			{
				FATAL(this->logger(),"Unexpected error in GeoRouting::face_change.");
				abort();
			}
			const Node* right_hand_receiver = &out_node;
			if( i )
			{
				if( (intercept - npm.destination_address().destination_position()).euclidean_norm() <
					(npm.location_entering_current_face() - npm.destination_address().destination_position()).euclidean_norm() )
				{
					npm.location_entering_current_face(intercept);
					right_hand_receiver = right_hand_forward(owner,out_node);
					right_hand_receiver = face_change(owner,npm,*right_hand_receiver);
					npm.first_edge_on_current_face( GeoRoutingDirectedLink(owner,*right_hand_receiver) );
				}
			}
			npm.next_hop(*right_hand_receiver);
			return right_hand_receiver;
		}
		// ---------------------------------------------------------------------
		const shawn::Node*
			GeoRouting::
			perimeter_init_forward( const shawn::Node& owner, const GeoRoutingGreedyMessage& gm )
			throw()
		{
			const Node* a_min = NULL;
			double b_in = norm( atan2(owner.real_position().y() - gm.destination_address().destination_position().y(),
									  owner.real_position().x() - gm.destination_address().destination_position().x()) );
			double delta_min = 3*PI;
			const GeoRoutingNodeInfo& rni = node_info(owner);

			const GeoRoutingNeighborhood& neighborhood = rni.geo_routing_neighborhood();
			for(GeoRoutingNeighborhoodConstIterator it = neighborhood.begin_neighborhood();
				it != neighborhood.end_neighborhood(); ++it)
			{
				if( ! it->second.blacklisted() )
				{
					double b_a = norm( atan2(owner.real_position().y() - it->second.position().y(),
											 owner.real_position().x() - it->second.position().x()) );
					Vec a_min_pos;
					double delta_b = norm( b_a - b_in );
					if( delta_b < delta_min )
					{
						delta_min = delta_b;
						a_min = it->first;
						a_min_pos = it->second.position();
					}
					else if( delta_b == delta_min )
					{
						assert( a_min );
						a_min = (it->second.position() - owner.real_position()).euclidean_norm() <
								(a_min_pos - owner.real_position()).euclidean_norm() ?
								it->first : a_min;
					}
				}
			}
			return a_min;
		}
		// ---------------------------------------------------------------------
		const shawn::Node*
			GeoRouting::
			right_hand_forward( const shawn::Node& owner, const shawn::Node& in_node )
			throw()
		{
			const Node* a_min = &in_node;
			double b_in = 0.0;
			const GeoRoutingNodeInfo& rni = node_info(owner);
			const GeoRoutingNeighborhoodInfo* neighborhood_info =
				rni.geo_routing_neighborhood().neighborhood_info(in_node);
			if( neighborhood_info )
			{
				Vec in_node_pos = neighborhood_info->position();
				b_in = norm( atan2(owner.real_position().y() - in_node_pos.y(),
								   owner.real_position().x() - in_node_pos.x()) );
			}
			else
			{
				FATAL(this->logger(),"Unexpected error in GeoRouting::right_hand_forward.");
				abort();
			}
			double delta_min = 3*PI;
			const GeoRoutingNeighborhood& neighborhood = rni.geo_routing_neighborhood();
			for(GeoRoutingNeighborhoodConstIterator it = neighborhood.begin_neighborhood();
				it != neighborhood.end_neighborhood(); ++it)
			{
				assert( it->first != &owner );
				if( it->first == &in_node || it->second.blacklisted() )
				{
					continue;
				}
				double b_a = norm( atan2(owner.real_position().y() - it->second.position().y(),
										 owner.real_position().x() - it->second.position().x()) );
				Vec a_min_pos;
				double delta_b = norm( b_a - b_in );
				if( delta_b < delta_min )
				{
					delta_min = delta_b;
					a_min = it->first;
					a_min_pos = it->second.position();
				}
				else if( delta_b == delta_min )
				{
					a_min = (it->second.position() - owner.real_position()).euclidean_norm() <
							(a_min_pos - owner.real_position()).euclidean_norm() ?
							it->first : a_min;
				}
			}
			return a_min;
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			greedy_forward_and_send( shawn::Node& owner, const GeoRoutingGreedyMessage& gm, bool initial_send )
			throw()
		{
			if( route_always_in_perimeter_mode_ )
			{
				return false;
			}

			const Node* n_best = &owner;
			double d_best = (owner.real_position() - gm.destination_address().destination_position()).euclidean_norm();
			const GeoRoutingNodeInfo& rni = node_info(owner);

			const GeoRoutingNeighborhood& neighborhood = rni.geo_routing_neighborhood();
			for( GeoRoutingNeighborhoodConstIterator it = neighborhood.begin_neighborhood();
				 it != neighborhood.end_neighborhood(); ++it )
			{
				double d = (it->second.position() - gm.destination_address().destination_position()).euclidean_norm();

				if( d < d_best )
				{
					n_best = it->first;
					d_best = d;
				}
			}
			if( n_best == &owner )
			{
				return false;
			}
			else
			{
				GeoRoutingGreedyMessage* ngm = new GeoRoutingGreedyMessage( gm, *n_best, owner.real_position() );
				if( initial_send )
				{
					INFORM_ROUTING_OBSERVERS( initial_send_geo_routing_message( *this, owner, *ngm ) );
				}
				INFORM_ROUTING_OBSERVERS( send_geo_routing_greedy_message( *this, owner, *ngm ) );
				send_routing_message( owner, ngm );
				return true;
			}
			return false;
		}
		// ---------------------------------------------------------------------
		void
			GeoRouting::
			update_comm_graph_gg( const shawn::Node& owner )
			throw()
		{
			const GeoRoutingNodeInfo& rni = node_info(owner);
			// Un-blacklist neighbors
			const GeoRoutingNeighborhood& neighborhood = rni.geo_routing_neighborhood();
			for(GeoRoutingNeighborhoodConstIterator n_it = neighborhood.begin_neighborhood();
				n_it != neighborhood.end_neighborhood(); ++n_it)
			{
				blacklist_neighbor( owner, *n_it->first, false );
			}
			// Blacklist neighbors
			for(GeoRoutingNeighborhoodConstIterator v_it = neighborhood.begin_neighborhood();
				v_it != neighborhood.end_neighborhood(); ++v_it)
			{
				for(GeoRoutingNeighborhoodConstIterator w_it = neighborhood.begin_neighborhood();
					w_it != neighborhood.end_neighborhood(); ++w_it)
				{
					if( w_it->first == v_it->first )
					{
						continue;
					}
					else
					{
						Vec m = midpoint( owner.real_position(), v_it->second.position() );
						if( (m - w_it->second.position()).euclidean_norm() <
							(owner.real_position() - m).euclidean_norm() )
						{
							blacklist_neighbor(owner,*v_it->first);
							break;
						}
					}
				}
			}
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			intersect( const shawn::Vec& neighbor_position,
					   const shawn::Vec& self_position,
					   const shawn::Vec& location_entering_perimeter_mode,
					   const shawn::Vec& destination_position,
					   shawn::Vec& intersection_position )
			throw()
		{
			volatile double m_0 = ( self_position.y() - neighbor_position.y() ) /
								  ( self_position.x() - neighbor_position.x() );
			volatile double m_1 = ( destination_position.y() - location_entering_perimeter_mode.y() ) /
								  ( destination_position.x() - location_entering_perimeter_mode.x() );
			volatile double b_0 = neighbor_position.y() - m_0 * neighbor_position.x();
			volatile double b_1 = location_entering_perimeter_mode.y() - m_1 * location_entering_perimeter_mode.x();
			// Slopes are not equal
			if( m_0 != m_1 )
			{
				volatile double x_intercept = ( b_0 - b_1 )/( m_1 - m_0 );
				volatile double y_intercept = m_1 * x_intercept + b_1;
				// Intercept in both line segments
				if( x_intercept < max(neighbor_position.x(),self_position.x()) &&
					x_intercept > min(neighbor_position.x(),self_position.x()) &&
					y_intercept < max(neighbor_position.y(),self_position.y()) &&
					y_intercept > min(neighbor_position.y(),self_position.y()) &&
					x_intercept < max(location_entering_perimeter_mode.x(),destination_position.x()) &&
					x_intercept > min(location_entering_perimeter_mode.x(),destination_position.x()) &&
					y_intercept < max(location_entering_perimeter_mode.y(),destination_position.y()) &&
					y_intercept > min(location_entering_perimeter_mode.y(),destination_position.y()) )
				{
					intersection_position = Vec(x_intercept,y_intercept);
					return true;
				}
			}
			return false;
		}
		// ---------------------------------------------------------------------
		double
			GeoRouting::
			norm(double d)
			throw()
		{
			while( d < 0 )
			{
				d += 2*PI;
			}
			return d;
		}
		// ---------------------------------------------------------------------
		const shawn::Vec
			GeoRouting::
			midpoint(const shawn::Vec& u, const shawn::Vec& v)
			throw()
		{
			Vec d = u - v;
			return (d * 0.5) + v;
		}
		// ---------------------------------------------------------------------
		bool
			GeoRouting::
			receive_message_for_me( shawn::Node& owner, const GeoRoutingGreedyMessage& gm )
			throw()
		{
			// IF designated receiver set ...
			if( gm.destination_address().designated_receiver() != NULL &&
				gm.destination_address().designated_receiver() == &owner )
			{
				INFORM_ROUTING_OBSERVERS( receive_geo_routing_message_for_me( *this, owner, gm ) );
				owner.receive( gm.application_message() );
				return true;
			}
			// ... ELSE addressing only by position
			double own_distance_to_center =
				(owner.real_position() - gm.destination_address().destination_position()).euclidean_norm();
			// Am I inside the circle?
			if( own_distance_to_center <= gm.destination_address().radius() )
			{
				const GeoRoutingNodeInfo& rni = node_info(owner);
				const GeoRoutingNeighborhood& grn = rni.geo_routing_neighborhood();
				double min_distance_to_center = DBL_MAX;
				for( GeoRoutingNeighborhoodConstIterator it = grn.begin_neighborhood(); it != grn.end_neighborhood(); ++it)
				{
					double neighbor_distance_to_center =
						(it->second.position() - gm.destination_address().destination_position()).euclidean_norm();
					min_distance_to_center = neighbor_distance_to_center < min_distance_to_center ?
											 neighbor_distance_to_center : min_distance_to_center;
				}
				if( own_distance_to_center <= min_distance_to_center )
				{
					INFORM_ROUTING_OBSERVERS( receive_geo_routing_message_for_me( *this, owner, gm ) );
					owner.receive( gm.application_message() );
					return true;
				}
				return false;
			}
			return false;
		}
		// ---------------------------------------------------------------------
		void
			GeoRouting::
			send_routing_message( shawn::Node& owner, const shawn::MessageHandle& mh )
			throw()
		{
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			double now = owner.current_time();
			EventScheduler& es = owner.world_w().scheduler_w();

			// Reschedule beacon timer while sending
			bool delete_successful = delete_beacon_timer( owner );
			assert( delete_successful );
			RoutingEventTag* et = new RoutingEventTag(owner);
			EventScheduler::EventHandle eh = es.new_event( *this, now + urv_ * beacon_interval_, et );
			rni.beacon_timer( eh );

			owner.send( mh );
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			geo_routing_neighborhood_update( const shawn::Node& owner )
			throw()
		{
			double now = owner.current_time();
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			bool neighborhood_changed = false;
			for( GeoRoutingNeighborhoodIterator it = rni.geo_routing_neighborhood_w().begin_neighborhood_w();
				 it != rni.geo_routing_neighborhood_w().end_neighborhood_w(); )
			{
				if( now - it->second.last_heard_time() > beacon_expiration_interval_ )
				{
					rni.geo_routing_neighborhood_w().neighborhood_erase(it++);
					neighborhood_changed = true;
				}
				else
				{
					++it;
				}
			}
			return neighborhood_changed;
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			geo_routing_neighborhood_update( const shawn::Node& owner,
											 const shawn::Node& neighbor,
											 const GeoRoutingNeighborhoodInfo& info )
			throw()
		{
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			bool neighborhood_changed = false;

			GeoRoutingNeighborhood& neighborhood = rni.geo_routing_neighborhood_w();
			GeoRoutingNeighborhoodConstIterator grn_it = neighborhood.find_neighborhood(neighbor);

			if( grn_it != neighborhood.end_neighborhood() )
			{
				if( grn_it->second.position() != info.position() )
				{
					// Neighbors position changed
					neighborhood_changed = true;
				}
			}
			else
			{
				// New neighbor
				neighborhood_changed = true;
			}
			neighborhood.neighborhood_insert(neighbor,info);
			// Check also for outdated neighbors
			return neighborhood_changed || geo_routing_neighborhood_update(owner);
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			delete_beacon_timer( shawn::Node& owner )
			throw()
		{
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			if( rni.beacon_timer() != NULL )
			{
				EventScheduler& es = owner.world_w().scheduler_w();
				EventScheduler::EventHandle eh = rni.beacon_timer_w();
				es.delete_event( eh );
				rni.beacon_timer( NULL );
				return true;
			}
			return false;
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			delete_all_beacon_timer( shawn::SimulationController& sc )
			throw()
		{
			bool ret = true;
			for( World::node_iterator it = sc.world_w().begin_nodes_w();
				 it != sc.world_w().end_nodes_w(); ++it )
			{
				ret &= delete_beacon_timer(*it);
			}
			return ret;
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			start_beacon_timer( shawn::Node& owner )
			throw()
		{
			double now = owner.current_time();
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			/*rni.last_position( owner.real_position() );*/

			GeoRoutingProcessor* grp = owner.get_processor_of_type_w<GeoRoutingProcessor>();
			if( grp == NULL || rni.beacon_timer() != NULL )
			{
				// No GeoRouting node OR already started
				return false;
			}

			EventScheduler& es = owner.world_w().scheduler_w();
			RoutingEventTag* et = new RoutingEventTag(owner);

			EventScheduler::EventHandle eh = es.new_event( *this, now + urv_ * beacon_interval_, et );
			rni.beacon_timer( eh );
			return true;
		}
		// ----------------------------------------------------------------------
		bool
			GeoRouting::
			start_all_beacon_timer( shawn::SimulationController& sc )
			throw()
		{
			bool ret = true;
			for( World::node_iterator it = sc.world_w().begin_nodes_w();
				 it != sc.world_w().end_nodes_w(); ++it )
			{
				ret &= start_beacon_timer(*it);
			}
			return ret;
		}
		// ----------------------------------------------------------------------
		void
			GeoRouting::
			blacklist_neighbor( const shawn::Node& owner, const shawn::Node& neighbor, bool blacklisted )
			throw()
		{
			GeoRoutingNodeInfo& rni = node_info_w(owner);
			GeoRoutingNeighborhoodIterator it = rni.geo_routing_neighborhood_w().find_neighborhood_w(neighbor);
			assert( it != rni.geo_routing_neighborhood_w().end_neighborhood_w() );
			if( it != rni.geo_routing_neighborhood_w().end_neighborhood_w() )
			{
				it->second.blacklisted(blacklisted);
			}
		}
	}
}
#endif

