/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_GEO_GEO_NEIGHBORHOOD_CREATION_TASK_H
#define __SHAWN_APPS_ROUTING_GEO_GEO_NEIGHBORHOOD_CREATION_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_task.h"

namespace shawn{ class SimulationController; class Node; }

namespace routing
{
	namespace geo
	{
		class GeoNeighborhoodCreationTask : public shawn::SimulationTask
		{
		public:
			GeoNeighborhoodCreationTask();
			virtual ~GeoNeighborhoodCreationTask();
			virtual std::string name() const throw();
			virtual std::string description() const throw();
			virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error ); 
		};

	}
}
#endif
#endif

