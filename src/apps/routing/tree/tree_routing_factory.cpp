/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "tree_routing_factory.h"
#ifdef ENABLE_ROUTING

#include <float.h>
#include <limits.h>

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/tree/tree_routing_factory.h"
#include "apps/routing/tree/tree_routing_processor.h"
#include "apps/routing/tree/tree_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace tree
	{
		// TreeRoutingProcessorFactory:

		const string TreeRoutingProcessorFactory::PROCESSOR_NAME = "tree";
		//-----------------------------------------------------------------------
		TreeRoutingProcessorFactory::
			TreeRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		TreeRoutingProcessorFactory::
			~TreeRoutingProcessorFactory()
		{}
		//-----------------------------------------------------------------------
		void
			TreeRoutingProcessorFactory::
			register_factory( SimulationController& sc )
			throw()
		{
			sc.processor_keeper_w().add( new TreeRoutingProcessorFactory );
		}
		//-----------------------------------------------------------------------
		string
			TreeRoutingProcessorFactory::
			name( void )
			const
			throw()
		{
			return PROCESSOR_NAME;
		}
		//-----------------------------------------------------------------------
		string
			TreeRoutingProcessorFactory::
			description( void )
			const
			throw()
		{
			return "tree. Recommended for routing internal use.";
		}
		//-----------------------------------------------------------------------
		shawn::Processor* 
			TreeRoutingProcessorFactory::
			create( void )
			throw()
		{
			return new TreeRoutingProcessor;
		}

		// TreeRoutingFactory:

		const string TreeRoutingFactory::FACTORY_NAME = "tree";
		//-----------------------------------------------------------------------
		TreeRoutingFactory::
			~TreeRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase*
			TreeRoutingFactory::
			create( const string& name, SimulationController& sc )
			throw()
		{
			double tree_construction_interval = 
				sc.environment().optional_double_param("tree_construction_interval",DBL_MAX / 3);
			double tree_expiration_interval = 
				sc.environment().optional_double_param("tree_expiration_interval",3 * tree_construction_interval);
			int tree_routing_message_time_to_live = 
				sc.environment().optional_int_param("tree_routing_message_time_to_live",INT_MAX);
			int tree_max_hops = sc.environment().optional_int_param("tree_max_hops",INT_MAX);
			double tree_construction_sending_jitter_lower_bound = 
				sc.environment().optional_double_param("tree_construction_sending_jitter_lower_bound",0.0);
			double tree_construction_sending_jitter_upper_bound = 
				sc.environment().optional_double_param("tree_construction_sending_jitter_upper_bound",0.1);
			return new TreeRouting( name, 
								    tree_construction_interval,
								    tree_expiration_interval,
								    tree_routing_message_time_to_live, 
								    tree_max_hops,
								    tree_construction_sending_jitter_lower_bound,
								    tree_construction_sending_jitter_upper_bound );
		}
		//-----------------------------------------------------------------------
		string
			TreeRoutingFactory::
			name()
			const
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		string
			TreeRoutingFactory::
			description()
			const
			throw()
		{
			return "Tree routing factory";
		}
	}
}
#endif
