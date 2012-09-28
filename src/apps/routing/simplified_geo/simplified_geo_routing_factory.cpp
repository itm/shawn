/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "simplified_geo_routing_factory.h"
#ifdef ENABLE_ROUTING

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/simplified_geo/simplified_geo_routing_factory.h"
#include "apps/routing/simplified_geo/simplified_geo_routing.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_processor.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace geo
	{
		// SimplifiedGeoRoutingProcessorFactory:

		const string SimplifiedGeoRoutingProcessorFactory::PROCESSOR_NAME = "simplified_geo";
		//-----------------------------------------------------------------------
		SimplifiedGeoRoutingProcessorFactory::
			SimplifiedGeoRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		SimplifiedGeoRoutingProcessorFactory::
			~SimplifiedGeoRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		void
			SimplifiedGeoRoutingProcessorFactory::
			register_factory( SimulationController& sc )
			throw()
		{
			sc.processor_keeper_w().add( new SimplifiedGeoRoutingProcessorFactory );
		}
		//-----------------------------------------------------------------------
		string
			SimplifiedGeoRoutingProcessorFactory::
			name( void )
			const
			throw()
		{
			return PROCESSOR_NAME;
		}
		//-----------------------------------------------------------------------
		string
			SimplifiedGeoRoutingProcessorFactory::
			description( void )
			const
			throw()
		{
			return "geo. Recommended for routing internal use.";
		}
		//-----------------------------------------------------------------------
		shawn::Processor* 
			SimplifiedGeoRoutingProcessorFactory::
			create( void )
			throw()
		{
			return new SimplifiedGeoRoutingProcessor;
		}
		//-----------------------------------------------------------------------

		// SimplifiedGeoRoutingFactory:

		const string SimplifiedGeoRoutingFactory::FACTORY_NAME = "simplified_geo";
		//-----------------------------------------------------------------------
		SimplifiedGeoRoutingFactory::
			~SimplifiedGeoRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase*
			SimplifiedGeoRoutingFactory::
			create( const string& name, SimulationController& sc )
			throw()
		{
			int messages_time_to_live = 
				sc.environment().optional_int_param("messages_time_to_live",INT_MAX);
			if(messages_time_to_live < 0)
			{
				FATAL(this->logger(),"messages_time_to_live must be a value greater or equal 0");
				abort();
			}
			bool route_always_in_perimeter_mode = 
				sc.environment().optional_bool_param("route_always_in_perimeter_mode",false);
			/*bool neighborhood_update = sc.environment().optional_bool_param("neighborhood_update",true);*/

			return new SimplifiedGeoRouting(name, 
											messages_time_to_live,
											route_always_in_perimeter_mode/*,
											neighborhood_update*/);
		}
		//-----------------------------------------------------------------------
		string
			SimplifiedGeoRoutingFactory::
			name()
			const
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		string
			SimplifiedGeoRoutingFactory::
			description()
			const
			throw()
		{
			return "Simplified geo routing factory";
		}
	}
}
#endif
