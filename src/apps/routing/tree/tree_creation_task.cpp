/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "tree_creation_task.h"
#ifdef ENABLE_ROUTING

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "sys/simulation/simulation_controller.h"
#include "sys/util/string_conv.h"
#include "sys/misc/tokenize.h"
#include "sys/world.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/tree/tree_creation_task.h"
#include "apps/routing/tree/tree_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace tree
	{
		// TreeCreationTask:

		TreeCreationTask::
			TreeCreationTask()
		{}
		// ----------------------------------------------------------------------
		TreeCreationTask::
			~TreeCreationTask()
		{}
		// ----------------------------------------------------------------------
		std::string
			TreeCreationTask::
			name( void )
			const
			throw()
		{
			return "tree_creation";
		}
		// ----------------------------------------------------------------------
		std::string
			TreeCreationTask::
			description( void )
			const
			throw()
		{
			return "This task creates a tree for a TreeRouting.\nIt can be run with the following configuration of parameters: \ntree_routing_instance and sink_id: creates a tree directly in the given tree routing instance. \nfilename and sink_id (and optional max_hops): is a dry run mode which stores a tree representation in a file. \ntree_routing_instance and filename: creates a tree directli in the given tree routing instance from file.";
		}
		// ----------------------------------------------------------------------
		void
			TreeCreationTask::
			run( shawn::SimulationController& sc )
			throw( std::runtime_error )
		{
			require_world( sc );
			bool tree_routing_set = false;
			bool filename_set = false;
			string tree_routing_instance = sc.environment().optional_string_param("routing_instance","",&tree_routing_set);
			string filename = sc.environment().optional_string_param("tree_creation_filename","",&filename_set);
			TreeRouting* routing_instance = NULL;

			if( tree_routing_set )
			{
				RoutingBaseHandle rbh = routing::routing_keeper_w(sc).find_w(tree_routing_instance);
				routing_instance = dynamic_cast<TreeRouting*>( rbh.get() );
				if( ! routing_instance )
				{
					ERROR(this->logger(),"The given routing is no TreeRouting!");
					abort();
				}
				routing_instance->init( sc.world_w() );
			}

			if( tree_routing_set && filename_set )
			{
				create_tree_from_file(sc,*routing_instance,filename);
				return;
			}
			int sink_id = sc.environment().required_int_param("sink_id");
			const Node* sink = sc.world().find_node_by_id( sink_id );

			if( tree_routing_set && ( ! filename_set ) )
			{
				create_tree_in_tree_routing( *routing_instance, *sink );
				return;
			}

			if( ( ! tree_routing_set ) && filename_set )
			{
				int max_hops = sc.environment().optional_int_param("max_hops",INT_MAX);
				create_tree_in_file( sc, filename, *sink, max_hops );
				return;
			}
		}
		// ----------------------------------------------------------------------
		void
			TreeCreationTask::
			create_tree_in_tree_routing( routing::tree::TreeRouting& routing_instance, const shawn::Node& sink )
			throw()
		{
			// Init stuff
			double now = sink.current_time();
			NodesToExamineMap nodes_to_examine;
			// Treat the sink
			routing_instance.tree_routing_table_update( sink,sink,TreeRoutingTableInfo(sink,0,now) );
			nodes_to_examine.insert( NodesToExamineMapValueType(0,&sink) );
			// Main loop
			while( ! nodes_to_examine.empty() )
			{
				// Fetch the min_node
				NodesToExamineMapIterator min_it = nodes_to_examine.begin();
				const shawn::Node* min_node = min_it->second;
				nodes_to_examine.erase( min_it );
				// Check the neighbors
				for( World::const_adjacency_iterator adj_it = min_node->begin_adjacent_nodes();
					 adj_it != min_node->end_adjacent_nodes(); ++adj_it )
				{
					const TreeRoutingNodeInfo& rni_min_node = routing_instance.node_info(*min_node);
					const TreeRoutingTableInfo* trti_min_node = rni_min_node.tree_routing_table().routing_table_info( sink );
					assert( trti_min_node );
					int hops = trti_min_node->hops_to_sink() + 1;
					// Fill the routing table
					const TreeRoutingNodeInfo& rni = routing_instance.node_info(*adj_it);
					const TreeRoutingTableInfo* trti = rni.tree_routing_table().routing_table_info( sink );
					if( !trti || hops < trti->hops_to_sink() )
					{
						routing_instance.tree_routing_table_update( *adj_it,sink,TreeRoutingTableInfo(*min_node,hops,now) );
						if( hops < routing_instance.tree_max_hops() )
						{
							nodes_to_examine.insert( NodesToExamineMapValueType( hops, &(*adj_it) ) );
						}
					}
				}
			}
		}
		// ----------------------------------------------------------------------
		void
			TreeCreationTask::
			create_tree_in_file( shawn::SimulationController& sc,
								 const std::string& filename,
								 const shawn::Node& sink,
								 int max_hops )
			throw()
		{
			// Avoid appending
			bool append = sc.environment().optional_bool_param("append",false);
			if(!append)
			{
				remove( filename.c_str() );
			}
			// Init stuff
//			double now = sink.current_time();
			NodesToExamineMap nodes_to_examine;
			TreeCreationHopsToSinkResult& result = *( new TreeCreationHopsToSinkResult( sc.world_w() ) );
			ofstream file_op(filename.c_str(),ios::app);
			// Treat the sink
			file_op << sink.id() << "\t"
					<< sink.id() << "\t"
					<< sink.id() << "\t"
					<< 0 << "\t"
					<< sink.real_position().x() << "\t"
					<< sink.real_position().y() << endl;
			result[sink].hops_ = 0;
			nodes_to_examine.insert( NodesToExamineMapValueType(0,&sink) );
			// Main loop
			while( ! nodes_to_examine.empty() )
			{
				NodesToExamineMapIterator min_it = nodes_to_examine.begin();
				const shawn::Node* min_node = min_it->second;
				nodes_to_examine.erase( min_it );
				for( World::const_adjacency_iterator adj_it = min_node->begin_adjacent_nodes();
					 adj_it != min_node->end_adjacent_nodes(); ++adj_it )
				{
					int hops = result[*min_node].hops_ + 1;
					if( hops < result[*adj_it].hops_ )
					{
						assert( result[*adj_it].hops_ == INT_MAX );
						result[*adj_it].hops_ = hops;
						file_op << adj_it->id() << "\t"
								<< sink.id() << "\t"
								<< min_node->id() << "\t"
								<< hops << "\t"
								<< adj_it->real_position().x() << "\t"
								<< adj_it->real_position().y() << endl;
						if( hops < max_hops )
						{
							nodes_to_examine.insert( NodesToExamineMapValueType( hops, &(*adj_it) ) );
						}
					}
				}
			}
			file_op.clear();
			file_op.close();
			delete &result;
		}
		// ----------------------------------------------------------------------
		void
			TreeCreationTask::
			create_tree_from_file( shawn::SimulationController& sc,
								   routing::tree::TreeRouting& routing_instance,
								   const std::string& filename )
			throw()
		{
			double now = sc.world().current_time();
			ifstream file_in;
			file_in.open(filename.c_str(),ifstream::in);
			if(!file_in)
			{
				ERROR(this->logger(),"Unable to open file " + filename + "!");
				abort();
			}
			string buf;
			while( getline(file_in,buf) )
			{
				StrTok tok(buf,"\t ");
				double line[6];
				int i = 0;
				for(StrTok::iterator it = tok.begin(); it != tok.end(); ++it)
				{
					line[i++] = conv_string_to_double(*it);
				}
				shawn::Node* node = sc.world_w().find_node_by_id_w( (int)line[0] );
				shawn::Node* sink = sc.world_w().find_node_by_id_w( (int)line[1] );
				shawn::Node* neighbor = sc.world_w().find_node_by_id_w( (int)line[2] );
				int hops_to_sink = (int)line[3];
				double node_x = line[4];
				double node_y = line[5];
				double node_real_pos_x = conv_string_to_double(conv_double_to_string(node->real_position().x()));
				double node_real_pos_y = conv_string_to_double(conv_double_to_string(node->real_position().y()));
				if( !( node && sink && neighbor ) )
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
				routing_instance.tree_routing_table_update(*node,*sink,TreeRoutingTableInfo(*neighbor,hops_to_sink,now));
			}
			INFO(this->logger(),"Reading from file: " + filename + " succeeded!");
		}
		// ----------------------------------------------------------------------
		std::string
			conv_double_to_string(double d)
			throw()
		{
			stringstream ss;
			ss << d;
			return ss.str();
		}
	}
}

#endif
