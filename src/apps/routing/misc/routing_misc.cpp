/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "routing_misc.h"
#ifdef ENABLE_ROUTING

#include <algorithm>

#include "sys/world.h"
#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"

#include "apps/routing/misc/routing_misc.h"

using namespace shawn;

namespace routing
{
	namespace misc
	{
		// HopsToSourceResultInfo:

		HopsToSourceResultInfo::
			HopsToSourceResultInfo() :
		predecessor_(NULL),
		hops_(INT_MAX)
		{}
		// ----------------------------------------------------------------------
		HopsToSourceResultInfo::
			HopsToSourceResultInfo( const HopsToSourceResultInfo& other ) :
		predecessor_(other.predecessor_),
		successors_(other.successors_),
		hops_(other.hops_)
		{}
		// ----------------------------------------------------------------------
		HopsToSourceResultInfo::
			~HopsToSourceResultInfo()
		{}
		// ----------------------------------------------------------------------
		const shawn::Node*
			HopsToSourceResultInfo::
			predecessor()
			const
			throw()
		{
			return predecessor_;
		}
		// ----------------------------------------------------------------------
		const HopsToSourceResultInfo::SuccessorList&
			HopsToSourceResultInfo::
			successors()
			const
			throw()
		{
			return successors_;
		}
		// ----------------------------------------------------------------------
		int
			HopsToSourceResultInfo::
			hops()
			const
			throw()
		{
			return hops_;
		}


		// hops_to_source:

