/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "centralized_tree_routing_factory.h"
#ifdef ENABLE_ROUTING

#include <limits.h>

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"

#include "apps/routing/centralized_tree/centralized_tree_routing_factory.h"
#include "apps/routing/centralized_tree/centralized_tree_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace tree
	{
		// CentralizedTreeRoutingFactory:

		const string CentralizedTreeRoutingFactory::FACTORY_NAME = "centralized_tree";
		//-----------------------------------------------------------------------
		CentralizedTreeRoutingFactory::
			~CentralizedTreeRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase*
			CentralizedTreeRoutingFactory::
			create( const string& name, SimulationController& sc )
			throw()
		{
			int tree_max_hops = sc.environment().optional_int_param("tree_max_hops",INT_MAX);
			double message_delay_per_hop = sc.environment().required_double_param("message_delay_per_hop");
			if( message_delay_per_hop < 0.0 )
			{
				FATAL(this->logger(),"Invalid message delay per hop!");
				abort();
			}
			double link_fail_probability = sc.environment().optional_double_param("link_fail_probability",0.0);
			return new CentralizedTreeRouting(name,tree_max_hops,message_delay_per_hop,link_fail_probability);
		}
		//-----------------------------------------------------------------------
		string
			CentralizedTreeRoutingFactory::
			name()
			const
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		string
			CentralizedTreeRoutingFactory::
			description()
			const
			throw()
		{
			return "Centralized tree routing factory";
		}
	}
}
#endif
