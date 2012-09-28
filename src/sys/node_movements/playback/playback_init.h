/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENT_PLAYBACK_PLAYBACK_INIT
#define __SHAWN_SYS_NODE_MOVEMENT_PLAYBACK_PLAYBACK_INIT

#include "sys/simulation/simulation_controller.h"


namespace shawn
{

   void init_playback_tasks( SimulationController& sc ) throw();

   void init_playback( SimulationController& sc ) throw();

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/playback_init.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: playback_init.h,v $
 *-----------------------------------------------------------------------*/
