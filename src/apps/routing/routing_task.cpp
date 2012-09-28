/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "routing_task.h"
#ifdef ENABLE_ROUTING

#include <sstream>
#include <string>

#include "sys/node.h"
#include "sys/processors/processor_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"

#include "apps/routing/routing.h"
#include "apps/routing/routing_factory.h"
#include "apps/routing/routing_keeper.h"
#include "apps/routing/routing_task.h"

using namespace std;

namespace routing
{
	// Routing Task:

	RoutingTask::
		RoutingTask()
	{}
	// ----------------------------------------------------------------------
	RoutingTask::
		~RoutingTask()
	{}
	// ----------------------------------------------------------------------
	std::string
		RoutingTask::
		name( void )
		const
		throw()
	{
		return "create_routing_protocol";
	}
	// ----------------------------------------------------------------------
	std::string
		RoutingTask::
		description( void )
		const
		throw()
	{
		return "Creates a new routing protocol instance. It places the instance in the routing_keeper. \
			   The keeper can be obtained using the methods routing::routing_keeper() and \
			   routing::routing_keeper_w().";
	}
	// ----------------------------------------------------------------------
	void
		RoutingTask::
		run( shawn::SimulationController& sc )
		throw( std::runtime_error )
	{
		const shawn::SimulationEnvironment& se = sc.environment();
		string protocol = se.required_string_param("protocol");
		string name = se.optional_string_param("name",protocol);

		routing::RoutingFactoryKeeper& rfk = routing::routing_factory_keeper_w(sc);
		RoutingFactoryHandle rfh = rfk.find_w(protocol);

		if( rfh.is_null() ){
			ostringstream oss;
			oss << "No factory for the routing algorithm '" << protocol << "' found. Available factories are: " << endl;
			rfk.print_contents(oss, true);
			throw std::runtime_error(oss.str());
		}

		routing::RoutingKeeper& rk = routing::routing_keeper_w(sc);
		rk.add( rfh->create(name,sc) );
	}
}

#endif

