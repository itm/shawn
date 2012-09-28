/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_writer_task.h"
#ifdef ENABLE_WISEML
#include <iostream>
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlWriterTask::WisemlWriterTask()
   {
   }
   // ----------------------------------------------------------------------
   WisemlWriterTask::~WisemlWriterTask()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlWriterTask::name() const throw()
   {
      return "wiseml_writer";
   }
   // ----------------------------------------------------------------------
   std::string WisemlWriterTask::description() const throw()
   {
      return "A task for generating WiseML files of the current topology.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlWriterTask::run(SimulationController &sc) throw()
   {
      std::string filename = sc.environment().optional_string_param("filename", "simulation.wiseml");
      std::ofstream file;
      file.open(filename.c_str());

      std::string wml;
      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");
      WisemlSetupCollector &setup = keeper->setup();
      setup.set_timeinfo_duration(sc.world().current_time());
      wml = keeper->generate_xml();
      file << wml;
      file.close();
   }
}
#endif
