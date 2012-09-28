/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "centralized_tree_routing.h"
#ifdef ENABLE_ROUTING

#include <fstream>
#include <iostream>
#include <sstream>

#include "sys/misc/random/basic_random.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/util/string_conv.h"
#include "sys/misc/tokenize.h"
#include "sys/world.h"
#include "sys/node.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/centralized_tree/centralized_tree_routing_factory.h"
#include "apps/routing/centralized_tree/centralized_tree_routing.h"

#include "apps/routing/tree/tree_creation_task.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace tree
	{
		// HopsToSinkResultInfo:

		CentralizedTreeRouting::HopsToSinkResultInfo::
			HopsToSinkResultInfo() :
		hops_(INT_MAX)
		{}
		// ----------------------------------------------------------------------
		CentralizedTreeRouting::HopsToSinkResultInfo::
			HopsToSinkResultInfo( const HopsToSinkResultInfo& other ) :
		hops_(other.hops_)
		{}
		// ----------------------------------------------------------------------
		CentralizedTreeRouting::HopsToSinkResultInfo::
			~HopsToSinkResultInfo()
		{}
		// ----------------------------------------------------------------------
		int 
			CentralizedTreeRouting::HopsToSinkResultInfo::
			hops() 
			const 
			throw()
		{
			return hops_;
		}

		// CentralizedTreeRouting:

		CentralizedTreeRouting::
			CentralizedTreeRouting( const std::string& name, 
									int tree_max_hops,
									double delay_per_hop,
									double link_fail_probability ) :
		name_(name),
		tree_max_hops_(tree_max_hops),
		delay_per_hop_(delay_per_hop),
		link_fail_probability_(link_fail_probability)
		{}
		// ----------------------------------------------------------------------
		CentralizedTreeRouting::
			~CentralizedTreeRouting()
		{
			for(CentralizedTreeRoutingTableMapIterator it = routing_table_.begin(); it != routing_table_.end(); ++it)
			{
				delete it->second;
			}
			routing_table_.clear();
		}
		// ----------------------------------------------------------------------
		bool 
			CentralizedTreeRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, shawn::Node* const& receiver )
		{
			EventScheduler& es = sender.world_w().scheduler_w();
			double now = es.current_time();
			EventScheduler::EventHandle eh = NULL;
			routing::RoutingEventTag* et = NULL;
			CentralizedTreeRoutingTableMapIterator it = routing_table_.find(receiver);
			if( it != routing_table_.end() )
			{
				HopsToSinkResult& result = *(it->second);
				int hops_to_sink = result[sender].hops();
				if( hops_to_sink < INT_MAX )
				{
					if( delivery_fail( hops_to_sink ) )
					{
						// The delivery fails
						INFORM_ROUTING_OBSERVERS( centralized_tree_transmission_delivery_fail(*this,sender,*receiver,mh) );
						return true;
					}
					et = new RoutingEventTag( mh, *receiver );
					eh = es.new_event( *this, now + (delay_per_hop_ * hops_to_sink), et );
					INFORM_ROUTING_OBSERVERS( schedule_centralized_tree_transmission(*this,sender,*receiver,mh) );
					delivery_timer_.insert( eh );
					return true;
				}
			}
			INFORM_ROUTING_OBSERVERS( insufficient_routing_information_while_initial_sending(*this,sender,*receiver,mh) );
			return false;
		}
		// ----------------------------------------------------------------------
		void 
			CentralizedTreeRouting::
			create_sink( shawn::Node& sender ) 
			throw()
		{
			HopsToSinkResult* result = new HopsToSinkResult( sender.world_w() );
			hops_to_sink( sender, *result, tree_max_hops_ );
			routing_table_.insert( CentralizedTreeRoutingTableMapValueType(&sender,result) );
		}
		// ----------------------------------------------------------------------
		int 
			CentralizedTreeRouting::
			tree_max_hops() 
			const 
			throw()
		{
			return tree_max_hops_;
		}
		// ----------------------------------------------------------------------
		std::string 
			CentralizedTreeRouting::
			name() 
			const 
			throw()
		{
			return name_;
		}
		// ----------------------------------------------------------------------
		std::string 
			CentralizedTreeRouting::
			description() 
			const 
			throw()
		{
			return "";
		}
		// ----------------------------------------------------------------------
		void CentralizedTreeRouting::
			timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
					 double time, shawn::EventScheduler::EventTagHandle& eth ) 
			throw()
		{
			RoutingEventTag* et = static_cast<RoutingEventTag*>( eth.get() );
			Node& receiver = et->node_w();

			CentralizedTreeRoutingEventHandleSetIterator it = delivery_timer_.find( eh );
			if( it != delivery_timer_.end() && eh == *it )
			{
				bool erased = static_cast<bool>( delivery_timer_.erase( eh ) );
				assert( erased );
				const Message* rm = et->const_message_handle().get();
				INFORM_ROUTING_OBSERVERS( deliver_centralized_tree_transmission(*this,receiver,rm) );
				receiver.receive( rm );
			}
		}
			
		void 
			CentralizedTreeRouting::
			observers_added(CentralizedTreeRoutingObserver& obs) 
			throw()
		{
			//
		}
		// ----------------------------------------------------------------------
		bool 
			CentralizedTreeRouting::
			delivery_fail( int hops ) 
			throw()
		{
			return link_fail_probability_ == 0.0 ? false : 
				   shawn::uniform_random_0i_1i() <= ( 1 - pow( (1 - link_fail_probability_) ,hops ) );
		}
		// ----------------------------------------------------------------------
		void 
			CentralizedTreeRouting::
			hops_to_sink( const shawn::Node& source, HopsToSinkResult& result, int max_hops ) 
			throw()
		{
			NodesToExamineMap nodes_to_examine;
			result[source].hops_ = 0;
			nodes_to_examine.insert( NodesToExamineMapValueType(0,&source) );
			// Main loop
			while( ! nodes_to_examine.empty() ) 
			{
				// Detach min_node
				NodesToExamineMapIterator min_it = nodes_to_examine.begin();
				const shawn::Node* min_node = min_it->second;
				nodes_to_examine.erase( min_it );
				// 
				for( World::const_adjacency_iterator adj_it = min_node->begin_adjacent_nodes(); 
					 adj_it != min_node->end_adjacent_nodes(); ++adj_it )
				{
					assert( result[*min_node].hops_ < INT_MAX ); 
					int hops = result[*min_node].hops_ + 1;
					if( hops < result[*adj_it].hops_ )
					{
						assert( result[*adj_it].hops_ == INT_MAX );
						result[*adj_it].hops_ = hops;
						if( hops < max_hops )
						{
							nodes_to_examine.insert( NodesToExamineMapValueType( hops, &(*adj_it) ) );
						}
					}
				}
			}
		}
		// ----------------------------------------------------------------------
		void 
			CentralizedTreeRouting::
			delete_delivery_timer( shawn::SimulationController& sc ) 
			throw()
		{
			EventScheduler& es = sc.world_w().scheduler_w();
			for( CentralizedTreeRoutingEventHandleSetIterator it = delivery_timer_.begin(); 
				 it!= delivery_timer_.end(); ++it)
			{
				es.delete_event( *it );
			}
			delivery_timer_.clear();
		}
		// ----------------------------------------------------------------------
		const CentralizedTreeRouting::CentralizedTreeRoutingTableMap& 
			CentralizedTreeRouting::
			routing_table() 
			const 
			throw()
		{
			return routing_table_;
		}
		// ----------------------------------------------------------------------
		CentralizedTreeRouting::CentralizedTreeRoutingTableMap& 
			CentralizedTreeRouting::
			routing_table_w() 
			throw()
		{
			return routing_table_;
		}

		// CentralizedTreeCreationTask:

		CentralizedTreeCreationTask::
			CentralizedTreeCreationTask()
		{}
		// ----------------------------------------------------------------------
		CentralizedTreeCreationTask::
			~CentralizedTreeCreationTask()
		{}
		// ----------------------------------------------------------------------
		std::string 
			CentralizedTreeCreationTask::
			name( void ) 
			const
			throw()
		{
			return "centralized_tree_creation";
		}
		// ----------------------------------------------------------------------
		std::string 
			CentralizedTreeCreationTask::
			description() 
			const 
			throw()
		{
			return "";
		}
		// ----------------------------------------------------------------------
		void 
			CentralizedTreeCreationTask::
			run( shawn::SimulationController& sc ) 
			throw( std::runtime_error )
		{
			require_world(sc);
			string centralized_tree_routing_instance = sc.environment().required_string_param("routing_instance");
			string filename = sc.environment().required_string_param("tree_creation_filename");
			RoutingBaseHandle rbh = routing::routing_keeper_w(sc).find_w(centralized_tree_routing_instance);
			CentralizedTreeRouting* routing_instance = dynamic_cast<CentralizedTreeRouting*>( rbh.get() );
			if( ! routing_instance )
			{
				ERROR(this->logger(),"The given routing is no CentralizedTreeRouting!");
				abort();
			}
			create_tree_from_file(sc,*routing_instance,filename);
		}
		// ----------------------------------------------------------------------
		void 
			CentralizedTreeCreationTask::
			create_tree_from_file( shawn::SimulationController& sc, 
								   routing::tree::CentralizedTreeRouting& routing_instance, 
								   const std::string& filename )
			throw()
		{
			ifstream file_in(filename.c_str(),ifstream::in);
			if(!file_in)
			{
				ERROR(this->logger(),"Unable to open file " + filename + "!");
				abort();
			}
			string buf;
			while( getline(file_in,buf) )
			{
				//cerr << buf << endl;
				StrTok tok(buf,"\t ");
				double line[6];
				int i = 0;
				for(StrTok::iterator it = tok.begin(); it != tok.end(); ++it)
				{
					if( i != 2 )
					{
						line[i++] = conv_string_to_double( *it );
					}
					else
					{
						++i;
					}
				}
				shawn::Node* node = sc.world_w().find_node_by_id_w( (int)line[0] );
				shawn::Node* sink = sc.world_w().find_node_by_id_w( (int)line[1] );
				int hops_to_sink = (int)line[3];
				double node_x = line[4];
				double node_y = line[5];
				double node_real_pos_x = conv_string_to_double(conv_double_to_string(node->real_position().x()));
				double node_real_pos_y = conv_string_to_double(conv_double_to_string(node->real_position().y()));
				if( !( node && sink ) )
				{
					ERROR(this->logger(),"Error while reading file!");
					abort();
				}
				if( node_x != node_real_pos_x ||
					node_y != node_real_pos_y )
				{
					ERROR(this->logger(),"Position mismatch! Read x: " 
						  + conv_double_to_string(node_x) 
						  + ", y: " + conv_double_to_string(node_y)
						  + " Real position x: " + conv_double_to_string(node_real_pos_x)
						  + ", y: " + conv_double_to_string(node_real_pos_y));
					abort();
				}
				CentralizedTreeRouting::CentralizedTreeRoutingTableMapIterator it = 
					routing_instance.routing_table_w().find(sink);
				if( it != routing_instance.routing_table_w().end() )
				{
					assert( it->second );
					CentralizedTreeRouting::HopsToSinkResult& result = *(it->second);
					result[*node].hops_ = hops_to_sink;
				}
				else
				{
					CentralizedTreeRouting::HopsToSinkResult& result = 
						*( new CentralizedTreeRouting::HopsToSinkResult( sc.world_w() ) );
					result[*node].hops_ = hops_to_sink;
					routing_instance.routing_table_w().
						insert( CentralizedTreeRouting::CentralizedTreeRoutingTableMapValueType(sink,&result) );
				}
			}
			INFO(this->logger(),"Reading from file: " + filename + " succeeded!");
		}
	}
}
#endif
