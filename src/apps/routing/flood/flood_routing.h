/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_H
#define __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <list>

#include "sys/misc/observable.h"
#include "sys/misc/random/uniform_random_variable.h"

#include "apps/routing/routing.h"


namespace routing
{
	namespace flood
	{
		class FloodRouting;
		class FloodRoutingMessage;

		//typedef routing::RoutingEventTag<FloodRoutingMessage> FloodRoutingEventTag;

		/// Keeps the application messages
		class FloodRoutingMessageHistory
		{
			friend class FloodRoutingNodeInfo;
			friend class FloodRouting;
		public:
			typedef std::list<shawn::ConstMessageHandle> MessageHistoryList;
			typedef std::list<shawn::ConstMessageHandle>::iterator MessageHistoryListIterator;
			typedef std::list<shawn::ConstMessageHandle>::const_iterator MessageHistoryListConstIterator;

			FloodRoutingMessageHistory();
			FloodRoutingMessageHistory( const FloodRoutingMessageHistory& );
			virtual ~FloodRoutingMessageHistory();

			virtual int history_size() const throw();

			virtual MessageHistoryListConstIterator find_history( const shawn::ConstMessageHandle& mh ) const throw();
			virtual MessageHistoryListConstIterator begin_history() const throw();
			virtual MessageHistoryListConstIterator end_history() const throw();
		protected:
			virtual MessageHistoryListIterator find_history_w( const shawn::ConstMessageHandle& mh ) throw();
			virtual MessageHistoryListIterator begin_history_w() throw();
			virtual MessageHistoryListIterator end_history_w() throw();

			virtual void history_push_back( const shawn::ConstMessageHandle& mh, int history_max_size ) throw();
		protected:
			MessageHistoryList history_;
		};// End of MessageHistory
		
		typedef FloodRoutingMessageHistory::MessageHistoryListIterator FloodRoutingMessageHistoryIterator; 
		typedef FloodRoutingMessageHistory::MessageHistoryListConstIterator FloodRoutingMessageHistoryConstIterator;

		class FloodRoutingNodeInfo
		{
			friend class Routing<int,FloodRoutingNodeInfo>;
			friend class FloodRouting;
		public:
			typedef std::set<shawn::EventScheduler::EventHandle> FloodRoutingEventHandleSet;
			typedef std::set<shawn::EventScheduler::EventHandle>::iterator FloodRoutingEventHandleSetIterator;
			typedef std::set<shawn::EventScheduler::EventHandle>::const_iterator FloodRoutingEventHandleSetConstIterator;

			FloodRoutingNodeInfo();
			FloodRoutingNodeInfo( const FloodRoutingNodeInfo& );
			virtual ~FloodRoutingNodeInfo();

			const FloodRoutingMessageHistory& message_history() const throw();
			const FloodRoutingEventHandleSet& sending_jitter_timer() const throw();
		protected:
			FloodRoutingMessageHistory& message_history_w() throw();
			FloodRoutingEventHandleSet& sending_jitter_timer_w() throw();
		private:
			FloodRoutingMessageHistory message_history_;
			FloodRoutingEventHandleSet sending_jitter_timer_;
		};// End of FloodRoutingNodeInfo

		typedef FloodRoutingNodeInfo::FloodRoutingEventHandleSet FloodRoutingEventHandleSet;
		typedef FloodRoutingNodeInfo::FloodRoutingEventHandleSetIterator FloodRoutingEventHandleSetIterator;
		typedef FloodRoutingNodeInfo::FloodRoutingEventHandleSetConstIterator FloodRoutingEventHandleSetConstIterator;

		struct FloodRoutingObserver
		{
			// Tracking FloodRoutingMessage
			virtual void receive_flood_routing_message( const routing::flood::FloodRouting& routing_instance, const shawn::Node& owner, const routing::flood::FloodRoutingMessage& frm ) throw() = 0;
			virtual void send_flood_routing_message( const routing::flood::FloodRouting& routing_instance, const shawn::Node& owner, const routing::flood::FloodRoutingMessage& nfrm ) throw() = 0;
			virtual void receive_flood_routing_message_for_me( const routing::flood::FloodRouting& routing_instance, const shawn::Node& owner, const routing::flood::FloodRoutingMessage& frm ) throw() = 0;
			virtual void initial_send_flood_routing_message( const routing::flood::FloodRouting& routing_instance, const shawn::Node& owner, const routing::flood::FloodRoutingMessage& nfrm ) throw() = 0;
		};// End of FloodRoutingObserver

		class FloodRouting : public Routing<int,FloodRoutingNodeInfo>,
							 public shawn::EventScheduler::EventHandler,
							 public shawn::Observable<FloodRoutingObserver>
		{
		public:
			FloodRouting( const std::string& name, 
						  int history_max_size, 
						  double probability,
						  double message_sending_jitter_lower_bound,
						  double message_sending_jitter_upper_bound );
			virtual ~FloodRouting();
			/// 
			virtual std::string name() const throw();
			///
			virtual std::string description() const throw();
			/// Satisfy the Observable interface
			virtual void observers_added( FloodRoutingObserver& obs ) throw();
			///
			virtual void timeout( shawn::EventScheduler& es, shawn::EventScheduler::EventHandle eh, double time, shawn::EventScheduler::EventTagHandle& eth ) throw();
			///
			virtual bool send_to( shawn::Node& sender, const shawn::ConstMessageHandle& mh, const int& hops );
			// Returns the possible maximum size of the history
			int history_max_size() const throw();
			/// Methods invoked by the FloodRoutingProcessors
			virtual void boot( shawn::Node& owner ) throw();
			virtual bool process_flood_routing_message( shawn::Node& owner, const FloodRoutingMessage& frm ) throw();
			/// Methods for convenient access to the "distributed data"
			bool message_in_history( const shawn::Node& owner, const shawn::ConstMessageHandle& mh ) const throw();
			const shawn::EventScheduler::EventHandle find_sending_jitter_timer( const shawn::Node& owner, const shawn::EventScheduler::EventHandle eh ) const throw();			
			void delete_all_sending_jitter_timer( shawn::Node& owner ) throw();
		protected:
		  virtual bool probabilistic_sending_decision() throw();
			void add_message_to_history( const shawn::Node& owner, const shawn::ConstMessageHandle& mh ) throw();
			shawn::EventScheduler::EventHandle find_sending_jitter_timer_w( const shawn::Node& owner, shawn::EventScheduler::EventHandle eh ) throw();

			FloodRouting( const FloodRouting& );
		private:
			const std::string name_;
			/*shawn::UniformRandomVariable urv_sending_decision_;*/
			shawn::UniformRandomVariable urv_;
			const int history_max_size_;
			const double probability_;
		};// End of FloodRouting
	}
}
#endif
#endif
