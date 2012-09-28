/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#ifndef __SHAWN_LEGACYAPPS_AUTOCAST_INIT_H
#define __SHAWN_LEGACYAPPS_AUTOCAST_INIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

namespace shawn
{ class SimulationController; }

extern "C" void init_autocast( shawn::SimulationController& );

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/



