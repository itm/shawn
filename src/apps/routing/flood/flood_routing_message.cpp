/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "flood_routing_message.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/flood/flood_routing_message.h"
#include "apps/routing/flood/flood_routing_processor.h"

using namespace std;
using namespace shawn;

// 1 for message type
// 4 for hop count
const int routing::flood::FloodRoutingMessage::MESSAGE_OVERHEAD = 1 + 4;
const bool routing::flood::FloodRoutingMessage::USE_ROUTING_MESSAGE_OVERHEAD = true;

namespace routing
{
	namespace flood
	{
		FloodRoutingMessage::
			FloodRoutingMessage( RoutingBase& routing_instance,
								 const shawn::ConstMessageHandle& application_message,
								 int time_to_live ) : 
		RoutingMessage( routing_instance, 
					    application_message, 
					    time_to_live > 0 ? time_to_live - 1 : 0)
		{}
		// ----------------------------------------------------------------------
		FloodRoutingMessage::
			FloodRoutingMessage( const FloodRoutingMessage& other ) :
		RoutingMessage(*other.routing_instance_w(),
					    other.application_message(),
					    other.destination_address() - 1)
		{
			assert( destination_address_ >= 0 );
		}
		// ----------------------------------------------------------------------
		FloodRoutingMessage::
			~FloodRoutingMessage()
		{}
		// ----------------------------------------------------------------------
		int 
			FloodRoutingMessage::
			size( void )
			const 
			throw()
		{
			return (USE_ROUTING_MESSAGE_OVERHEAD ? RoutingMessage::size() : application_message()->size())
					+ MESSAGE_OVERHEAD;
		}
		// ----------------------------------------------------------------------
		int 
			FloodRoutingMessage::
			time_to_live() 
			const 
			throw()
		{
			assert( destination_address_ >= 0 );
			return destination_address_;
		}
		// ----------------------------------------------------------------------
		std::ostream& 
			operator<<(std::ostream& os, const FloodRoutingMessage& frm)
		{
			return os << "Time to live: " << frm.destination_address_;
		}
	}
}
#endif
