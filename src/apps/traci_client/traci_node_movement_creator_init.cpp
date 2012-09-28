/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "traci_node_movement_creator_init.h"
#ifdef ENABLE_TRACICLIENT

#include "traci_node_movement_creator_init.h"
#include "traci_node_movement_creator.h"

extern "C" void init_traciclient( shawn::SimulationController& sc )
{
    std::cout << "TraCIClient init" << std::endl;
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.cpp,v $
 *-----------------------------------------------------------------------*/

