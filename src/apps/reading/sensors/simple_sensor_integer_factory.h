/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SIMPLE_READING_SENSOR_INTEGER_FACTORY_H
#define __SIMPLE_READING_SENSOR_INTEGER_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_READING
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/sensors/sensor_factory.h"
#include "apps/reading/sensors/simple_sensors.h"
#include "sys/node.h"

namespace reading
{
   class SimpleSensorIntegerFactory
         : public SensorFactory
   {
   public:
      typedef std::map<std::string,Sensor*> SensorMap;
      SimpleSensorIntegerFactory( void );
      virtual ~SimpleSensorIntegerFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      IntegerSensor* create(IntegerReading &reading, shawn::Node node);

   protected:
      Sensor* find_sensor( std::string name );
      SensorMap sensor_map_;
   };
   DECLARE_HANDLES(SimpleSensorIntegerFactory);
}
#endif
#endif
