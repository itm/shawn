/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/examples/examples_init.h"
#ifdef ENABLE_EXAMPLES

#include "apps/examples/simulationtask/exampletask.h"
#include "apps/examples/simulationtask/connectivity_task.h"
#include "apps/examples/processor/helloworld_processor_factory.h"
#include "apps/examples/processor/helloworld_random_processor_factory.h"
#include "apps/examples/tag/tag_test.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include <iostream>

extern "C" void init_examples( shawn::SimulationController& sc )
{
	std::cout << "Initialising examples" << std::endl;
	
	sc.simulation_task_keeper_w().add( new exampletask::ExampleTask );
	sc.simulation_task_keeper_w().add( new examples::ConnectivityTask );
	
	helloworld::HelloworldProcessorFactory::register_factory(sc);
	helloworld::HelloworldRandomProcessorFactory::register_factory(sc);
	
	sc.simulation_task_keeper_w().add( new examples::tagtest::TagTestTask );
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask_init.cpp,v $
 * Version $Revision: 194 $
 * Date    $Date: 2008-04-28 16:57:41 +0200 (Mon, 28 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: exampletask_init.cpp,v $
 *-----------------------------------------------------------------------*/
