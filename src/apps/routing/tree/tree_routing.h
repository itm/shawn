/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_TREE_TREE_ROUTING_H
#define __SHAWN_APPS_ROUTING_TREE_TREE_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <map>

#include "sys/misc/observable.h"
#include "sys/misc/random/uniform_random_variable.h"

#include "apps/routing/routing.h"

namespace routing
{
	namespace tree
	{
		class TreeRouting;
		class TreeRoutingMessage;
		class TreeConstructionMessage;

		class TreeRoutingTable
		{
			friend class TreeRouting;
			friend class TreeRoutingNodeInfo;
		public:
			struct TreeRoutingTableInfo
			{
				friend class TreeRoutingTable;
				TreeRoutingTableInfo( const shawn::Node& neighbor, int hops_to_sink, double sink_construction_time );
				TreeRoutingTableInfo( const TreeRoutingTableInfo& other );
				virtual ~TreeRoutingTableInfo();
				const shawn::Node* neighbor() const throw();
				int hops_to_sink() const throw();
				double sink_creation_time() const throw();
			protected:
				void neighbor( const shawn::Node& n ) throw();
				void hops_to_sink( int h ) throw();
				void sink_creation_time(double t) throw();
				const shawn::Node* neighbor_;
				int hops_to_sink_;
				double sink_creation_time_;
			};// End of TreeRoutingTableInfo

			// Maps sink addresses to hops
			// <sink,TreeRoutingTableInfo>
			typedef std::map<const shawn::Node*,TreeRoutingTableInfo> TreeRoutingTableMap;
			typedef std::map<const shawn::Node*,TreeRoutingTableInfo>::iterator TreeRoutingTableMapIterator;
			typedef std::map<const shawn::Node*,TreeRoutingTableInfo>::const_iterator TreeRoutingTableMapConstIterator;
			
			TreeRoutingTable();
			TreeRoutingTable( const TreeRoutingTable& );
			virtual ~TreeRoutingTable();
			TreeRoutingTableMapConstIterator begin_table() const throw();
			TreeRoutingTableMapConstIterator end_table() const throw();
			TreeRoutingTableMapConstIterator table_find( const shawn::Node& sink ) const throw();
			const TreeRoutingTableInfo* routing_table_info( const shawn::Node& sink ) const throw();
		protected:
			TreeRoutingTableMapIterator begin_table_w() throw();
			TreeRoutingTableMapIterator end_table_w() throw();
			TreeRoutingTableMapIterator table_find_w( const shawn::Node& sink ) throw();
			TreeRoutingTableInfo* routing_table_info_w( const shawn::Node& sink ) throw();
			void table_insert( const shawn::Node& sink, const TreeRoutingTableInfo& info ) throw();
			bool table_erase( const shawn::Node& sink ) throw();
			void table_erase( const TreeRoutingTableMapIterator it ) throw();
			TreeRoutingTableMap table_;
		};// End of TreeRoutingTable

		class TreeRoutingNodeInfo
		{
			friend class Routing<const shawn::Node*,TreeRoutingNodeInfo>;
			friend class TreeRouting;
		public:
			typedef std::set<shawn::EventScheduler::EventHandle> TreeRoutingEventHandleSet;
			typedef std::set<shawn::EventScheduler::EventHandle>::iterator TreeRoutingEventHandleSetIterator;
			typedef std::set<shawn::EventScheduler::EventHandle>::const_iterator TreeRoutingEventHandleSetConstIterator;

			TreeRoutingNodeInfo();
			TreeRoutingNodeInfo( const TreeRoutingNodeInfo& );
			virtual ~TreeRoutingNodeInfo();

			const TreeRoutingTable& tree_routing_table() const throw();
			// For periodic tree construction
			const shawn::EventScheduler::EventHandle tree_construction_timer() const throw();
			shawn::EventScheduler::EventHandle tree_construction_timer_w() throw();
			// For jittering tree construction message forward
			const TreeRoutingEventHandleSet& tree_construction_sending_jitter_timer() const throw();
		protected:
			TreeRoutingTable& tree_routing_table_w() throw();
			void tree_construction_timer( shawn::EventScheduler::EventHandle eh ) throw();
			// For jittering tree construction message forward
			TreeRoutingEventHandleSet& tree_construction_sending_jitter_timer_w() throw();
			//
			TreeRoutingTable routing_table_;
			// Timer for periodic tree construction
			shawn::EventScheduler::EventHandle tree_construction_timer_;
			// Timers for jittering tree construction message forward
			TreeRoutingEventHandleSet tree_construction_sending_jitter_timer_;
		};// End of TreeRoutingProcessorMemberInfo

