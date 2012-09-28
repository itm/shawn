/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_setup_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
namespace wiseml
{
   WisemlSetupTask::WisemlSetupTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlSetupTask::~WisemlSetupTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlSetupTask::name() const throw()
   {
      return "wiseml_setup";
   }
   // ----------------------------------------------------------------------
   std::string WisemlSetupTask::description() const throw()
   {
      return "Gathers data for the setup section of a WiseML file.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlSetupTask::run(SimulationController &sc) throw()
   {
      WisemlDataKeeper *keeper = data_keeper(sc);
      WisemlSetupCollector &setup = keeper->setup();

      //Origin:
      double ox = sc.environment().optional_double_param("x", 0.0);
      double oy = sc.environment().optional_double_param("y", 0.0);
      double oz = sc.environment().optional_double_param("z", 0.0);
      double op = sc.environment().optional_double_param("phi", 0.0);
      double ot = sc.environment().optional_double_param("theta", 0.0);
      shawn::Vec origin(ox,oy,oz);
      setup.set_origin(origin, op, ot);

      //Timeinfo:
      std::string start = sc.environment().optional_string_param(
         "start", setup.generate_timestring());
      std::string end = sc.environment().optional_string_param(
         "end", "");
      std::string unit = sc.environment().optional_string_param(
         "timeinfo_unit", "rounds");
      double factor = sc.environment().optional_double_param(
         "timeinfo_factor", 1.0);
      setup.set_timeinfo_start(start);
      setup.set_timeinfo_end(end);
      setup.set_timeinfo_unit(unit);
      setup.set_timeinfo_factor(factor);


      //Interpolation:
      setup.set_interpolation(sc.environment().optional_string_param(
         "interpolation", ""));

      //Description:
      std::string desc = sc.environment().optional_string_param(
         "description", "");
      int first_ul = desc.find_first_of('_');
      while(first_ul != std::string::npos)
      {
         desc = desc.replace(first_ul, 1, " ");
         first_ul = desc.find_first_of('_');
      }
      setup.set_description(desc);

      setup.gather_topology();
   }
   
   
   // ----------------------------------------------------------------------
   WisemlDataKeeper* WisemlSetupTask::data_keeper(SimulationController &sc)
   {
      WisemlDataKeeper *keeper = sc.keeper_by_name_w<WisemlDataKeeper>(
         "wiseml_data_keeper");
      if(keeper == NULL)
      {
         sc.add_keeper(keeper = new WisemlDataKeeper(sc));
      }

      return keeper;
   }
}
#endif
