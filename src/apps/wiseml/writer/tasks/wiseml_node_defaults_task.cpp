/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_node_defaults_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlNodeDefaultsTask::WisemlNodeDefaultsTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlNodeDefaultsTask::~WisemlNodeDefaultsTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlNodeDefaultsTask::name() const throw()
   {
      return "wiseml_node_defaults";
   }
   // ----------------------------------------------------------------------
   std::string WisemlNodeDefaultsTask::description() const throw()
   {
      return "Gathers data for the setup section of a WiseML file.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlNodeDefaultsTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      WisemlSetupCollector &setup = keeper->setup();

      NodeTemplate nodedef;
      nodedef.gateway = sc.environment().optional_bool_param(
         "is_gateway", false);
      nodedef.image = sc.environment().optional_string_param(
         "programDetails", "");
      nodedef.nodetype = sc.environment().optional_string_param(
         "nodetype", "");
      nodedef.description = replace_underline(
         sc.environment().optional_string_param("description", ""));
      nodedef.posx = sc.environment().optional_double_param(
         "x", 0.0);
      nodedef.posy = sc.environment().optional_double_param(
         "y", 0.0);
      nodedef.posz = sc.environment().optional_double_param(
         "z", 0.0);
      setup.set_node_defaults(nodedef);
   }

   std::string 
      WisemlNodeDefaultsTask::
      replace_underline(std::string src)
   {
      std::string out = src;
      if(!src.empty())
      {
         int first_ul = out.find_first_of('_');
         while(first_ul != std::string::npos)
         {
            out = out.replace(first_ul, 1, " ");
            first_ul = out.find_first_of('_');
         }
      }
      return out;
   }
}
#endif
