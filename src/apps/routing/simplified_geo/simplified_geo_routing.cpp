/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "simplified_geo_routing.h"
#ifdef ENABLE_ROUTING

#include <float.h>
#include <algorithm> 

#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/communication_model.h"

#include "apps/routing/simplified_geo/simplified_geo_routing_factory.h"
#include "apps/routing/simplified_geo/simplified_geo_routing.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_processor.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_message.h"

using namespace std;
using namespace shawn;

const double routing::geo::SimplifiedGeoRouting::PI = 3.14159265;

namespace routing
{
	namespace geo
	{
		// SimplifiedGeoRouting:

		/*! \brief This implementation is based of Greedy Perimeter Stateless Routing by Brad Karp.
		 *         See: "GPSR: Greedy Perimeter Stateless Routing for Wireless Networks" OR
		 *		   "Geographic Routing for Wireless Networks"
		 */

		SimplifiedGeoRouting::
			SimplifiedGeoRouting( const string& name, 
								  int messages_time_to_live,
								  bool route_always_in_perimeter_mode ) : 
		name_(name),
		messages_time_to_live_(messages_time_to_live),
		route_always_in_perimeter_mode_(route_always_in_perimeter_mode)
		{}
		// ----------------------------------------------------------------------
		SimplifiedGeoRouting::
			~SimplifiedGeoRouting()
		{}
		//-----------------------------------------------------------------------
		bool 
			SimplifiedGeoRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const GeoRoutingAddress& destination_address )
		{
			SimplifiedGeoRoutingProcessor* sgrp = sender.get_processor_of_type_w<SimplifiedGeoRoutingProcessor>();
			if( sgrp == NULL )
			{
				// No SimplifiedGeoRouting node
				return false;
			}

			if( destination_address.designated_receiver() != NULL && 
				!partitions_.reachable( sender, *destination_address.designated_receiver() ) )
			{
				INFORM_ROUTING_OBSERVERS( destination_unreachable(*this,sender,*destination_address.designated_receiver(),mh) );
				return false;
			}

			SimplifiedGeoRoutingGreedyMessage gm = 
				SimplifiedGeoRoutingGreedyMessage(*this,mh,destination_address,messages_time_to_live_);
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
					SimplifiedGeoRoutingPerimeterMessage* npm = new SimplifiedGeoRoutingPerimeterMessage(gm,*right_hand_receiver,sender);
					INFORM_ROUTING_OBSERVERS( initial_send_geo_routing_message( *this, sender, *npm ) );
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message( *this, sender, *npm ) );
					sender.send( npm );
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
		void 
			SimplifiedGeoRouting::
			determine_partitions( shawn::SimulationController& sc ) 
			throw()
		{
			partitions_.init( sc );
		}
		// ----------------------------------------------------------------------
		std::string 
			SimplifiedGeoRouting::
			name() 
			const 
			throw()
		{
			return name_;
		}
		// ----------------------------------------------------------------------
		string 
			SimplifiedGeoRouting::
			description() 
			const 
			throw()
		{
			return "This is a decentralized geo routing algorithm. It's associated processor is '"
				+ routing::geo::SimplifiedGeoRoutingProcessorFactory::PROCESSOR_NAME + "'."; 
		}
		// ----------------------------------------------------------------------
		void 
			SimplifiedGeoRouting::
			messages_time_to_live(int t)
			throw()
		{
			messages_time_to_live_ = t;
		}
		// ----------------------------------------------------------------------
		int 
			SimplifiedGeoRouting:: 
			messages_time_to_live()
			const 
			throw()
		{
			return messages_time_to_live_;
		}
		// ----------------------------------------------------------------------
		void 
			SimplifiedGeoRouting::
			observers_added(SimplifiedGeoRoutingObserver& obs) 
			throw()
		{
			// Ignore
		}
		// ----------------------------------------------------------------------
		void 
			SimplifiedGeoRouting::
			boot( shawn::Node& owner ) 
			throw()
		{
			// 
		}
		// ---------------------------------------------------------------------
		bool 
			SimplifiedGeoRouting::
			process_greedy_message( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) 
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_geo_routing_greedy_message(*this,owner,gm) );
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
			/*if( neighborhood_update_ )
			{
				geo_routing_neighborhood_update(owner);
			}*/
			if( !greedy_forward_and_send(owner,gm) )
			{
				const Node* right_hand_receiver = perimeter_init_forward(owner,gm);
				if(right_hand_receiver)
				{
					SimplifiedGeoRoutingPerimeterMessage* npm = new SimplifiedGeoRoutingPerimeterMessage(gm,*right_hand_receiver,owner);
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message(*this,owner,*npm) );
					owner.send( npm );
				}
			}
			return true;
		}
		// ---------------------------------------------------------------------
		bool 
			SimplifiedGeoRouting::
			process_perimeter_message( shawn::Node& owner, const SimplifiedGeoRoutingPerimeterMessage& pm ) 
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_geo_routing_perimeter_message(*this,owner,pm) ); 
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
					greedy_forward_and_send(owner,static_cast<const SimplifiedGeoRoutingGreedyMessage&>( pm ));
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
					SimplifiedGeoRoutingPerimeterMessage* npm = new SimplifiedGeoRoutingPerimeterMessage( pm );
					right_hand_receiver = face_change( owner, *npm, *right_hand_receiver );
					assert( right_hand_receiver != &owner );
					INFORM_ROUTING_OBSERVERS( send_geo_routing_perimeter_message(*this,owner,*npm) );
					owner.send( npm );
				}
			}
			return true;
		}
		// ---------------------------------------------------------------------
		const shawn::Node* 
			SimplifiedGeoRouting::
			face_change( const shawn::Node& owner, SimplifiedGeoRoutingPerimeterMessage& npm, const shawn::Node& out_node ) 
			throw()
		{
			Vec intercept;
			bool i = false;
			
			Vec out_node_pos = out_node.real_position();
			i = intersect( out_node_pos, owner.real_position(),
						   npm.location_entering_perimeter_mode(),
						   npm.destination_address().destination_position(),
						   intercept );

			const Node* right_hand_receiver = &out_node;
			if( i )
			{
				if( (intercept - npm.destination_address().destination_position()).euclidean_norm() < 
					(npm.location_entering_current_face() - npm.destination_address().destination_position()).euclidean_norm() )
				{
					npm.location_entering_current_face(intercept);
					//const shawn::Vec& pos = owner.real_position();
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
			SimplifiedGeoRouting::
			perimeter_init_forward( const shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) 
			throw()
		{
			set<const Node*> blacklisted_neighbors;
			update_comm_graph_gg(owner,blacklisted_neighbors);

			const Node* a_min = NULL;
			double b_in = norm( atan2(owner.real_position().y() - gm.destination_address().destination_position().y(), 
									  owner.real_position().x() - gm.destination_address().destination_position().x()) );
			double delta_min = 3*PI;

			for( World::const_adjacency_iterator it = owner.begin_adjacent_nodes();
				 it != owner.end_adjacent_nodes(); ++it )
			{
				if( &(*it) == &owner )
				{
					continue;
				}
				set<const Node*>::const_iterator blacklisted_it = blacklisted_neighbors.find( &(*it) );
				if( blacklisted_it == blacklisted_neighbors.end() /* ! blacklisted */ )
				{
					double b_a = norm( atan2(owner.real_position().y() - it->real_position().y(),
											 owner.real_position().x() - it->real_position().x()) );
					Vec a_min_pos;
					double delta_b = norm( b_a - b_in );
					if( delta_b < delta_min )
					{
						delta_min = delta_b;
						a_min = &(*it);
					}
					else if( delta_b == delta_min )
					{
						assert( a_min );
						a_min = (it->real_position() - owner.real_position()).euclidean_norm() < 
								(a_min_pos - owner.real_position()).euclidean_norm() ? 
								&(*it) : a_min;
					}
				}
			}
			return a_min;
		}
		// ---------------------------------------------------------------------
		const shawn::Node* 
			SimplifiedGeoRouting::
			right_hand_forward( const shawn::Node& owner, const shawn::Node& in_node ) 
			throw()
		{
			set<const Node*> blacklisted_neighbors;
			update_comm_graph_gg(owner,blacklisted_neighbors);

			const Node* a_min = &in_node;
			double b_in = 0.0;

			Vec in_node_pos = in_node.real_position();
			b_in = norm( atan2(owner.real_position().y() - in_node_pos.y(),
							   owner.real_position().x() - in_node_pos.x()) );
			
			double delta_min = 3*PI;
			for( World::const_adjacency_iterator it = owner.begin_adjacent_nodes();
				 it != owner.end_adjacent_nodes(); ++it )
			{
				set<const Node*>::const_iterator blacklisted_it = blacklisted_neighbors.find( &(*it) );
				if( &(*it) == &owner || 
					&(*it) == &in_node || 
					blacklisted_it != blacklisted_neighbors.end() /* blacklisted */ )
				{
					continue;
				}
				double b_a = norm( atan2(owner.real_position().y() - it->real_position().y(),
										 owner.real_position().x() - it->real_position().x()) );
				Vec a_min_pos;
				double delta_b = norm( b_a - b_in );
				if( delta_b < delta_min )
				{
					delta_min = delta_b;
					a_min = &(*it);
					a_min_pos = it->real_position();
				}
				else if( delta_b == delta_min )
				{
					a_min = (it->real_position() - owner.real_position()).euclidean_norm() < 
							(a_min_pos - owner.real_position()).euclidean_norm() ? 
							&(*it) : a_min;
				}
			}
			return a_min;
		}
		// ---------------------------------------------------------------------
		bool 
			SimplifiedGeoRouting::
			greedy_forward_and_send( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm, bool initial_send ) 
			throw()
		{
			if( route_always_in_perimeter_mode_ )
			{
				return false;
			}

			const Node* n_best = &owner;
			double d_best = (owner.real_position() - gm.destination_address().destination_position()).euclidean_norm();
			
			for( World::const_adjacency_iterator it = owner.begin_adjacent_nodes(); 
				 it != owner.end_adjacent_nodes(); ++it )
			{
				if( &(*it) == &owner )
				{
					continue;
				}
				double d = (it->real_position() - gm.destination_address().destination_position()).euclidean_norm();

				if( d < d_best )
				{
					n_best = &(*it);
					d_best = d;
				}
			}
			if( n_best == &owner )
			{
				return false;
			}
			else
			{
				SimplifiedGeoRoutingGreedyMessage* ngm = new SimplifiedGeoRoutingGreedyMessage( gm, *n_best );
				if( initial_send )
				{
					INFORM_ROUTING_OBSERVERS( initial_send_geo_routing_message( *this, owner, *ngm ) );
				}
				INFORM_ROUTING_OBSERVERS( send_geo_routing_greedy_message( *this, owner, *ngm ) );
				owner.send( ngm );
				return true;
			}
			return false;
		}
		// ---------------------------------------------------------------------
		void 
			SimplifiedGeoRouting::
			update_comm_graph_gg( const shawn::Node& owner, std::set<const shawn::Node*>& blacklisted_neighbors ) 
			throw()
		{
			// Blacklist neighbors
			for(World::const_adjacency_iterator v_it = owner.begin_adjacent_nodes();
				v_it != owner.end_adjacent_nodes(); ++v_it)
			{
				if( &(*v_it) == &owner )
				{
					continue;
				}
				for(World::const_adjacency_iterator w_it = owner.begin_adjacent_nodes();
					w_it != owner.end_adjacent_nodes(); ++w_it)
				{
					if( &(*w_it) == &(*v_it) || &(*w_it) == &owner )
					{
						continue;
					}
					else
					{
						Vec m = midpoint( owner.real_position(), v_it->real_position() );
						if( (m - w_it->real_position()).euclidean_norm() < 
							(owner.real_position() - m).euclidean_norm() )
						{
							blacklisted_neighbors.insert( &(*v_it) );
							break;
						}
					}
				}
			}
		}
		// ---------------------------------------------------------------------
		bool
			SimplifiedGeoRouting::
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
			SimplifiedGeoRouting::
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
			SimplifiedGeoRouting::
			midpoint(const shawn::Vec& u, const shawn::Vec& v) 
			throw()
		{
			Vec d = u - v;
			return (d * 0.5) + v;
		}
		// ---------------------------------------------------------------------
		bool  
			SimplifiedGeoRouting::
			receive_message_for_me( shawn::Node& owner, const SimplifiedGeoRoutingGreedyMessage& gm ) 
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
				double min_distance_to_center = DBL_MAX;
				for( World::const_adjacency_iterator it = owner.begin_adjacent_nodes(); 
					 it != owner.end_adjacent_nodes(); ++it)
				{
					if( &(*it) == &owner )
					{
						continue;
					}
					double neighbor_distance_to_center = 
						(it->real_position() - gm.destination_address().destination_position()).euclidean_norm();
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
	}
}
#endif

