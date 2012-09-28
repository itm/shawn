/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/world/wisemlworld_task.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/worlds/world_tasks.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/util/defutils.h"
#include "apps/wiseml/world/wisemlworld_task.h"
#include "apps/wiseml/world/wisemlworld_factory.h"

using namespace shawn;
using namespace shawn::xml;
using namespace std;

namespace wiseml
{
   SimulationTaskWiseMLWorldFactory::
   SimulationTaskWiseMLWorldFactory()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskWiseMLWorldFactory::
       ~SimulationTaskWiseMLWorldFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskWiseMLWorldFactory::
       name( void )
       const throw()
   {
       return "wiseml_world";
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskWiseMLWorldFactory::
       description( void )
       const throw()
   {
       return "Reads topology from a WiseML file";
   }
   // ----------------------------------------------------------------------
   shawn::ProcessorWorldFactory*
   SimulationTaskWiseMLWorldFactory::
      create_factory( shawn::SimulationController& sc )
       throw( std::runtime_error )
   {
      WiseMLWorldFactory* wmlf = NULL;

      try
      {
         std::string file = sc.environment().required_string_param("file");
         std::string scenario = sc.environment().optional_string_param("scenario_id", "");
         std::string trace = sc.environment().optional_string_param("trace_id", "");
         double timefactor = sc.environment().optional_double_param("timefactor", 1.0);
         wmlf = new WiseMLWorldFactory();
         wmlf->set_filename( file );
         wmlf->set_scenario_id(scenario);
         wmlf->set_trace_id(trace);
         wmlf->set_timefactor(timefactor);
      }
      catch( std::runtime_error& )
      {
         delete wmlf;
         throw;
      }

      return wmlf;
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskWiseMLWorldFactory::
      set_node_count( shawn::SimulationController&,
      shawn::ProcessorWorldFactory& )
       throw( std::runtime_error )
   {
       // nothing -- wiseml factory needs no count
   }
}

#endif
