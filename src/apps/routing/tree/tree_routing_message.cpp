/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "tree_routing_message.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/tree/tree_routing_processor.h"
#include "apps/routing/tree/tree_routing_message.h"

using namespace shawn;

// 1 for message type
// 4 for next_hop_
// 4 for time_to_live_
const int routing::tree::TreeRoutingMessage::MESSAGE_OVERHEAD = 1 + 4 + 4;
const bool routing::tree::TreeRoutingMessage::USE_ROUTING_MESSAGE_OVERHEAD = true;
// 1 for message type
// 4 for Message::source()
// 4 for hop_count_
// 4 for construction_time_
// 4 for routing_instance_
const int routing::tree::TreeConstructionMessage::MESSAGE_OVERHEAD = 1 + 4 + 4 + 4 + 4;

namespace routing
{
	namespace tree
	{
		// TreeRoutingMessage:

		TreeRoutingMessage::
			TreeRoutingMessage( RoutingBase& routing_instance,
								const ConstMessageHandle& application_message,
								const Node& next_hop,
								const Node& sink,
								int time_to_live ) :
		RoutingMessage(routing_instance, application_message, &sink),
		next_hop_(&next_hop),
		time_to_live_(time_to_live > 0 ? time_to_live - 1 : 0)
		{}
		// ----------------------------------------------------------------------
		TreeRoutingMessage::
			TreeRoutingMessage( const TreeRoutingMessage& other, const shawn::Node& next_hop ) :
		RoutingMessage( other ),
		next_hop_(&next_hop),
		time_to_live_(other.time_to_live_ - 1)
		{
			assert( time_to_live_ >= 0 );
		}
		// ----------------------------------------------------------------------
		TreeRoutingMessage::
			~TreeRoutingMessage()
		{}
		// ----------------------------------------------------------------------
		int
			TreeRoutingMessage::
			size( void )
			const
			throw()
		{
			return (USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() : application_message()->size())
					+ MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		const Node*
			TreeRoutingMessage::
			next_hop()
			const
			throw()
		{
			return next_hop_;
		}
		// ----------------------------------------------------------------------
		int
			TreeRoutingMessage::
			time_to_live()
			const
			throw()
		{
			return time_to_live_;
		}
		// ----------------------------------------------------------------------
		std::ostream&
			operator<<(std::ostream& os, const TreeRoutingMessage& trm)
		{
			return os << "Next hop: " << trm.next_hop_ << "\n"
					  << "Time to live: " << trm.time_to_live_ << "\n"
					  << "Routing instance: " << trm.routing_instance_;
		}

		// TreeConstructionMessage:

		TreeConstructionMessage::
			TreeConstructionMessage( TreeRouting& ri, const shawn::Node& sink, double sink_creation_time ) :
		sink_(&sink),
		hop_count_(1),
		sink_creation_time_(sink_creation_time),
		routing_instance_(&ri)
		{
			setSize(MESSAGE_OVERHEAD);
		}
		// ----------------------------------------------------------------------
		TreeConstructionMessage::
			TreeConstructionMessage( const TreeConstructionMessage& other ) :
		sink_(other.sink_),
		hop_count_(other.hop_count_ + 1),
		sink_creation_time_(other.sink_creation_time_),
		routing_instance_(other.routing_instance_)
		{
			setSize(MESSAGE_OVERHEAD);
		}
		// ----------------------------------------------------------------------
		TreeConstructionMessage::
			~TreeConstructionMessage()
		{}
		// ----------------------------------------------------------------------
		const shawn::Node*
			TreeConstructionMessage::
			sink()
			const
			throw()
		{
			assert(sink_);
			return sink_;
		}
		// ----------------------------------------------------------------------
		int
			TreeConstructionMessage::
			hop_count()
			const
			throw()
		{
			return hop_count_;
		}
		// ----------------------------------------------------------------------
		double
			TreeConstructionMessage::
			sink_creation_time()
			const
			throw()
		{
			return sink_creation_time_;
		}
		// ----------------------------------------------------------------------
		const TreeRouting*
			TreeConstructionMessage::
			routing_instance()
			const
			throw()
		{
			assert(routing_instance_);
			return routing_instance_;
		}
		// ----------------------------------------------------------------------
		TreeRouting*
			TreeConstructionMessage::
			routing_instance_w()
			const
			throw()
		{
			assert(routing_instance_);
			return routing_instance_;
		}
		// ----------------------------------------------------------------------
		std::ostream&
			operator<<(std::ostream& os, const TreeConstructionMessage& tcm)
		{
			return os << "Sink/Id: " << tcm.sink_ << "/" << tcm.sink_->id() << "\n"
				<< "Hop count: " << tcm.hop_count_ << "\n"
				<< "Sink creation time: " << tcm.sink_creation_time_ << "\n"
				<< "Routing instance: " << tcm.routing_instance_;
		}
	}
}
#endif
