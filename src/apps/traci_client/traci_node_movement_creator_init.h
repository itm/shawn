/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_INIT_H
#define __SHAWN_LEGACYAPPS_TRACINODEMOVEMENTCREATOR_INIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TRACICLIENT

namespace shawn
{
class SimulationController;
}


extern "C" void init_tracinodemvementcreator( shawn::SimulationController& );


#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.h,v $
 *-----------------------------------------------------------------------*/

