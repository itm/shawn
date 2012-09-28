/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_ROUTING_H
#define __SHAWN_APPS_ROUTING_ROUTING_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include <typeinfo>

#include "sys/misc/dynamic_node_array.h"
#include "sys/util/keeper_managed.h"
#include "sys/event_scheduler.h"
#include "sys/message.h"

namespace shawn{ class Node; class Processor; }

#define INFORM_ROUTING_OBSERVERS(m) for(ObserverIterator routing_observer_iterator = observers_begin(); \
										routing_observer_iterator != observers_end(); \
										++routing_observer_iterator) \
									{ \
										(**routing_observer_iterator).m; \
									}

namespace routing
{
	// RoutingBase:

	DECLARE_HANDLES(RoutingBase);
	class RoutingBase : public shawn::KeeperManaged
	{};// End of RoutingBase

	// RoutingMessage:

	class RoutingMessageBase : public shawn::Message
	{
	public:
		RoutingMessageBase( RoutingBase& ri, const shawn::ConstMessageHandle& mh ) :
			application_message_(mh),
			routing_instance_(&ri)
		{}
		// ----------------------------------------------------------------------
		RoutingMessageBase( const RoutingMessageBase& o ) :
			application_message_(o.application_message_),
			routing_instance_(o.routing_instance_)
		{}
		// ----------------------------------------------------------------------
		virtual ~RoutingMessageBase()
		{}
		// ----------------------------------------------------------------------
		inline const shawn::ConstMessageHandle&
			application_message()
			const
			throw()
		{ return application_message_; }
		// ----------------------------------------------------------------------
		inline const routing::RoutingBase*
			routing_instance()
			const
			throw()
		{ return routing_instance_; }
		// ----------------------------------------------------------------------
		inline routing::RoutingBase*
			routing_instance_w()
			const
			throw()
		{ return routing_instance_; }
		// ----------------------------------------------------------------------
		virtual int
			size( void )
			const
			throw()
		{ return application_message_->size() + 4; }

	protected:
		const shawn::ConstMessageHandle application_message_;
		RoutingBase* routing_instance_;
	};// End of RoutingMessageBase

	// NoNodeInfo:
	class NoNodeInfo
	{}; // End of NoNodeInfo

	// Routing:

