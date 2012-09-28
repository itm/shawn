/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_MOTIONEVENTTASK

#include "apps/motion_event_task/motion_event_task_init.h"
#include "apps/motion_event_task/motion_event_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"

namespace motion_event
{
	extern "C" void init_motioneventtask( shawn::SimulationController& sc )
	{
		sc.simulation_task_keeper_w().add( new motion_event::MotionEventTask );
	}
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task_init.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2007/01/29 16:37:14 $
 *-----------------------------------------------------------------------
 * $Log: motion_event_task_init.cpp,v $
 * Revision 1.1  2007/01/29 16:37:14  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2005/03/05 06:43:10  tbaum
 * added motion_event_task
 *
*-----------------------------------------------------------------------*/
