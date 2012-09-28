/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "centralized_flood_routing_factory.h"
#ifdef ENABLE_ROUTING

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"

#include "apps/routing/centralized_flood/centralized_flood_routing_factory.h"
#include "apps/routing/centralized_flood/centralized_flood_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace flood
	{
		const string CentralizedFloodRoutingFactory::FACTORY_NAME = "centralized_flood";
		//-----------------------------------------------------------------------
		CentralizedFloodRoutingFactory::
			~CentralizedFloodRoutingFactory()
		{}
		//-----------------------------------------------------------------------
		routing::RoutingBase* 
			CentralizedFloodRoutingFactory::
			create( const std::string& name, shawn::SimulationController& sc ) 
			throw()
		{
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
			double message_delay_per_hop = sc.environment().required_double_param("message_delay_per_hop");
			if( message_delay_per_hop < 0.0 )
			{
				FATAL(this->logger(),"Invalid message delay per hop!");
				abort();
			}
			double link_fail_probability = sc.environment().optional_double_param("link_fail_probability",0.0);
			if( link_fail_probability < 0.0 || link_fail_probability > 1.0 )
			{
				FATAL(this->logger(),"Invalid link fail probability!");
				abort();
			}
			return new CentralizedFloodRouting(name,
										  probability,
										  message_sending_jitter_lower_bound,
										  message_sending_jitter_upper_bound,
										  message_delay_per_hop,
										  link_fail_probability);
		}
		//-----------------------------------------------------------------------
		std::string 
			CentralizedFloodRoutingFactory::
			name() 
			const 
			throw()
		{
			return FACTORY_NAME;
		}
		//-----------------------------------------------------------------------
		std::string 
			CentralizedFloodRoutingFactory::
			description() 
			const 
			throw()
		{
			return "Centralized flood routing factory";
		}
	}
}

#endif
