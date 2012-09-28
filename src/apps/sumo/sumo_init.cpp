/**
 * @file sumo_init.cpp
 * Implementation of the initialisation functions.
 *
 * @author Josef Schlittenlacher
 * @author Walter Bamberger
 *
 * $Revision: 497 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_init.cpp $
 * $Id: sumo_init.cpp 497 2011-01-20 11:09:02Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010 Josef Schlittenlacher<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "sumo_init.h"
#ifdef ENABLE_SUMO

#include "log_movement_processor/log_movement_processor_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"
#include <iostream>

extern "C" void
init_sumo( shawn::SimulationController& sc )
{
   std::cout << "Initialising app SUMO" << std::endl;
   sc.processor_keeper_w().add(new sumo::LogMovementProcessorFactory());
}

#endif /* ENABLE_SUMO */
