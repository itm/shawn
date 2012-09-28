/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "routing_init.h"
#ifdef ENABLE_ROUTING

//#include <iostream>

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

#include "apps/routing/routing_init.h"
#include "apps/routing/routing_keeper.h"
#include "apps/routing/routing_task.h"

// TreeRouting
#include "apps/routing/tree/tree_routing.h"
#include "apps/routing/tree/tree_routing_factory.h"

#include "apps/routing/tree/tree_creation_task.h"

// FloodRouting
#include "apps/routing/flood/flood_routing.h"
#include "apps/routing/flood/flood_routing_factory.h"

// GeoRouting
#include "apps/routing/geo/geo_routing.h"
#include "apps/routing/geo/geo_routing_factory.h"

#include "apps/routing/geo/geo_neighborhood_creation_task.h"

// CentralizedFloodRouting
#include "apps/routing/centralized_flood/centralized_flood_routing.h"
#include "apps/routing/centralized_flood/centralized_flood_routing_factory.h"

// CentralizedTreeRouting
#include "apps/routing/centralized_tree/centralized_tree_routing.h"
#include "apps/routing/centralized_tree/centralized_tree_routing_factory.h"

// Simplified GeoRouting
#include "apps/routing/simplified_geo/simplified_geo_routing.h"
#include "apps/routing/simplified_geo/simplified_geo_routing_factory.h"

//#include "apps/routing/simplified_geo/simplified_geo_neighborhood_creation_task.h"

extern "C" void init_routing( shawn::SimulationController& sc )
{
	// Create two keepers in the simulation controller 
    // one for the factories ... 
	sc.add_keeper( new routing::RoutingFactoryKeeper );
	// ... and one for the created routing protocol instances
	sc.add_keeper( new routing::RoutingKeeper );
	sc.simulation_task_keeper_w().add( new routing::RoutingTask );

	routing::RoutingFactoryKeeper& rfk = routing::routing_factory_keeper_w(sc);

	// Initialize the routing algorithms
	
	// Distributed Tree
	{
		routing::tree::TreeRoutingProcessorFactory::register_factory(sc);
		rfk.add( new routing::tree::TreeRoutingFactory );
		// Tree knowledge injection
		sc.simulation_task_keeper_w().add( new routing::tree::TreeCreationTask );
	}

	// Distributed Flood
	{
		routing::flood::FloodRoutingProcessorFactory::register_factory(sc);
		rfk.add( new routing::flood::FloodRoutingFactory );
	}

	// Distributed GeoRouting
	{
		routing::geo::GeoRoutingProcessorFactory::register_factory(sc);
		rfk.add( new routing::geo::GeoRoutingFactory );
		// Pre creates the neighborhood
		sc.simulation_task_keeper_w().add( new routing::geo::GeoNeighborhoodCreationTask );
	}

	// Centralized FloodRouting
	{
		rfk.add( new routing::flood::CentralizedFloodRoutingFactory );
	}

	// Centralized TreeRouting
	{
		rfk.add( new routing::tree::CentralizedTreeRoutingFactory );
		sc.simulation_task_keeper_w().add( new routing::tree::CentralizedTreeCreationTask ); 
	}

	// Simplified GeoRouting
	{
		routing::geo::SimplifiedGeoRoutingProcessorFactory::register_factory(sc);
		rfk.add( new routing::geo::SimplifiedGeoRoutingFactory );
		//// Pre creates the neighborhood
		//sc.simulation_task_keeper_w().add( new routing::geo::SimplifiedGeoNeighborhoodCreationTask );
	}
}

#endif


