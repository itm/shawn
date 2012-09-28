/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "simplified_geo_routing_processor.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/simplified_geo/simplified_geo_routing_factory.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_processor.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_message.h"

using namespace shawn;
using namespace std;

namespace routing
{
	namespace geo
	{
		// SimplifiedGeoRoutingProcessor:

		SimplifiedGeoRoutingProcessor::
			SimplifiedGeoRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		SimplifiedGeoRoutingProcessor::
			~SimplifiedGeoRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		void 
			SimplifiedGeoRoutingProcessor::
			boot( void ) 
			throw()
		{
			routing::RoutingKeeper& rk = routing::routing_keeper_w( owner_w().world_w().simulation_controller_w() );
			for( RoutingKeeper::HandleMapType::iterator it = rk.begin_handles_w(); 
				 it != rk.end_handles_w(); ++it )
			{
				SimplifiedGeoRouting* routing_instance = dynamic_cast<SimplifiedGeoRouting*>( it->second.get() );
				if( routing_instance )
				{
					routing_instance->boot( owner_w() );
				}
			}
		}
		// ----------------------------------------------------------------------
		bool 
			SimplifiedGeoRoutingProcessor::
			process_message( const shawn::ConstMessageHandle& mh ) 
			throw()
		{
			const SimplifiedGeoRoutingPerimeterMessage* pm = dynamic_cast<const SimplifiedGeoRoutingPerimeterMessage*>( mh.get() );
			if(pm)
			{
				SimplifiedGeoRouting* routing_instance = static_cast<SimplifiedGeoRouting*>( pm->routing_instance_w() );
				return routing_instance->process_perimeter_message(owner_w(),*pm);
			}
			const SimplifiedGeoRoutingGreedyMessage* gm = dynamic_cast<const SimplifiedGeoRoutingGreedyMessage*>( mh.get() );
			if(gm)
			{
				SimplifiedGeoRouting* routing_instance = static_cast<SimplifiedGeoRouting*>( gm->routing_instance_w() );
				return routing_instance->process_greedy_message(owner_w(),*gm);
			}
			return Processor::process_message( mh );
		}
		// ----------------------------------------------------------------------
		void 
			SimplifiedGeoRoutingProcessor::
			work( void ) 
			throw()
		{
			//
		}
	}
}
#endif
