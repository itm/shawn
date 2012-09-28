/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"

#include "sys/misc/misc_init.h"
#include "sys/misc/degree/degree_sequences.h"
#include "sys/misc/random/random_init.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"

#include "sys/worlds/save_world_task.h"

#include "sys/misc/localization/localization_anchor_placement.h"
#include "sys/misc/localization/dump_location.h"

#include "sys/misc/os/os_init.h"



namespace shawn
{

   void 
   init_misc( SimulationController& sc )
      throw()
   {
      sc.simulation_task_keeper_w().add( new SimulationTaskDegreeSequence );
      
      sc.simulation_task_keeper_w().add( new SimulationTaskLocalizationAnchorPlacement );
      sc.simulation_task_keeper_w().add( new SimulationTaskDumpLocation );

      init_misc_random(sc);
      init_misc_os(sc);
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/misc_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: misc_init.cpp,v $
 *-----------------------------------------------------------------------*/
