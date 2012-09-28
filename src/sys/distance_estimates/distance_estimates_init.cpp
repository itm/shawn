/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/distance_estimates_init.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/distance_estimates/task_randomized_distance_estimate.h"
#include "sys/distance_estimates/task_absolute_error_distance_estimate.h"
#include "sys/distance_estimates/task_lqi_distance_estimate.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

namespace shawn
{

	void 
		init_distance_estimates( SimulationController& sc )
		throw()
	{
		sc.distance_estimate_keeper_w().add( new PerfectDistanceEstimate );
		sc.simulation_task_keeper_w().add( new SimulationTaskRandomizedDistanceEstimate );
		sc.simulation_task_keeper_w().add( new SimulationTaskAbsoluteErrorDistanceEstimate);
		sc.simulation_task_keeper_w().add( new SimulationTaskLQIDistanceEstimate);

	}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/distance_estimates_init.cpp,v $
 * Version $Revision: 474 $
 * Date    $Date: 2010-11-25 14:40:05 +0100 (Thu, 25 Nov 2010) $
 *-----------------------------------------------------------------------
 * $Log: distance_estimates_init.cpp,v $
 *-----------------------------------------------------------------------*/
