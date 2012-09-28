/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "distest_init.h"
#ifdef ENABLE_DISTEST

#include "apps/distest/neighbor_intersection_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "apps/distest/distest_task.h"
#include "apps/distest/multihop_distest_task.h"
#include "apps/distest/test_task.h"


extern "C" void init_distest( shawn::SimulationController& sc )
{
	sc.distance_estimate_keeper_w().add( new shawn::NeighborhoodIntersectionDistanceEstimate );
	sc.simulation_task_keeper_w().add( new distest::DistanceEstimateTask );
	sc.simulation_task_keeper_w().add( new distest::MultihopDistanceEstimateTask );
	sc.simulation_task_keeper_w().add( new distest::TestTask );
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/distest/distest_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: distest_init.cpp,v $
 *-----------------------------------------------------------------------*/