		void
			hops_to_source::
			operator() ( const shawn::Node& source,
						 HopsToSourceResult& result,
						 const shawn::Node* target,
						 int max_hops )
		{
			// Init stuff
			NodesToExamineMap nodes_to_examine;
			// Hops from source to source are 0
			result[source].hops_ = 0;
			// Start from source
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
						result[*adj_it].predecessor_ = min_node;
						result[*min_node].successors_.push_back( &(*adj_it) );
						if( hops < max_hops )
						{
							nodes_to_examine.insert( NodesToExamineMapValueType( hops, &(*adj_it) ) );
						}
						if( target && target == &(*adj_it) )
						{
							return;
						}
					}
				}
			}
		}// End of hops_to_source::operator()

		//

		// GreedyPathInfo:

		GreedyPathInfo::
			GreedyPathInfo() :
		predecessor_(NULL),
		visited_(false),
		inserted_(false),
		hops_(INT_MAX)
		{}
		// ----------------------------------------------------------------------
		GreedyPathInfo::
			GreedyPathInfo( const GreedyPathInfo& other ) :
		predecessor_(other.predecessor_),
		visited_(other.visited_),
		inserted_(other.inserted_),
		hops_(other.hops_)
		{}
		// ----------------------------------------------------------------------
		GreedyPathInfo::
			~GreedyPathInfo()
		{}
		// ----------------------------------------------------------------------
		const shawn::Node*
			GreedyPathInfo::
			predecessor()
			const
			throw()
		{
			return predecessor_;
		}
		// ----------------------------------------------------------------------
		int
			GreedyPathInfo::
			hops()
			const
			throw()
		{
			return hops_;
		}

		// greedy_path:

		bool
			greedy_path::
			operator() ( const shawn::Node& source,
						 const shawn::Node& target,
						 GreedyPathResult& result )
		{
			// Init stuff
			NodesToExamineMap nodes_to_examine;
			// Start from the source
			double dist = ( source.real_position() - target.real_position() ).euclidean_norm();
			nodes_to_examine.insert( NodesToExamineMapValueType( dist, &source ) );
			result[source].inserted_ = true;
			result[source].hops_ = 0;
			// Main loop
			while( ! nodes_to_examine.empty() )
			{
				NodesToExamineMapIterator min_it = nodes_to_examine.begin();
				const shawn::Node* min_node = min_it->second;
				nodes_to_examine.erase( min_it );
				if( result[*min_node].visited_ )
				{
					//std::cerr << "Node: " << min_node->id() << " already visited!" << std::endl;
					continue;
				}
				result[*min_node].visited_ = true;
				//std::cerr << "Explore node: " << min_node->id() << std::endl;
				//
				for( World::const_adjacency_iterator adj_it = min_node->begin_adjacent_nodes();
					 adj_it != min_node->end_adjacent_nodes(); ++adj_it )
				{
					if( &(*adj_it) == min_node )
					{
						continue;
					}
					if( result[*adj_it].inserted_ )
					{
						/*std::cerr << "adj_it->id(): " << adj_it->id() << std::endl;
						std::cerr << "result[*min_node].predecessor_: " << result[*min_node].predecessor_->id() << std::endl;
						std::cerr << "result[*adj_it].hops_: " << result[*adj_it].hops_ << std::endl;
						std::cerr << "result[*(result[*min_node].predecessor_)].hops_: " << result[*(result[*min_node].predecessor_)].hops_ << std::endl;*/
						assert( result[*min_node].predecessor_ );
						if( result[*adj_it].hops_ < result[*(result[*min_node].predecessor_)].hops_ )
						{
							result[*min_node].predecessor_ = &(*adj_it);
							result[*min_node].hops_ = result[*adj_it].hops_ + 1;
						}
						continue;
					}
					result[*adj_it].predecessor_ = min_node;
					result[*adj_it].inserted_ = true;
					int hops = result[*min_node].hops_ + 1;
					result[*adj_it].hops_ = hops;
					dist = ( adj_it->real_position() - target.real_position() ).euclidean_norm();
					nodes_to_examine.insert( NodesToExamineMapValueType( dist, &(*adj_it) ) );
					//std::cerr << "Node: " << adj_it->id() << " with dist: " << dist << " and hops:" << hops << " inserted!" << std::endl;
				}
				if( min_node == &target )
				{
					return true;
				}
			}
			return false;
		}//

		// Partitions:

		Partitions::
			Partitions()
		{}
		// ----------------------------------------------------------------------
		Partitions::
			~Partitions()
		{}
		// ----------------------------------------------------------------------
		void
			Partitions::
			init( shawn::SimulationController& sc )
			throw()
		{
			partitions_.clear();
			shawn::World& world = sc.world_w();
			for( shawn::World::const_node_iterator source_it = world.begin_nodes();
				 source_it != world.end_nodes(); ++source_it )
			{
				bool found = false;
				for( std::vector<std::set<const shawn::Node*> >::iterator partitions_it = partitions_.begin();
					 partitions_it != partitions_.end(); ++partitions_it )
				{
					if( partitions_it->find( &(*source_it) ) != partitions_it->end() )
					{
						found = true;
					}
				}
				if( found ) continue;
				NodesSet nodes_to_examine;
				nodes_to_examine.insert( &(*source_it) );
				NodesSet nodes;
				while( ! nodes_to_examine.empty() )
				{
					NodesSetIterator node_to_examine_it = nodes_to_examine.begin();
					const shawn::Node* examined_node = *node_to_examine_it;
					nodes_to_examine.erase( node_to_examine_it );
					//
					for( World::const_adjacency_iterator adj_it = examined_node->begin_adjacent_nodes();
						 adj_it != examined_node->end_adjacent_nodes(); ++adj_it )
					{
						if( nodes.find( &(*adj_it) ) == nodes.end() )
						{
							nodes.insert( &(*adj_it) );
							nodes_to_examine.insert( &(*adj_it) );
						}
					}
				}
				partitions_.push_back(nodes);
			}
		}
		// ----------------------------------------------------------------------
		bool
			Partitions::
			reachable(const shawn::Node& a, const shawn::Node& b)
			throw()
		{
			bool found_a = false;
			bool found_b = false;
			for( std::vector<std::set<const shawn::Node*> >::iterator partitions_it = partitions_.begin();
				 partitions_it != partitions_.end(); ++partitions_it )
			{
				if( partitions_it->find( &a ) != partitions_it->end() ) found_a = true;
				if( partitions_it->find( &b ) != partitions_it->end() ) found_b = true;
				if( found_a && !found_b ) return false;
				if( !found_a && found_b ) return false;
				if( found_a && found_b ) return true;
			}
			return true;
		}

	}
}
#endif

