/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_EXAMPLETASK_EXAMPLETASK_INIT_H
#define __SHAWN_APPS_EXAMPLETASK_EXAMPLETASK_INIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_EXAMPLES

namespace shawn
{ class SimulationController; }


extern "C" void init_examples( shawn::SimulationController& );


#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.h,v $
 *-----------------------------------------------------------------------*/
