/**
 * @file vanet_init.cpp
 * Implements the app's initialisation functions
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vanet_init.cpp $
 * $Id: vanet_init.cpp 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "vanet_init.h"
#ifdef ENABLE_VANET

#include "vehicle/vehicle_processor_factory.h"
#include "environment/create_environment_task.h"
#include "init_vanet_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/world.h"
#include <iostream>

extern "C" void init_vanet( shawn::SimulationController& sc )
{
	INFO( sc.logger(), "Initialising VANET module" );

	sc.simulation_task_keeper_w().add( new vanet::InitVanetTask() );
	sc.simulation_task_keeper_w().add( new vanet::CreateEnvironmentTask() );
	vanet::VehicleProcessorFactory::register_factory(sc);
}

#endif
