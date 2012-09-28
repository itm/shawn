/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_scenario_task.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlScenarioTask::WisemlScenarioTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlScenarioTask::~WisemlScenarioTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlScenarioTask::name() const throw()
   {
      return "wiseml_scenario";
   }
   // ----------------------------------------------------------------------
   std::string WisemlScenarioTask::description() const throw()
   {
      return "Creates a new scenario.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlScenarioTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      if(keeper == NULL)
      {
         cout << "WisemlScenarioTask:"
            << " Unable to create scenario because WisemlDataKeeper was not"
            << " found. Make shure to run wiseml_setup before this task, or"
            << " to create a WisemlDataKeeper by code." << endl;
         return;
      }

      keeper->add_scenario(sc.environment().required_string_param("id"));
   }
}
#endif
