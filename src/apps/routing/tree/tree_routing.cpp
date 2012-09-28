/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "tree_routing.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"

#include "apps/routing/tree/tree_routing.h"
#include "apps/routing/tree/tree_routing_factory.h"
#include "apps/routing/tree/tree_routing_processor.h"
#include "apps/routing/tree/tree_routing_message.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace tree
	{
		// TreeRoutingTableInfo:

		TreeRoutingTable::TreeRoutingTableInfo::
			TreeRoutingTableInfo( const shawn::Node& neighbor, int hops_to_sink, double sink_construction_time) :
		neighbor_(&neighbor),
		hops_to_sink_(hops_to_sink),
		sink_creation_time_(sink_construction_time)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableInfo::
			TreeRoutingTableInfo( const TreeRoutingTableInfo& other ) :
		neighbor_(other.neighbor_),
		hops_to_sink_(other.hops_to_sink_),
		sink_creation_time_(other.sink_creation_time_)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableInfo::
			~TreeRoutingTableInfo()
		{}
		// ----------------------------------------------------------------------
		const shawn::Node*
			TreeRoutingTable::TreeRoutingTableInfo::
			neighbor()
			const
			throw()
		{
			return neighbor_;
		}
		// ----------------------------------------------------------------------
		int
			TreeRoutingTable::TreeRoutingTableInfo::
			hops_to_sink() 
			const
			throw()
		{
			return hops_to_sink_;
		}
		// ----------------------------------------------------------------------
		double 
			TreeRoutingTable::TreeRoutingTableInfo::
			sink_creation_time()
			const
			throw()
		{
			return sink_creation_time_;
		}
		// ----------------------------------------------------------------------
		void
			TreeRoutingTable::TreeRoutingTableInfo::
			neighbor( const shawn::Node& n )
			throw()
		{
			neighbor_ = &n;
		}
		// ----------------------------------------------------------------------
		void
			TreeRoutingTable::TreeRoutingTableInfo::
			hops_to_sink( int h )
			throw()
		{
			hops_to_sink_ = h;
		}
		// ----------------------------------------------------------------------
		void 
			TreeRoutingTable::TreeRoutingTableInfo::
			sink_creation_time(double t) 
			throw()
		{
			sink_creation_time_ = t;
		}

		// TreeRoutingTable:

		TreeRoutingTable::
			TreeRoutingTable()
		{}
		// ----------------------------------------------------------------------
		TreeRoutingTable::
			TreeRoutingTable( const TreeRoutingTable& other ) :
		table_(other.table_)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingTable::
			~TreeRoutingTable()
		{}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapConstIterator
			TreeRoutingTable::
			begin_table() 
			const 
			throw()
		{
			return table_.begin();
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapConstIterator
			TreeRoutingTable::
			end_table() 
			const 
			throw()
		{
			return table_.end();
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapConstIterator 
			TreeRoutingTable::
			table_find( const shawn::Node& sink ) 
			const 
			throw()
		{
			return table_.find(&sink);
		}
		// ----------------------------------------------------------------------
		const TreeRoutingTable::TreeRoutingTableInfo*
			TreeRoutingTable::
			routing_table_info( const shawn::Node& sink ) 
			const 
			throw()
		{
			TreeRoutingTableMapConstIterator it = table_.find(&sink);
			const TreeRoutingTableInfo* trti = NULL;
			if( it != table_.end() )
			{ 
				trti = &it->second;
			}
			return trti;
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapIterator
			TreeRoutingTable::
			begin_table_w()  
			throw()
		{
			return table_.begin();
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapIterator
			TreeRoutingTable::
			end_table_w() 
			throw()
		{
			return table_.end();
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableMapIterator 
			TreeRoutingTable::
			table_find_w( const shawn::Node& sink ) 
			throw()
		{
			return table_.find(&sink);
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable::TreeRoutingTableInfo*
			TreeRoutingTable::
			routing_table_info_w( const shawn::Node& sink ) 
			throw()
		{
			TreeRoutingTableMapIterator it = table_.find(&sink);
			TreeRoutingTableInfo* trti = NULL;
			if( it != table_.end() )
			{ 
				trti = &it->second;
			}
			return trti;
		}
		// ----------------------------------------------------------------------
		void 
			TreeRoutingTable::
			table_insert( const shawn::Node& sink, const TreeRoutingTableInfo& info ) 
			throw()
		{
			TreeRoutingTableMapIterator it = table_.find(&sink);
			if( it == table_.end() )
			{
				table_.insert( TreeRoutingTableMap::value_type( &sink, info ) );
			}
			else
			{
				it->second.neighbor( *info.neighbor() );
				it->second.hops_to_sink( info.hops_to_sink() );
				it->second.sink_creation_time( info.sink_creation_time() );
			}
		}
		// ----------------------------------------------------------------------
		bool 
			TreeRoutingTable::
			table_erase( const shawn::Node& sink ) 
			throw()
		{
			TreeRoutingTableMapIterator it = table_.find(&sink);
			if( it != table_.end() )
			{
				table_.erase(it);
				return true;
			}
			return false;
		}
		// ----------------------------------------------------------------------
		void 
			TreeRoutingTable::
			table_erase( TreeRoutingTableMapIterator it ) 
			throw()
		{
			table_.erase(it);
		}

		// TreeRoutingNodeInfo:

		TreeRoutingNodeInfo::
			TreeRoutingNodeInfo() :
		tree_construction_timer_(NULL)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingNodeInfo::
			TreeRoutingNodeInfo( const TreeRoutingNodeInfo& other ) :
		routing_table_(other.routing_table_),
		tree_construction_timer_(other.tree_construction_timer_),
		tree_construction_sending_jitter_timer_(other.tree_construction_sending_jitter_timer_)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingNodeInfo::
			~TreeRoutingNodeInfo()
		{}
		// ----------------------------------------------------------------------
		const TreeRoutingTable& 
			TreeRoutingNodeInfo::
			tree_routing_table() 
			const
			throw()
		{
			return routing_table_;
		}
		// ----------------------------------------------------------------------
		TreeRoutingTable& 
			TreeRoutingNodeInfo::
			tree_routing_table_w() 
			throw()
		{
			return routing_table_;
		}
		// ----------------------------------------------------------------------
		const shawn::EventScheduler::EventHandle 
			TreeRoutingNodeInfo::
			tree_construction_timer() 
			const 
			throw()
		{
			return tree_construction_timer_;
		}
		// ----------------------------------------------------------------------
		shawn::EventScheduler::EventHandle 
			TreeRoutingNodeInfo::
			tree_construction_timer_w() 
			throw()
		{
			return tree_construction_timer_;
		}
		// ----------------------------------------------------------------------
		void 
			TreeRoutingNodeInfo::
			tree_construction_timer( shawn::EventScheduler::EventHandle eh ) 
			throw()
		{
			tree_construction_timer_ = eh;
		}
		// ----------------------------------------------------------------------
		const TreeRoutingNodeInfo::TreeRoutingEventHandleSet& 
			TreeRoutingNodeInfo::
			tree_construction_sending_jitter_timer() 
			const 
			throw()
		{
			return tree_construction_sending_jitter_timer_;
		}
		// ----------------------------------------------------------------------
		TreeRoutingNodeInfo::TreeRoutingEventHandleSet& 
			TreeRoutingNodeInfo::
			tree_construction_sending_jitter_timer_w()  
			throw()
		{
			return tree_construction_sending_jitter_timer_;
		}
		

		// TreeRouting:

		TreeRouting::
			TreeRouting( const std::string& name, 
						 double tree_construction_interval,
						 double tree_expiration_interval,
						 int tree_routing_message_time_to_live, 
						 int tree_max_hops,
						 double tree_construction_sending_jitter_lower_bound,
						 double tree_construction_sending_jitter_upper_bound ) : 
		name_(name),
		tree_construction_interval_(tree_construction_interval),
		tree_expiration_interval_(tree_expiration_interval),
		tree_routing_message_time_to_live_(tree_routing_message_time_to_live),
		tree_max_hops_(tree_max_hops)
		{
			urv_.set_lower_bound(tree_construction_sending_jitter_lower_bound);
			urv_.set_upper_bound(tree_construction_sending_jitter_upper_bound);
			urv_.set_lower_bound_inclusive(true);
			urv_.set_upper_bound_inclusive(true);
			urv_.init();
		}
		//-----------------------------------------------------------------------
		TreeRouting::
			~TreeRouting()
		{}
		//-----------------------------------------------------------------------
		bool 
			TreeRouting::
			send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const shawn::Node* const& receiver ) 
		{
			TreeRoutingProcessor* trp = sender.get_processor_of_type_w<TreeRoutingProcessor>();
			if( !receiver || trp == NULL )
			{
				return false;
			}
			// Get the routing table entry
			tree_routing_table_update( sender );
			const TreeRoutingNodeInfo& rni = node_info(sender);
			const TreeRoutingTableInfo* trti = rni.tree_routing_table().routing_table_info( *receiver );
			if( trti )
			{
				// Create message
				TreeRoutingMessage* ntrm = new TreeRoutingMessage( *this, mh, *trti->neighbor()/*next hop*/, 
															       *receiver, tree_routing_message_time_to_live_ );
				// Send message
				INFORM_ROUTING_OBSERVERS( initial_send_tree_routing_message(*this,sender,*ntrm) );
				INFORM_ROUTING_OBSERVERS( send_tree_routing_message(*this,sender,*ntrm) );
				sender.send( ntrm );
				return true;
			}
			INFORM_ROUTING_OBSERVERS( insufficient_routing_information_while_initial_sending(*this,sender,*receiver,mh) );
			return false;
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			create_sink( shawn::Node& sender )
			throw()
		{
			TreeRoutingProcessor* trp = sender.get_processor_of_type_w<TreeRoutingProcessor>();
			if( trp == NULL )
			{
				// No TreeRouting node
				return;
			}
			tree_routing_table_update(sender, sender, TreeRoutingTableInfo( sender, 0, sender.current_time() ) );

			TreeConstructionMessage* ntcm = new TreeConstructionMessage( *this, sender, sender.current_time() );
			INFORM_ROUTING_OBSERVERS( send_tree_construction_message(*this,sender,*ntcm) );

			double now = sender.current_time();
			TreeRoutingNodeInfo& rni = node_info_w(sender);
			EventScheduler& es = sender.world_w().scheduler_w();

			RoutingEventTag* et = new RoutingEventTag(sender);
			EventScheduler::EventHandle eh = es.new_event( *this, now + tree_construction_interval_, et );

			rni.tree_construction_timer( eh );

			sender.send( ntcm );
		}
		//-----------------------------------------------------------------------
		int 
			TreeRouting::
			tree_max_hops() 
			const 
			throw()
		{
			return tree_max_hops_;
		}
		//-----------------------------------------------------------------------
		std::string 
			TreeRouting::
			name() 
			const 
			throw()
		{
			return name_;
		}
		//-----------------------------------------------------------------------
		std::string 
			TreeRouting::
			description() 
			const 
			throw()
		{
			return "This is a decentralized tree routing algorithm. It's associated processor is '"
				+ routing::tree::TreeRoutingProcessorFactory::PROCESSOR_NAME + "'.";
		}
		//-----------------------------------------------------------------------
		int 
			TreeRouting::
			tree_routing_message_time_to_live() 
			const 
			throw()
		{
			return tree_routing_message_time_to_live_;
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			tree_routing_message_time_to_live(int i) 
			throw()
		{
			tree_routing_message_time_to_live_ = i;
		}
		//-----------------------------------------------------------------------
		bool 
			TreeRouting::
			sink_reachable( const shawn::Node& source_node, const shawn::Node& sink_node ) 
			const 
			throw()
		{
			const TreeRoutingNodeInfo& rni = node_info(source_node);
			const TreeRoutingTableInfo* trti = rni.tree_routing_table().routing_table_info( sink_node );
			return trti != NULL;
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, 
					 double time, shawn::EventScheduler::EventTagHandle& eth ) 
			throw()
		{
			RoutingEventTag* et = static_cast<RoutingEventTag*>( eth.get() );
			Node& owner = et->node_w();
			TreeRoutingNodeInfo& rni = node_info_w(owner);

			if( eh == rni.tree_construction_timer() )
			{
				TreeConstructionMessage* ntcm = new TreeConstructionMessage(*this,owner,time);
				tree_routing_table_update( owner, owner, TreeRoutingTableInfo( owner ,0 ,time ) );
				INFORM_ROUTING_OBSERVERS( send_tree_construction_message(*this,owner,*ntcm) );

				RoutingEventTag* net = new RoutingEventTag( *et );
				EventScheduler::EventHandle neh = es.new_event( *this, time + tree_construction_interval_, net );
				rni.tree_construction_timer( neh );

				owner.send( ntcm );
				return;
			}
			if( eh == find_tree_construction_sending_jitter_timer( owner, eh ) )
			{
				// Shrink the jitter timer set
				bool erased = static_cast<bool>( rni.tree_construction_sending_jitter_timer_w().erase( eh ) );
				assert( erased );
				// Fetch the message ...
				TreeConstructionMessage* ntcm = static_cast<TreeConstructionMessage*>( et->message_handle().get() );
				// ... and send it!
				INFORM_ROUTING_OBSERVERS( send_tree_construction_message(*this,owner,*ntcm) );
				owner.send( ntcm );
			}
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			observers_added(TreeRoutingObserver& obs) 
			throw()
		{
			// Ignore
		}
		//-----------------------------------------------------------------------
		bool 
			TreeRouting::
			process_tree_routing_message( shawn::Node& owner, const TreeRoutingMessage& trm ) 
			throw()
		{
			INFORM_ROUTING_OBSERVERS( receive_tree_routing_message(*this,owner,trm) );
			if( trm.next_hop() != &owner )
			{
				// Discard message because I'm not the next hop
				return true;
			}
			// Message for the sink
			if( trm.destination_address() == &owner )
			{
				INFORM_ROUTING_OBSERVERS( receive_tree_routing_message_for_me(*this,owner,trm) );
				owner.receive( trm.application_message() );
				return true;
			}
			// If I'm the next hop receiver, search the routing table for the sink
			const TreeRoutingNodeInfo& rni = node_info(owner);
			const TreeRoutingTableInfo* trti = rni.tree_routing_table().routing_table_info( *trm.destination_address() );
			assert( trti );
			if( trti )
			{
				// Get the next hop towards the sink, create new TreeRoutingMessage and resend it
				if( trm.time_to_live() > 0 )
				{
					TreeRoutingMessage* ntrm = new TreeRoutingMessage( trm, *trti->neighbor()/*next hop*/ );
					INFORM_ROUTING_OBSERVERS( send_tree_routing_message(*this,owner,*ntrm) );
					owner.send( ntrm );
				}
				else
				{
					INFORM_ROUTING_OBSERVERS( message_time_to_live_over(*this,owner,trm) );
				}
				return true;
			}
			return false;
		}
		//-----------------------------------------------------------------------
		bool 
			TreeRouting::
			process_tree_construction_message( shawn::Node& owner, const TreeConstructionMessage& tcm )
			throw()
		{
			double now = owner.current_time();
			TreeRoutingNodeInfo& rni = node_info_w(owner);

			INFORM_ROUTING_OBSERVERS( receive_tree_construction_message(*this,owner,tcm) );
			// Already in routing table?

			const TreeRoutingTableInfo* trti = rni.tree_routing_table().routing_table_info( *tcm.sink() );
			if( trti )
			{
				if( trti->hops_to_sink() <= tcm.hop_count() && 
					trti->sink_creation_time() >= tcm.sink_creation_time() )
				{
					// Message with greater or equal hop count to the sink AND
					// old TreeConstructionMessage
					return true;
				}
			}// Message with smaller hop count to the sink -> update entry! OR
			 // No routing table entry -> create one!
			// Method creates or updates a routing table entry
			tree_routing_table_update( owner, *tcm.sink(), TreeRoutingTableInfo( tcm.source(), tcm.hop_count(), tcm.sink_creation_time() ) );
			// Resend the TreeConstructionMessage
			if( tcm.hop_count() < tree_max_hops_ )
			{
				TreeConstructionMessage* ntcm = new TreeConstructionMessage( tcm );
				// Resend the message with a jitter
				TreeRoutingNodeInfo& rni = node_info_w(owner);
				EventScheduler& es = owner.world_w().scheduler_w();
				//TreeRoutingEventTag* et = new TreeRoutingEventTag(owner,ntcm);
				RoutingEventTag* et = new RoutingEventTag(owner,ntcm);
				EventScheduler::EventHandle eh = es.new_event(*this, now + urv_, et );
				rni.tree_construction_sending_jitter_timer_w().insert( eh );
			}
			return true;
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			boot( shawn::Node& owner ) 
			throw()
		{
			init( owner.world_w() );
		}
		//-----------------------------------------------------------------------
		void 
			TreeRouting::
			tree_routing_table_update( const shawn::Node& owner ) 
			throw()
		{
			double now = owner.current_time();
			TreeRoutingNodeInfo& rni = node_info_w(owner);

			for(TreeRoutingTableIterator it = rni.tree_routing_table_w().begin_table_w();
				it != rni.tree_routing_table_w().end_table_w(); )
			{
				if( now - it->second.sink_creation_time() > tree_expiration_interval_ )
				{
					rni.tree_routing_table_w().table_erase( it++ );
				}
				else
				{
					++it;
				}
			}
		}
		//-----------------------------------------------------------------------
		/// Inserts or updates an entry in a routing table
		void 
			TreeRouting::
			tree_routing_table_update( const shawn::Node& owner, const shawn::Node& sink, const TreeRoutingTableInfo& info ) 
			throw()
		{
			TreeRoutingNodeInfo& rni = node_info_w(owner);
			rni.tree_routing_table_w().table_insert( sink, info );
			tree_routing_table_update( owner );
		}
		// ----------------------------------------------------------------------
		const shawn::EventScheduler::EventHandle 
			TreeRouting::
			find_tree_construction_sending_jitter_timer( const shawn::Node& owner, const shawn::EventScheduler::EventHandle eh ) 
			const
			throw()
		{
			const TreeRoutingNodeInfo& rni = node_info(owner);
			TreeRoutingEventHandleSetConstIterator it = rni.tree_construction_sending_jitter_timer().find( eh );
			if( it != rni.tree_construction_sending_jitter_timer().end() )
			{
				return *it;
			}
			return NULL;
		}
		// ----------------------------------------------------------------------
		shawn::EventScheduler::EventHandle 
			TreeRouting::
			find_tree_construction_sending_jitter_timer_w( const shawn::Node& owner, shawn::EventScheduler::EventHandle eh ) 
			throw()
		{
			TreeRoutingNodeInfo& rni = node_info_w(owner);
			TreeRoutingEventHandleSetIterator it = rni.tree_construction_sending_jitter_timer_w().find( eh );
			if( it != rni.tree_construction_sending_jitter_timer_w().end() )
			{
				return *it;
			}
			return NULL;
		}
		// ----------------------------------------------------------------------
		bool 
			TreeRouting::
			delete_tree_construction_timer( shawn::Node& owner )
			throw()
		{
			TreeRoutingNodeInfo& rni = node_info_w(owner);
			if( rni.tree_construction_timer() != NULL )
			{
				EventScheduler& es = owner.world_w().scheduler_w();
				EventScheduler::EventHandle eh = rni.tree_construction_timer_w();
				es.delete_event( eh );
				rni.tree_construction_timer( NULL );
				return true;
			}
			return false;
		}
		// ----------------------------------------------------------------------
		void TreeRouting::
			delete_all_tree_construction_sending_jitter_timer( shawn::Node& owner ) 
			throw()
		{
			TreeRoutingNodeInfo& rni = node_info_w(owner);
			EventScheduler& es = owner.world_w().scheduler_w();
			for( TreeRoutingEventHandleSetIterator it = rni.tree_construction_sending_jitter_timer_w().begin();
				 it != rni.tree_construction_sending_jitter_timer_w().end(); ++it )
			{
				es.delete_event( *it );
			}
			rni.tree_construction_sending_jitter_timer_w().clear();
		}
	}
}
#endif
