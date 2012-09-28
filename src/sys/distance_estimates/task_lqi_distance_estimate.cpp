/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/task_lqi_distance_estimate.h"
#include "sys/distance_estimates/lqi_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <limits>
#include <iostream>

namespace shawn
{

	// ----------------------------------------------------------------------
	SimulationTaskLQIDistanceEstimate::
		SimulationTaskLQIDistanceEstimate()
	{}
	   
	// ----------------------------------------------------------------------
	SimulationTaskLQIDistanceEstimate::
		~SimulationTaskLQIDistanceEstimate()
	{}
	   
	// ----------------------------------------------------------------------
	void
		SimulationTaskLQIDistanceEstimate::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		std::string name = sc.environment().required_string_param("name");
		std::string filename = sc.environment().required_string_param("filename");
		int max_measure_size =  sc.environment().optional_int_param("max_measure_size",-1);


		LQIDistanceEstimate* de = new LQIDistanceEstimate(name,filename);

		if(max_measure_size > 0)
			de->set_max_measure_size(max_measure_size);

		de->init();

		sc.distance_estimate_keeper_w().add(de);

		std::cout << "Created new " << de->name() << " distance estimate, name = " << name << " distance estimate, filename = " << filename

			<< std::endl;
	}
	
	// ----------------------------------------------------------------------
	std::string
	SimulationTaskLQIDistanceEstimate::
	name( void )
	  const throw()
	{
		return std::string("create_lqi_distance_estimate");
	}
	// ----------------------------------------------------------------------
	std::string
	SimulationTaskLQIDistanceEstimate::
	description( void )
	  const throw()
	{
		return std::string("$name: Name of the distance estimate");
	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/task_randomized_distance_estimate.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: task_randomized_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
