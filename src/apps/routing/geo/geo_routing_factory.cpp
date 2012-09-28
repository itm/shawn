/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "geo_routing_factory.h"
#ifdef ENABLE_ROUTING

#include <limits.h>

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/geo/geo_routing_factory.h"
#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/geo_routing_processor.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace geo
	{
		// GeoRoutingProcessorFactory:

		const string GeoRoutingProcessorFactory::PROCESSOR_NAME = "geo";
		//-----------------------------------------------------------------------
		GeoRoutingProcessorFactory::
			GeoRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		GeoRoutingProcessorFactory::
			~GeoRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		void
			GeoRoutingProcessorFactory::
			register_factory( SimulationController& sc )
			throw()
		{
			sc.processor_keeper_w().add( new GeoRoutingProcessorFactory );
		}
		//-----------------------------------------------------------------------
		string
			GeoRoutingProcessorFactory::
			name( void )
			const
			throw()
		{
			return PROCESSOR_NAME;
		}
		//-----------------------------------------------------------------------
		string
			GeoRoutingProcessorFactory::
			description( void )
			const
			throw()
		{
			return "geo. Recommended for routing internal use.";
		}
		//-----------------------------------------------------------------------
		shawn::Processor* 
			GeoRoutingProcessorFactory::
			create( void )
			throw()
		{
			return new GeoRoutingProcessor;
		}
		//-----------------------------------------------------------------------

		// GeoRoutingFactory:

		const string GeoRoutingFactory::FACTORY_NAME = "geo";
		//-----------------------------------------------------------------------
		GeoRoutingFactory::
			~GeoRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase*
			GeoRoutingFactory::
			create( const string& name, SimulationController& sc )
			throw()
		{
			double beacon_interval = 
				sc.environment().optional_double_param("beacon_interval",1.0);
			if(beacon_interval < 0.0)
			{
				FATAL(this->logger(),"beacon_interval must ba a value greater 0.0");
				abort();
			}

			double magnitude_in_beacon_interval = 
				sc.environment().optional_double_param("magnitude_in_beacon_interval",0.5);
			if(magnitude_in_beacon_interval < 0.0 || magnitude_in_beacon_interval > 1.0)
			{
				FATAL(this->logger(),"magnitude_in_beacon_interval must ba a value between 0.0 and 1.0");
				abort();
			}

			int messages_time_to_live = 
				sc.environment().optional_int_param("messages_time_to_live",INT_MAX);
			if(messages_time_to_live < 0)
			{
				FATAL(this->logger(),"messages_time_to_live must be a value greater or equal 0");
				abort();
			}
			double beacon_expiration_interval = sc.environment().
				optional_double_param("beacon_expiration_interval",
				3*(beacon_interval + magnitude_in_beacon_interval * beacon_interval));

			bool route_always_in_perimeter_mode = 
				sc.environment().optional_bool_param("route_always_in_perimeter_mode",false);

			return new GeoRouting(name, 
								  beacon_interval, 
								  magnitude_in_beacon_interval, 
								  beacon_expiration_interval, 
								  messages_time_to_live,
								  route_always_in_perimeter_mode);
		}
		//-----------------------------------------------------------------------
		string
			GeoRoutingFactory::
			name()
			const
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		string
			GeoRoutingFactory::
			description()
			const
			throw()
		{
			return "Geo routing factory";
		}
	}
}
#endif