	template<typename AbstractAddress,
			 typename RoutingNodeInfoTypeName = NoNodeInfo>
	class Routing : public routing::RoutingBase,
					public shawn::NodeChangeListener
	{
	public:
		/// Returns true or false whether it's possible in principle to send the message towards the address
		virtual bool send_to( shawn::Node& sender,
							  const shawn::ConstMessageHandle& mh,
							  const AbstractAddress& aa ) = 0;
		// RoutingMessage:

		class RoutingMessage : public routing::RoutingMessageBase
		{
		public:
			RoutingMessage( RoutingBase& ri, const shawn::ConstMessageHandle& mh, const AbstractAddress& aa ) :
				RoutingMessageBase(ri,mh),
				destination_address_(aa)
			{}
			// ----------------------------------------------------------------------
			RoutingMessage( const RoutingMessage& o ) :
				RoutingMessageBase(*o.routing_instance_,o.application_message_),
				destination_address_(o.destination_address_)
			{}
			// ----------------------------------------------------------------------
			virtual ~RoutingMessage()
			{}
			// ----------------------------------------------------------------------
			inline AbstractAddress
				destination_address()
				const
				throw()
			{
				return destination_address_;
			}
			// ----------------------------------------------------------------------
			virtual int
				size( void )
				const
				throw()
			{
				return sizeof(AbstractAddress) + RoutingMessageBase::size();
			}

		protected:
			AbstractAddress destination_address_;
		};// End of RoutingMessage

		// NodeInfo related:

		typedef RoutingNodeInfoTypeName RoutingNodeInfoType;

		Routing() :
		node_infos_(NULL),
		world_(NULL)
		{}
		// ----------------------------------------------------------------------
		virtual ~Routing()
		{
			if( world_ )
			{
				world_->remove_node_change_listener(*this);
			}
			if( node_infos_ )
			{
				delete node_infos_;
			}
		}
		// ----------------------------------------------------------------------
		virtual void node_added( shawn::Node& ) throw()
		{}
		// ----------------------------------------------------------------------
		virtual void node_removed( shawn::Node& ) throw()
		{}
		// ----------------------------------------------------------------------
		virtual void id_changed( int, int ) throw()
		{}
		// ----------------------------------------------------------------------
		/// If your derived routing will be also an NodeChangeListener
		/// overwrite the concerning methods AND ensure that
		/// Routing<YourRoutingAbstractAddress,YourRoutingNodeInfo>::invalidate
		/// is called in your YourRouting::invalidate.
		virtual bool
			invalidate( void )
			throw()
		{
			if( node_infos_ )
			{
				delete node_infos_;
				node_infos_ = NULL;
			}
			world_ = NULL;
			return false;
		}
		// ----------------------------------------------------------------------
		virtual void
			init( shawn::World& world )
			throw()
		{
			if( typeid(RoutingNodeInfoType) == typeid(NoNodeInfo) )
			{
				WARN(this->logger(),"init() while using NoNodeInfo is unnecessary!");
			}
			if( !world_ )
			{
				world_ = &world;
				world.add_node_change_listener( *this );
			}
			if( !node_infos_ )
			{
				node_infos_ = new shawn::DynamicNodeArray<RoutingNodeInfoType>( world );
			}
		}
		// ----------------------------------------------------------------------
		inline const RoutingNodeInfoType&
			node_info( const shawn::Node& owner )
			const
			throw()
		{
			assert( node_infos_ );
			return (*node_infos_)[owner];
		}
		// ----------------------------------------------------------------------
		inline RoutingNodeInfoType&
			node_info_w( const shawn::Node& owner )
			throw()
		{
			assert( node_infos_ );
			return (*node_infos_)[owner];
		}
		// ----------------------------------------------------------------------
		inline bool
			has_node_infos()
			const
			throw()
		{
			return node_infos_ != NULL;
		}

	private:
		shawn::DynamicNodeArray<RoutingNodeInfoType>* node_infos_;
		shawn::World* world_;

	};// End of Routing

	// RoutingEventTag:

	class RoutingEventTag : public shawn::EventScheduler::EventTag
	{
	public:
		RoutingEventTag( shawn::Node& node, const shawn::MessageHandle& mh = NULL ) :
		node_(&node),
		mh_(mh),
		cmh_(NULL)
		{}
		// ----------------------------------------------------------------------
		RoutingEventTag( const shawn::ConstMessageHandle& cmh, shawn::Node& node ) :
		node_(&node),
		mh_(NULL),
		cmh_(cmh)
		{}
		// ----------------------------------------------------------------------
		RoutingEventTag( const RoutingEventTag& other ) :
		node_(other.node_),
		mh_(other.mh_),
		cmh_(other.cmh_)
		{}
		// ----------------------------------------------------------------------
		virtual ~RoutingEventTag()
		{}
		// ----------------------------------------------------------------------
		inline const shawn::Node&
			node()
			const
			throw()
		{
			return *node_;
		}
		// ----------------------------------------------------------------------
		inline shawn::Node&
			node_w()
			throw()
		{
			return *node_;
		}
		// ----------------------------------------------------------------------
		inline const shawn::ConstMessageHandle&
			const_message_handle()
			const
			throw()
		{
			assert( cmh_.is_not_null() );
			return cmh_;
		}
		// ----------------------------------------------------------------------
		inline const shawn::MessageHandle&
			message_handle()
			throw()
		{
			assert( mh_.is_not_null() );
			return mh_;
		}

	private:
		shawn::Node* node_;
		const shawn::MessageHandle mh_;
		const shawn::ConstMessageHandle cmh_;
	};
}
#endif
#endif
