/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_INIT_H
#define __SHAWN_VIS_INIT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

namespace shawn
{ class SimulationController; }

extern "C" void init_vis( shawn::SimulationController& );

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/vis_init.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: vis_init.h,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

