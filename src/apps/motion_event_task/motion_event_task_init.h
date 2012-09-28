/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_INIT_H
#define __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_INIT_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_MOTIONEVENTTASK

#include "sys/world.h"

namespace shawn
{ class SimulationController; }

namespace motion_event
{
	extern "C" void init_motioneventtask( shawn::SimulationController& );
}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task_init.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2007/01/31 17:24:17 $
 *-----------------------------------------------------------------------
 * $Log: motion_event_task_init.h,v $
 * Revision 1.2  2007/01/31 17:24:17  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2007/01/29 16:37:14  krueger
 * *** empty log message ***
 *
 * Revision 1.2  2005/04/07 13:37:33  lipphardt
 * corrected #ifndef / #define variable name
 *
 * Revision 1.1  2005/03/05 06:43:10  tbaum
 * added motion_event_task
 *
*-----------------------------------------------------------------------*/
