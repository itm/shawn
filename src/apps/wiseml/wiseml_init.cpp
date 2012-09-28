/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/wiseml/wiseml_init.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/world/wisemlworld_task.h"
#include "apps/wiseml/writer/tasks/wiseml_writer_task.h"
#include "apps/wiseml/writer/tasks/wiseml_setup_task.h"
#include "apps/wiseml/writer/tasks/wiseml_writer_test.h"
#include "apps/wiseml/writer/tasks/wiseml_node_defaults_task.h"
#include "apps/wiseml/writer/tasks/wiseml_link_defaults_task.h"
#include "apps/wiseml/writer/tasks/wiseml_default_capability_task.h"
#include "apps/wiseml/writer/tasks/wiseml_trace_task.h"
#include "apps/wiseml/writer/tasks/wiseml_scenario_task.h"
#include "apps/wiseml/communication/wiseml_comm_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "apps/reading/sensors/sensor_keeper.h"
#include "apps/wiseml/sensors/wiseml_string_sensor_factory.h"
#include "apps/wiseml/examples/wiseml_example_processor_factory.h"
#include "apps/wiseml/writer/wiseml_data_keeper.h"

#include <iostream>

extern "C" void init_wiseml( shawn::SimulationController& sc )
{
   sc.simulation_task_keeper_w().add( new wiseml::SimulationTaskWiseMLWorldFactory );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlWriterTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlWriterTest );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlSetupTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlTraceTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlScenarioTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlNodeDefaultsTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlLinkDefaultsTask );
   sc.simulation_task_keeper_w().add( new wiseml::WisemlDefaultCapabilityTask );
   sc.communication_model_keeper_w().add( new wiseml::WiseMlCommModelFactory );
   wiseml::WisemlExampleProcessorFactory::register_factory(sc);
   /**
    * For adding Wiseml<type>SensorFactories, see 
    * Wiseml::WiseMLWorldFactory::fillworld(...) method
    */
}

#endif
