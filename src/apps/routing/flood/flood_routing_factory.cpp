/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "flood_routing_factory.h"
#ifdef ENABLE_ROUTING

#include <limits.h>

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/flood/flood_routing_factory.h"
#include "apps/routing/flood/flood_routing_processor.h"
#include "apps/routing/flood/flood_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace flood
	{
		// FloodRoutingProcessorFactory:

		const string FloodRoutingProcessorFactory::PROCESSOR_NAME = "flood";
		//-----------------------------------------------------------------------
		FloodRoutingProcessorFactory::
			FloodRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		FloodRoutingProcessorFactory::
			~FloodRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		void
			FloodRoutingProcessorFactory::
			register_factory( SimulationController& sc )
			throw()
		{
			sc.processor_keeper_w().add( new FloodRoutingProcessorFactory );
		}
		//-----------------------------------------------------------------------
		string
			FloodRoutingProcessorFactory::
			name( void )
			const
			throw()
		{
			return PROCESSOR_NAME;
		}
		//-----------------------------------------------------------------------
		string
			FloodRoutingProcessorFactory::
			description( void )
			const
			throw()
		{
			return "flood. Recommended for routing internal use.";
		}
		//-----------------------------------------------------------------------
		shawn::Processor* 
			FloodRoutingProcessorFactory::
			create( void )
			throw()
		{
			return new FloodRoutingProcessor;
		}

		// FloodRoutingFactory:

		const string FloodRoutingFactory::FACTORY_NAME = "flood";
		//-----------------------------------------------------------------------
		FloodRoutingFactory::
			~FloodRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase*
			FloodRoutingFactory::
			create( const string& name, SimulationController& sc )
			throw()
		{
			int history_max_size = sc.environment().optional_int_param("history_max_size",INT_MAX);
			if( history_max_size < 0 )
			{
				FATAL(this->logger(),"Invalid history_max_size!");
				abort();
			}
			double probability = sc.environment().optional_double_param("resending_probability",1.0);
			if( probability < 0.0 || probability > 1.0 )
			{
				FATAL(this->logger(),"Invalid resending_probability!");
				abort();
			}
			double message_sending_jitter_lower_bound = 
				sc.environment().optional_double_param("message_sending_jitter_lower_bound",0.0);
			double message_sending_jitter_upper_bound = 
				sc.environment().optional_double_param("message_sending_jitter_upper_bound",0.02);
			return new FloodRouting(name,
									history_max_size,
									probability,
									message_sending_jitter_lower_bound,
									message_sending_jitter_upper_bound);
		}
		//-----------------------------------------------------------------------
		string
			FloodRoutingFactory::
			name()
			const
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		string
			FloodRoutingFactory::
			description()
			const
			throw()
		{
			return "Flood routing factory";
		}
	}
}
#endif
