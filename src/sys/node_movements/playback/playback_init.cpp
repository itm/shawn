/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_task_keeper.h"
#include "sys/node_movements/playback/node_movement_task.h"
#include "sys/node_movements/playback/movement_reset_task.h"
#include "sys/simulation/simulation_controller.h"


namespace shawn
{

   void init_playback_tasks( SimulationController& sc ) throw()
   {
      sc.simulation_task_keeper_w().add( new SimulationTaskNodeMovement );
      sc.simulation_task_keeper_w().add( new MovementResetTask );
   }
   // ----------------------------------------------------------------------

   void init_playback( SimulationController& sc ) throw()
   {
      init_playback_tasks(sc);
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/playback_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: playback_init.cpp,v $
 *-----------------------------------------------------------------------*/
