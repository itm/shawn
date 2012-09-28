/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_25D_TOPOLOGY_TASK_H
#define __SHAWN_APPS_TOPOLOGY_25D_TOPOLOGY_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace topology
{

   class Topology25DTask
      : public shawn::SimulationTask
   {
   public:
      Topology25DTask();
      virtual ~Topology25DTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_25d_task.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: topology_25d_task.h,v $
 *-----------------------------------------------------------------------*/
