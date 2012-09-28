/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/testbedservice/testbedservice_init.h"
#ifdef ENABLE_TESTBEDSERVICE

#include <iostream>
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "apps/testbedservice/testbedservice_task.h"
#include "apps/testbedservice/sockets/socket_task.h"
#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "apps/testbedservice/virtual_links/virtual_link_task.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model_factory.h"
#include "apps/testbedservice/processor/example_testbedservice_processor_factory.h"
#include "apps/testbedservice/core/testbedservice_client.h"

#include "sys/simulation/simulation_controller.h"

extern "C" void init_testbedservice( shawn::SimulationController& sc )
{
   std::cout << "Initialising Testbedservice Module" << std::endl;
   sc.simulation_task_keeper_w().add( new testbedservice::TestbedServiceTask );
   sc.simulation_task_keeper_w().add( new testbedservice::SocketTask );
   sc.simulation_task_keeper_w().add( new testbedservice::TestbedServiceVirtualLinkTask );

   sc.transmission_model_keeper_w().add( new testbedservice::VirtualLinkTransmissionModelFactory() );

   testbedservice::ExampleTestbedServiceProcessorFactory::register_factory( sc );
}

#endif
