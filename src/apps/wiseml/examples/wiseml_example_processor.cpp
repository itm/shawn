/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/examples/wiseml_example_processor.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_processor.h"
#include "apps/examples/processor/helloworld_message.h"
#include "sys/node.h"
#include "apps/wiseml/sensors/wiseml_string_sensor_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/wiseml/writer/wiseml_data_keeper.h"
#include "apps/wiseml/writer/wiseml_trace_collector.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logging.h"
#include <iostream>


using namespace std;
using namespace shawn;

namespace wiseml
{
   WisemlExampleProcessor::
   WisemlExampleProcessor()
   :  old_value_(""),
      some_sensor_(NULL),
      writer_battery_(100)
   {
   }
   // ----------------------------------------------------------------------
   WisemlExampleProcessor::
   ~WisemlExampleProcessor()
   {}
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::
   boot( void )
      throw()
   {
      last_time_of_receive_ = simulation_round();
      send( new helloworld::HelloworldMessage );
      
      sim_controller_ = &owner_w().world_w().simulation_controller_w();

      some_sensor_ = get_string_sensor("battery");

      if(some_sensor_ != NULL)
         old_value_ = some_sensor_->value();
      
      if(owner().id()==0)
      {
         //Add capability test:
         WisemlDataKeeper *keeper = 
            sim_controller_->keeper_by_name_w<WisemlDataKeeper>(
            "wiseml_data_keeper");
         if(keeper != NULL)
         {
            Capability cap;
            cap.datatype = "double";
            cap.unit = "degree";
            cap.name = "temperature";
            cap.def_value = "22.0";
            keeper->setup().add_capability(owner().label(), cap);
            keeper->setup().set_bool_param("gateway", true, owner().label());
         }

      }


   }
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::
   work( void )
      throw()
   {
      // Wiseml trace writer example:
      bool worked = (shawn::uniform_random_0i_1i() >= 0.5);
      if(worked && writer_battery_ > 0)
      {
         writer_battery_--;
         value_changed();
      }

      // Wiseml sensor example:
      if(some_sensor_ != NULL)
      {
         string cur_value = some_sensor_->value();

         if(cur_value != old_value_)
         {
            old_value_ = cur_value;
            cout << owner().label() << ":Battery = " << cur_value << endl;
         }
      }
   }
   // ----------------------------------------------------------------------
   WisemlStringSensor* 
   WisemlExampleProcessor::get_string_sensor(std::string capability)
   {
      
      sensor_keeper_ = sim_controller_->
         keeper_by_name_w<reading::SensorKeeper>("SensorKeeper");

      try
      {
         reading::SensorFactory *factory = 
            sensor_keeper_->find_w("wiseml_string_sensor").get();

         WisemlStringSensorFactory *string_factory = 
            dynamic_cast<WisemlStringSensorFactory*>(factory);

         WisemlStringSensor* sensor = 
            string_factory->create(capability, owner_w());

         return sensor;
      }
      catch(std::runtime_error e)
      {
		  WARN( logger(), "wiseml_string_sensor not registered?");
      }

      return NULL;
   }
   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessor::value_changed()
   {
      WisemlDataKeeper *keeper = 
         sim_controller_->keeper_by_name_w<WisemlDataKeeper>(
         "wiseml_data_keeper");
      if(keeper != NULL)
      {
         try
         {
            std::stringstream valstr;
            valstr << writer_battery_;
            // Getting the Trace
            WisemlTraceCollector &trace = keeper->trace("example_trace");
            // Committing the value change to the trace
            trace.capability_value(owner().label(), "battery", valstr.str());
         }
         catch(std::runtime_error er)
         {
            WARN( logger(), "WisemlTraceCollector not found!");
         }
      }
   }
   // ----------------------------------------------------------------------
}
#endif
#endif
