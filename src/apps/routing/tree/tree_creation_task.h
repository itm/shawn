/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_TREE_TREE_CREATION_TASK_H
#define __SHAWN_APPS_ROUTING_TREE_TREE_CREATION_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <limits.h>
#include <map>

#include "sys/simulation/simulation_task.h"
#include "sys/misc/dynamic_node_array.h"

namespace shawn{ class SimulationController; class Node; }

namespace routing
{
	namespace tree
	{
		class TreeRouting;

		class TreeCreationTask : public shawn::SimulationTask
		{
		public:
			typedef std::multimap<double,const shawn::Node*> NodesToExamineMap;
			typedef std::multimap<double,const shawn::Node*>::iterator NodesToExamineMapIterator;
			typedef std::multimap<double,const shawn::Node*>::value_type NodesToExamineMapValueType;

			struct TreeCreationHopsToSinkInfo
			{
				TreeCreationHopsToSinkInfo() : 
				hops_(INT_MAX)
				{}
				TreeCreationHopsToSinkInfo( const TreeCreationHopsToSinkInfo& other ) :
				hops_(other.hops_)
				{}
				virtual ~TreeCreationHopsToSinkInfo()
				{}
				int hops_;
			};

			class TreeCreationHopsToSinkResult : public shawn::DynamicNodeArray<TreeCreationHopsToSinkInfo>
			{
			public:
				TreeCreationHopsToSinkResult( shawn::World& world ) :
				shawn::DynamicNodeArray<TreeCreationHopsToSinkInfo>( world )
				{}
				virtual ~TreeCreationHopsToSinkResult()
				{}
			};

			TreeCreationTask();
			virtual ~TreeCreationTask();
			virtual std::string name( void ) const throw();
			virtual std::string description( void ) const throw();
			virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
		protected:
			virtual void create_tree_in_tree_routing( routing::tree::TreeRouting& routing_instance, const shawn::Node& sink ) throw();
			virtual void create_tree_in_file( shawn::SimulationController& sc, const std::string& filename, const shawn::Node& sink, int max_hops ) throw();
			virtual void create_tree_from_file( shawn::SimulationController& sc, routing::tree::TreeRouting& routint_instance, const std::string& filename ) throw();
		};
		std::string conv_double_to_string(double d) throw();
	}
}

#endif
#endif
