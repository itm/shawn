/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_DIST_ESTINIT_H
#define __SHAWN_APPS_DIST_ESTINIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

namespace shawn
{ class SimulationController; }

extern "C" void init_distest( shawn::SimulationController& );
    
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/distest/distest_init.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: distest_init.h,v $
 *-----------------------------------------------------------------------*/
