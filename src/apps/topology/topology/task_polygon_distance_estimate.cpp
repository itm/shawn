/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#include "shawn_config.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/distance_estimates/absolute_error_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/topology/topology_keepers.h"
#include "apps/topology/topology/polygon_topology.h"
#include "apps/topology/comm_models/polygon_comm_model.h"
#include "apps/topology/topology/task_polygon_distance_estimate.h"

#include <iostream>
#include <limits>

using namespace std;
using namespace shawn;

namespace topology
{

	// ----------------------------------------------------------------------
	SimulationTaskPolygonDistanceEstimate::
		SimulationTaskPolygonDistanceEstimate()
	{}

	// ----------------------------------------------------------------------
	SimulationTaskPolygonDistanceEstimate::
		~SimulationTaskPolygonDistanceEstimate()
	{}

	// ----------------------------------------------------------------------
	void
		SimulationTaskPolygonDistanceEstimate::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		string distest_name = sc.environment().required_string_param("name");
		string topology_name = sc.environment().required_string_param("topology");
		double lower = sc.environment().required_double_param("lower_bound");
		double upper = sc.environment().required_double_param("upper_bound");
		string fname = sc.environment().required_string_param("file_name");
		string rssi_dist = sc.environment().required_string_param("rssi_dist");
		string normal_rand = sc.environment().optional_string_param("normaldist", "");
//		bool attenuation = sc.environment().optional_bool_param("with_attenuation", false);

		//Retrieve the topology by the supplied name
		reading::ConstReadingHandle h = topology::topology_keeper(sc).find(topology_name);
		assert( h.is_not_null() );
		const PolygonTopology* pt = dynamic_cast<const PolygonTopology*>( h.get() );

		if( pt == NULL )
		{
			cerr << "The supplied topology (" << topology_name << "( does not exist or is not of type PolygonTopology" << endl;
			exit(1);
		}




		//Parameterize the newly created model
		PolygonTopologyDistEst* de = new PolygonTopologyDistEst(distest_name, *pt, upper, lower, fname, rssi_dist/*, attenuation*/);
		de->set_transmission_range(upper);
		de->init();
		sc.distance_estimate_keeper_w().add(de);


	}

	// ----------------------------------------------------------------------
	std::string
	SimulationTaskPolygonDistanceEstimate::
	name( void )
	  const throw()
	{
		return std::string("create_polygon_distance_estimate");
	}
	// ----------------------------------------------------------------------
	std::string
	SimulationTaskPolygonDistanceEstimate::
	description( void )
	  const throw()
	{
		return std::string("$name: Name of the distance estimate, ");
	}

}

#endif
