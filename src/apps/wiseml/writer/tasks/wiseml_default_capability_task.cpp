/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_default_capability_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
#include <fstream>
#include "apps/wiseml/writer/wiseml_generator.h"
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlDefaultCapabilityTask::WisemlDefaultCapabilityTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlDefaultCapabilityTask::~WisemlDefaultCapabilityTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlDefaultCapabilityTask::name() const throw()
   {
      return "wiseml_default_capability";
   }
   // ----------------------------------------------------------------------
   std::string WisemlDefaultCapabilityTask::description() const throw()
   {
      return "Gathers data for the setup section of a WiseML file.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlDefaultCapabilityTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      WisemlSetupCollector &setup = keeper->setup();

      // Link or node capability?
      bool forlink = 
         sc.environment().optional_bool_param("forlink", false);

      Capability cap;
      cap.name = sc.environment().required_string_param("name");
      cap.datatype = sc.environment().required_string_param("datatype");
      cap.unit = sc.environment().required_string_param("unit");
      cap.def_value = sc.environment().required_string_param("value");

      if(!forlink)
      {
         setup.set_default_node_capability(cap);
      }
      else
      {
         setup.set_default_link_capability(cap);
      }

   }
}
#endif
