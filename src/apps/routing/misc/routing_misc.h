/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
 
#ifndef __SHAWN_APPS_ROUTING_MISC_ROUTING_MISC_H
#define __SHAWN_APPS_ROUTING_MISC_ROUTING_MISC_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <limits.h>
#include <set>
#include <vector>

#include "sys/misc/dynamic_node_array.h"

namespace shawn{ class SimulationController;
				 class Node; }

namespace routing
{
	namespace misc
	{
		struct HopsToSourceResultInfo
		{
			friend struct hops_to_source;
			///
			typedef std::list<const shawn::Node*> SuccessorList;
			typedef std::list<const shawn::Node*>::iterator SuccessorListIterator;
			typedef std::list<const shawn::Node*>::const_iterator SuccessorListConstIterator;
			///
			HopsToSourceResultInfo();
			HopsToSourceResultInfo( const HopsToSourceResultInfo& other );
			virtual ~HopsToSourceResultInfo();
			const shawn::Node* predecessor() const throw();
			const SuccessorList& successors() const throw();
			int hops() const throw();
		protected:
			//
			const shawn::Node* predecessor_;
			SuccessorList successors_;
			int hops_;
		};// End of HopsToSourceResultInfo

		class HopsToSourceResult : public shawn::DynamicNodeArray<HopsToSourceResultInfo>
		{
		public:
			HopsToSourceResult( shawn::World& world ) :
			shawn::DynamicNodeArray<HopsToSourceResultInfo>( world )
			{}
			virtual ~HopsToSourceResult()
			{}
		};// End of HopsToSourceResult

		struct hops_to_source
		{
			void operator() ( const shawn::Node& source, 
							  HopsToSourceResult& result, 
							  const shawn::Node* target = NULL,
							  int max_hops = INT_MAX );
		protected:
			typedef std::multimap<int,const shawn::Node*> NodesToExamineMap;
			typedef std::multimap<int,const shawn::Node*>::iterator NodesToExamineMapIterator;
			typedef std::multimap<int,const shawn::Node*>::value_type NodesToExamineMapValueType;
		};// End of hops_to_source

		//

		struct GreedyPathInfo
		{
			friend struct greedy_path;
			//
			GreedyPathInfo();
			GreedyPathInfo( const GreedyPathInfo& other );
			virtual ~GreedyPathInfo();
			const shawn::Node* predecessor() const throw();
			int hops() const throw();
		protected:
			const shawn::Node* predecessor_;
			bool visited_;
			bool inserted_;
			int hops_;
		};// End of GreedyPathInfo

		class GreedyPathResult : public shawn::DynamicNodeArray<GreedyPathInfo>
		{
		public:
			GreedyPathResult( shawn::World& world ) :
			shawn::DynamicNodeArray<GreedyPathInfo>( world )
			{}
			virtual ~GreedyPathResult()
			{}
		};// End of GreedyPathResult

		struct greedy_path
		{
			bool operator() ( const shawn::Node& source, 
							  const shawn::Node& target, 
							  GreedyPathResult& result );
		protected:
			typedef std::multimap<double,const shawn::Node*> NodesToExamineMap;
			typedef std::multimap<double,const shawn::Node*>::iterator NodesToExamineMapIterator;
			typedef std::multimap<double,const shawn::Node*>::value_type NodesToExamineMapValueType;
		};// End of greedy_path

		class Partitions
		{
		public:
			Partitions();
			virtual ~Partitions();
			void init( shawn::SimulationController& sc ) throw();
			bool reachable(const shawn::Node& a, const shawn::Node& b) throw();
		private:
			std::vector<std::set<const shawn::Node*> > partitions_;

			typedef std::set<const shawn::Node*> NodesSet;
			typedef std::set<const shawn::Node*>::iterator NodesSetIterator;
		};
	}
}
#endif
#endif

