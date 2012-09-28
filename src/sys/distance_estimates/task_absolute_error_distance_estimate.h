/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_ABSOLUTE_ERROR_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_TASK_ABSOLUTE_ERROR_DISTANCE_ESTIMATE_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{

   class SimulationTaskAbsoluteErrorDistanceEstimate
      : public SimulationTask
   {
   public:
      SimulationTaskAbsoluteErrorDistanceEstimate();
      virtual ~SimulationTaskAbsoluteErrorDistanceEstimate();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/task_randomized_distance_estimate.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: task_randomized_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/
