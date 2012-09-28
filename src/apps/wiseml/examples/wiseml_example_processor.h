/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_EXAMPLES_PROCESSOR_H
#define __SHAWN_APPS_WISEML_EXAMPLES_PROCESSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_processor.h"
#include "apps/wiseml/sensors/wiseml_string_sensor.h"
#include "apps/reading/sensors/sensor_keeper.h"
#include <set>
#include <string>
using namespace std;

namespace wiseml
{
   /**
    * An example processor which shows how to use WiseML sensors.
    */
   class WisemlExampleProcessor
       : public helloworld::HelloworldProcessor
   {
   public:
      WisemlExampleProcessor();
      virtual ~WisemlExampleProcessor();

      virtual void boot( void ) throw();
      virtual void work( void ) throw();
   protected:
      string old_value_;
      int writer_battery_;
      WisemlStringSensor* some_sensor_;
      SimulationController* sim_controller_;
      reading::SensorKeeper* sensor_keeper_;
      WisemlStringSensor* get_string_sensor(std::string capability); 
      void value_changed();
   };


}

#endif
#endif
#endif
