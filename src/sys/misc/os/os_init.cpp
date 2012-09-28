/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/os/os_init.h"
#include "sys/misc/os/log_sys_resources.h"
#include "sys/misc/os/load_plugin_task.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   void 
   init_misc_os( SimulationController& sc )
      throw()
   {
      sc.simulation_task_keeper_w().add( new LogSysResourcesTask );
      sc.simulation_task_keeper_w().add( new LoadPluginTask );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/os_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: os_init.cpp,v $
 *-----------------------------------------------------------------------*/
