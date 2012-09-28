/**
 * @file init_vanet_task.cpp
 * Implements the class InitVanetTask that initialises pre and post step tasks
 * of the VANET app.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/init_vanet_task.cpp $
 * $Id: init_vanet_task.cpp 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "init_vanet_task.h"

#ifdef ENABLE_VANET
#include "statistics/statistics_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"

namespace vanet
{

InitVanetTask::InitVanetTask()
{
}

InitVanetTask::~InitVanetTask()
{
}

std::string
InitVanetTask::name( void ) const throw ()
{
   return "init_vanet";
}

std::string
InitVanetTask::description( void ) const throw ()
{
   return "Initialises the pre and post step tasks of the VANET application";
}

void
InitVanetTask::run( shawn::SimulationController& sc )
         throw ( std::runtime_error )
{
   sc.world_w().add_post_step_task(&StatisticsTask::create(sc));
}

}
#endif
