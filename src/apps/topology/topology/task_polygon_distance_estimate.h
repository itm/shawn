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

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_POLYGON_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_POLYGON_DISTANCE_ESTIMATE_H

#include "sys/simulation/simulation_task.h"

namespace topology
{

   class SimulationTaskPolygonDistanceEstimate
	   : public shawn::SimulationTask
   {
   public:
      SimulationTaskPolygonDistanceEstimate();
      virtual ~SimulationTaskPolygonDistanceEstimate();
      virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };

}

#endif
#endif
