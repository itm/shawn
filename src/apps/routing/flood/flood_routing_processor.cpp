/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "flood_routing_processor.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/flood/flood_routing_factory.h"
#include "apps/routing/flood/flood_routing_processor.h"
#include "apps/routing/flood/flood_routing_message.h"

using namespace shawn;
using namespace std;

namespace routing
{
	namespace flood
	{
		// FloodRoutingProcessor:

		FloodRoutingProcessor::
			FloodRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		FloodRoutingProcessor::
			~FloodRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		void 
			FloodRoutingProcessor::
			boot( void ) 
			throw()
		{
			routing::RoutingKeeper& rk = routing::routing_keeper_w( owner_w().world_w().simulation_controller_w() );
			for( RoutingKeeper::HandleMapType::iterator it = rk.begin_handles_w(); 
				 it != rk.end_handles_w(); ++it )
			{
				FloodRouting* routing_instance = dynamic_cast<FloodRouting*>( it->second.get() );
				if( routing_instance )
				{
					routing_instance->boot( owner_w() );
				}
			}
		}
		// ----------------------------------------------------------------------
		bool 
			FloodRoutingProcessor::
			process_message( const shawn::ConstMessageHandle& mh ) 
			throw()
		{
			const FloodRoutingMessage* frm = dynamic_cast<const FloodRoutingMessage*>( mh.get() );
			if(frm)
			{
				FloodRouting* routing_instance = static_cast<FloodRouting*>( frm->routing_instance_w() );
				return routing_instance->process_flood_routing_message(owner_w(),*frm);
			}
			return Processor::process_message( mh );
		}
		// ----------------------------------------------------------------------
		void 
			FloodRoutingProcessor::
			work( void ) 
			throw()
		{
			//
		}
	}
}
#endif