		typedef TreeRoutingNodeInfo::TreeRoutingEventHandleSet TreeRoutingEventHandleSet;
		typedef TreeRoutingNodeInfo::TreeRoutingEventHandleSetIterator TreeRoutingEventHandleSetIterator;
		typedef TreeRoutingNodeInfo::TreeRoutingEventHandleSetConstIterator TreeRoutingEventHandleSetConstIterator;
		
		typedef TreeRoutingTable::TreeRoutingTableMapIterator TreeRoutingTableIterator;
		typedef TreeRoutingTable::TreeRoutingTableMapConstIterator TreeRoutingTableConstIterator;
		typedef TreeRoutingTable::TreeRoutingTableInfo TreeRoutingTableInfo;

		struct TreeRoutingObserver
		{
			// Track TreeConstructionMessage
			virtual void receive_tree_construction_message( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeConstructionMessage& tcm ) throw() = 0;
			virtual void send_tree_construction_message( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeConstructionMessage& tcm ) throw() = 0;
			// Track TreeRoutingMessage
			virtual void receive_tree_routing_message( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeRoutingMessage& trm ) throw() = 0;
			virtual void receive_tree_routing_message_for_me( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeRoutingMessage& trm ) throw() = 0;
			virtual void send_tree_routing_message( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeRoutingMessage& ntrm ) throw() = 0;
			virtual void initial_send_tree_routing_message( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeRoutingMessage& ntrm ) throw() = 0;
			//
			virtual void insufficient_routing_information_while_initial_sending( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const shawn::Node& receiver, const shawn::ConstMessageHandle& application_message ) throw() = 0;
			virtual void message_time_to_live_over( const routing::tree::TreeRouting& routing_instance, const shawn::Node& owner, const routing::tree::TreeRoutingMessage& trm ) throw() = 0;
		};// End of TreeRoutingObserver

		class TreeRouting : public Routing<const shawn::Node*,TreeRoutingNodeInfo>,
							public shawn::EventScheduler::EventHandler,
							public shawn::Observable<TreeRoutingObserver>
		{
		public:
			friend class TreeCreationTask;
			TreeRouting( const std::string& name, 
						 double tree_construction_interval,
						 double tree_expiration_interval,
						 int tree_routing_message_time_to_live, 
						 int tree_max_hops,
						 double tree_construction_sending_jitter_lower_bound,
						 double tree_construction_sending_jitter_upper_bound );
			virtual ~TreeRouting();
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const shawn::Node* const& receiver );
			void create_sink( shawn::Node& sender ) throw();
			int tree_max_hops() const throw();
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			int tree_routing_message_time_to_live() const throw();
			void tree_routing_message_time_to_live(int i) throw();
			bool sink_reachable( const shawn::Node& source_node, const shawn::Node& sink_node ) const throw();
			virtual void timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
								  double time, shawn::EventScheduler::EventTagHandle& eth ) throw();
			/// To satisfy the Observable interface
			virtual void observers_added(TreeRoutingObserver& obs) throw();
			virtual void boot( shawn::Node& owner ) throw();
			virtual bool process_tree_routing_message( shawn::Node& owner, const TreeRoutingMessage& trm ) throw();
			virtual bool process_tree_construction_message( shawn::Node& owner, const TreeConstructionMessage& tcm ) throw();
			///
			const shawn::EventScheduler::EventHandle find_tree_construction_sending_jitter_timer( const shawn::Node& owner, const shawn::EventScheduler::EventHandle eh ) const throw();
			///
			bool delete_tree_construction_timer( shawn::Node& owner ) throw();
			void delete_all_tree_construction_sending_jitter_timer( shawn::Node& owner ) throw();
		protected:
			/// Inserts or updates an entry in a routing table
			void tree_routing_table_update( const shawn::Node& owner, const shawn::Node& sink, const TreeRoutingTableInfo& info) throw();
			void tree_routing_table_update( const shawn::Node& owner ) throw();
			///
			shawn::EventScheduler::EventHandle find_tree_construction_sending_jitter_timer_w( const shawn::Node& owner, shawn::EventScheduler::EventHandle eh ) throw();
		private:
			const std::string name_;
			const double tree_construction_interval_;
			const double tree_expiration_interval_;
			int tree_routing_message_time_to_live_;
			const int tree_max_hops_;
			shawn::UniformRandomVariable urv_;
		};// End of TreeRouting
	}
}

#endif
#endif
