/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "geo_neighborhood_creation_task.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/geo/geo_neighborhood_creation_task.h"
#include "apps/routing/geo/geo_routing.h"

using namespace std;
using namespace shawn;

namespace routing
{
	namespace geo
	{
		GeoNeighborhoodCreationTask::
			GeoNeighborhoodCreationTask()
		{}
		// ----------------------------------------------------------------------
		GeoNeighborhoodCreationTask::
			~GeoNeighborhoodCreationTask()
		{}
		// ----------------------------------------------------------------------
		std::string 
			GeoNeighborhoodCreationTask::
			name() 
			const 
			throw()
		{
			return "geo_neighborhood_creation";
		}
		// ----------------------------------------------------------------------
		std::string 
			GeoNeighborhoodCreationTask::
			description() 
			const 
			throw()
		{
			return "";
		}
		// ----------------------------------------------------------------------
		void 
			GeoNeighborhoodCreationTask::
			run( shawn::SimulationController& sc ) 
			throw( std::runtime_error )
		{
			require_world(sc);
			string geo_routing_instance = sc.environment().required_string_param("geo_routing_instance");
			RoutingBaseHandle rbh = routing::routing_keeper_w(sc).find_w(geo_routing_instance);
			GeoRouting* routing_instance = dynamic_cast<GeoRouting*>( rbh.get() );
			if( ! routing_instance )
			{
				ERROR(this->logger(),"The given routing is no GeoRouting!");
				abort();
			}
			double now = sc.world().current_time();
			routing_instance->init( sc.world_w() );
			for( World::const_node_iterator n_it = sc.world().begin_nodes(); 
				 n_it != sc.world().end_nodes(); ++n_it )
			{
				for( World::const_adjacency_iterator adj_it = n_it->begin_adjacent_nodes(); 
					 adj_it != n_it->end_adjacent_nodes(); ++adj_it )
				{
					routing_instance->
						geo_routing_neighborhood_update(*n_it,*adj_it,GeoRoutingNeighborhoodInfo(now,adj_it->real_position()));
				}
			}
		}
	}
}
#endif


