/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/task_absolute_error_distance_estimate.h"
#include "sys/distance_estimates/absolute_error_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <limits>
#include <iostream>

namespace shawn
{

	// ----------------------------------------------------------------------
	SimulationTaskAbsoluteErrorDistanceEstimate::
		SimulationTaskAbsoluteErrorDistanceEstimate()
	{}
	   
	// ----------------------------------------------------------------------
	SimulationTaskAbsoluteErrorDistanceEstimate::
		~SimulationTaskAbsoluteErrorDistanceEstimate()
	{}
	   
	// ----------------------------------------------------------------------
	void
		SimulationTaskAbsoluteErrorDistanceEstimate::
		run( SimulationController& sc )
		throw( std::runtime_error )
	{
		std::string name = sc.environment().required_string_param("name");
		double offset_err = sc.environment().optional_double_param("offset", 0.0);
		double absolute_err = sc.environment().required_double_param("error");

		AbsoluteErrorDistanceEstimate* de = new AbsoluteErrorDistanceEstimate(name, offset_err, absolute_err);
		sc.distance_estimate_keeper_w().add(de);

		std::cout << "Created new " << de->name() << " distance estimate, name = " << name 
			<< ", offset_err = " << offset_err 
			<< ", absolute_err = "<< absolute_err 
			<< std::endl;
	}
	
	// ----------------------------------------------------------------------
	std::string
	SimulationTaskAbsoluteErrorDistanceEstimate::
	name( void )
	  const throw()
	{
		return std::string("create_absolute_error_distance_estimate");
	}
	// ----------------------------------------------------------------------
	std::string
	SimulationTaskAbsoluteErrorDistanceEstimate::
	description( void )
	  const throw()
	{
		return std::string("$name: Name of the distance estimate, $error: absolute error, $offset");
	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/task_randomized_distance_estimate.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: task_randomized_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
