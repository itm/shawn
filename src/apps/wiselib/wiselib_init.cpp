/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiselib/wiselib_init.h"
#ifdef ENABLE_WISELIB

#include <iostream>
#include "sys/simulation/simulation_controller.h"
#include "apps/wiselib/wiselib_example_processor_factory.h"
#include "apps/wiselib/metrics/one_hop_metrics_processor_factory.h"
#include "apps/wiselib/metrics/individual_link_metrics_processor_factory.h"

#include "sys/simulation/simulation_controller.h"

extern "C" void init_wiselib( shawn::SimulationController& sc )
{
   std::cout << "Initialising Wiselib-Shawn module" << std::endl;
   wiselib::WiselibExampleProcessorFactory::register_factory( sc );
   wiselib::OneHopMetricsProcessorFactory::register_factory( sc );
   wiselib::IndividualLinkMetricsProcessorFactory::register_factory( sc );
}

#endif
