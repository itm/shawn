/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_CENTRALIZED_TREE_CENTRALIZED_TREE_ROUTING_H
#define __SHAWN_APPS_ROUTING_CENTRALIZED_TREE_CENTRALIZED_TREE_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <limits.h>
#include <map>

#include "sys/misc/observable.h"
#include "sys/misc/dynamic_node_array.h"

#include "apps/routing/routing.h"

namespace shawn{ 
	class Node; 
	class Message;
	typedef class shawn::RefcntPointer<const Message> ConstMessageHandle;
	typedef class shawn::RefcntPointer<Message> MessageHandle;
}

namespace routing
{
	namespace tree
	{
		class CentralizedTreeRouting;

		struct CentralizedTreeRoutingObserver
		{
			virtual void deliver_centralized_tree_transmission( const routing::tree::CentralizedTreeRouting& routing_instance, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
			virtual void schedule_centralized_tree_transmission( const routing::tree::CentralizedTreeRouting& routing_instance, const shawn::Node& sender, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
			virtual void centralized_tree_transmission_delivery_fail( const routing::tree::CentralizedTreeRouting& routing_instance, const shawn::Node& sender, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0;
			virtual void insufficient_routing_information_while_initial_sending( const routing::tree::CentralizedTreeRouting& routing_instance, const shawn::Node& sender, const shawn::Node& receiver, const shawn::ConstMessageHandle& mh ) throw() = 0; 
		};// End of CentralizedTreeRoutingObserver

		class CentralizedTreeRouting : public routing::Routing<shawn::Node*>,
									   public shawn::EventScheduler::EventHandler,
									   public shawn::Observable<CentralizedTreeRoutingObserver>
		{
		public:
			friend class CentralizedTreeCreationTask;

			struct HopsToSinkResultInfo
			{
				friend class CentralizedTreeRouting;
				friend class CentralizedTreeCreationTask;

				HopsToSinkResultInfo();
				HopsToSinkResultInfo( const HopsToSinkResultInfo& other );
				virtual ~HopsToSinkResultInfo();
				int hops() const throw();
			protected:
				int hops_;
			};// End of HopsToSinkResultInfo

			class HopsToSinkResult : public shawn::DynamicNodeArray<HopsToSinkResultInfo>
			{
			public:
				HopsToSinkResult( shawn::World& world ) :
				shawn::DynamicNodeArray<HopsToSinkResultInfo>( world )
				{}
				virtual ~HopsToSinkResult()
				{}
			};// End of HopsToSinkResult

			typedef std::set<shawn::EventScheduler::EventHandle> CentralizedTreeRoutingEventHandleSet;
			typedef std::set<shawn::EventScheduler::EventHandle>::iterator CentralizedTreeRoutingEventHandleSetIterator;
			typedef std::set<shawn::EventScheduler::EventHandle>::const_iterator CentralizedTreeRoutingEventHandleSetConstIterator;

			typedef std::map<const shawn::Node*,HopsToSinkResult*> CentralizedTreeRoutingTableMap;
			typedef std::map<const shawn::Node*,HopsToSinkResult*>::iterator CentralizedTreeRoutingTableMapIterator;
			typedef std::map<const shawn::Node*,HopsToSinkResult*>::const_iterator CentralizedTreeRoutingTableMapConstIterator;
			typedef std::map<const shawn::Node*,HopsToSinkResult*>::value_type CentralizedTreeRoutingTableMapValueType;

			CentralizedTreeRouting( const std::string& name, 
									int tree_max_hops,
									double delay_per_hop,
									double link_fail_probability );
			virtual ~CentralizedTreeRouting();
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, shawn::Node* const& receiver );
			void create_sink( shawn::Node& sender ) throw();
			int tree_max_hops() const throw();
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			virtual void timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
								  double time, shawn::EventScheduler::EventTagHandle& eth ) throw();
			virtual void observers_added(CentralizedTreeRoutingObserver& obs) throw();
			void delete_delivery_timer( shawn::SimulationController& sc ) throw(); 
			const CentralizedTreeRoutingTableMap& routing_table() const throw();
		protected:
			void hops_to_sink( const shawn::Node& source, HopsToSinkResult& result, int max_hops = INT_MAX ) throw();
			bool delivery_fail( int hops ) throw();
			CentralizedTreeRoutingTableMap& routing_table_w() throw();
			//
			const std::string name_;
			const int tree_max_hops_;
			const double delay_per_hop_;
			const double link_fail_probability_;
			//
			typedef std::multimap<int,const shawn::Node*> NodesToExamineMap;
			typedef std::multimap<int,const shawn::Node*>::iterator NodesToExamineMapIterator;
			typedef std::multimap<int,const shawn::Node*>::value_type NodesToExamineMapValueType;
			//
			CentralizedTreeRoutingTableMap routing_table_;
			CentralizedTreeRoutingEventHandleSet delivery_timer_;
		};// End of CentralizedTreeRouting

		class CentralizedTreeCreationTask :public shawn::SimulationTask
		{
		public:
			CentralizedTreeCreationTask();
			virtual ~CentralizedTreeCreationTask();
			virtual std::string name( void ) const throw();
			virtual std::string description() const throw();
			virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
		protected:
			virtual void create_tree_from_file( shawn::SimulationController& sc, routing::tree::CentralizedTreeRouting& routing_instance, const std::string& filename ) throw();
		};

		typedef CentralizedTreeRouting::CentralizedTreeRoutingTableMap CentralizedTreeRoutingTableMap;
		typedef CentralizedTreeRouting::CentralizedTreeRoutingTableMapIterator CentralizedTreeRoutingTableMapIterator;
		typedef CentralizedTreeRouting::CentralizedTreeRoutingTableMapConstIterator CentralizedTreeRoutingTableMapConstIterator;
	}
}

#endif
#endif
