/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/tasks/wiseml_writer_test.h"
#ifdef ENABLE_WISEML
#include <iostream>
#include "apps/wiseml/writer/wiseml_data_keeper.h"
namespace wiseml
{
   WisemlWriterTest::WisemlWriterTest()
   {
   }
   // ----------------------------------------------------------------------
   WisemlWriterTest::~WisemlWriterTest()
   {
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlWriterTest::name() const throw()
   {
      return "wiseml_writer_test";
   }
   // ----------------------------------------------------------------------
   std::string WisemlWriterTest::description() const throw()
   {
      return "A task for generating WiseML files of the current topology.";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void WisemlWriterTest::run(SimulationController &sc) throw()
   {
      shawn::World::const_node_iterator nit = sc.world().begin_nodes();
      std::string trace_xml = "";

      WisemlDataKeeper *keeper = 
         sc.keeper_by_name_w<WisemlDataKeeper>("wiseml_data_keeper");

      WisemlTraceCollector *trace = 
         keeper->add_trace("test:trace:1");
      WisemlScenarioCollector *scenario = 
         keeper->add_scenario("test:scenario:1");
      WisemlScenarioCollector *scenario2 = 
         keeper->add_scenario("test:scenario:2");
      WisemlScenarioCollector *scenario3 = 
         keeper->add_scenario("test:scenario:3");

      scenario->enable_link(nit->label(), (nit+1)->label());
      scenario->disable_link((nit+2)->label(), (nit+6)->label());
      scenario->enable_node((nit+10)->label());
      scenario->disable_node((nit+15)->label());
      scenario->node_movement((nit+7)->label());
      scenario->capability_value((nit+12)->label(), "battery", "19");

      scenario2->enable_link(nit->label(), (nit+1)->label());
      scenario2->disable_link((nit+2)->label(), (nit+6)->label());
      scenario2->enable_node((nit+10)->label());
      scenario2->disable_node((nit+15)->label());
      scenario2->node_movement((nit+7)->label());
      scenario2->capability_value((nit+12)->label(), "battery", "19");


      trace->node_movement(nit->label());
      trace->capability_value(nit->label(), "battery", "65");
      ++nit;
      trace->capability_value(nit->label(), "battery", "40");
      ++nit;
      trace->node_movement(nit->label());


      std::string filename = sc.environment().optional_string_param(
         "filename", "simulation.wiseml");
      std::ofstream file;
      file.open(filename.c_str());
      file << keeper->generate_xml();
      file.close();
   }
}
#endif
