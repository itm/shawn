/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "geo_routing_processor.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/geo/geo_routing_factory.h"
#include "apps/routing/geo/geo_routing_processor.h"
#include "apps/routing/geo/geo_routing_message.h"

using namespace shawn;
using namespace std;

namespace routing
{
	namespace geo
	{
		// GeoRoutingProcessor:

		GeoRoutingProcessor::
			GeoRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		GeoRoutingProcessor::
			~GeoRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		void 
			GeoRoutingProcessor::
			boot( void ) 
			throw()
		{
			routing::RoutingKeeper& rk = routing::routing_keeper_w( owner_w().world_w().simulation_controller_w() );
			for( RoutingKeeper::HandleMapType::iterator it = rk.begin_handles_w(); 
				 it != rk.end_handles_w(); ++it )
			{
				GeoRouting* routing_instance = dynamic_cast<GeoRouting*>( it->second.get() );
				if( routing_instance )
				{
					routing_instance->boot( owner_w() );
				}
			}
		}
		// ----------------------------------------------------------------------
		bool 
			GeoRoutingProcessor::
			process_message( const shawn::ConstMessageHandle& mh ) 
			throw()
		{
			const GeoRoutingBeaconMessage* bm = dynamic_cast<const GeoRoutingBeaconMessage*>( mh.get() );
			if(bm)
			{
				return bm->routing_instance_w()->process_beacon_message(owner_w(),*bm);
			}
			const GeoRoutingPerimeterMessage* pm = dynamic_cast<const GeoRoutingPerimeterMessage*>( mh.get() );
			if(pm)
			{
				GeoRouting* routing_instance = static_cast<GeoRouting*>( pm->routing_instance_w() );
				return routing_instance->process_perimeter_message(owner_w(),*pm);
			}
			const GeoRoutingGreedyMessage* gm = dynamic_cast<const GeoRoutingGreedyMessage*>( mh.get() );
			if(gm)
			{
				GeoRouting* routing_instance = static_cast<GeoRouting*>( gm->routing_instance_w() );
				return routing_instance->process_greedy_message(owner_w(),*gm);
			}
			return Processor::process_message( mh );
		}
		// ----------------------------------------------------------------------
		void 
			GeoRoutingProcessor::
			work( void ) 
			throw()
		{
			//
		}
	}
}
#endif
