/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/sensors/simple_sensor_integer_factory.h"

namespace reading
{
   SimpleSensorIntegerFactory::
   SimpleSensorIntegerFactory( void )
   {}
   // ----------------------------------------------------------------------
   SimpleSensorIntegerFactory::
   ~SimpleSensorIntegerFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimpleSensorIntegerFactory::
   name( void )
      const throw()
   {
      return "simple_sensor_integer";
   }
   // ----------------------------------------------------------------------
   std::string
   SimpleSensorIntegerFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   Sensor*
   SimpleSensorIntegerFactory::
   find_sensor( std::string name )
   {
      SensorMap::const_iterator it = sensor_map_.find(name);
         return it==sensor_map_.end()
            ? NULL
            : it->second;
   }
   // ----------------------------------------------------------------------
   IntegerSensor*
   SimpleSensorIntegerFactory::
   create(IntegerReading &reading, shawn::Node node)
   {
      std::string sensor_id = reading.name();
      sensor_id += "_";
      sensor_id += node.label();

      Sensor* tmp_sens = find_sensor(sensor_id);
      IntegerSensor* out_sensor;
      if(tmp_sens == NULL)
      {
         IntegerSensor* out_sensor = new IntegerSensor(reading, node);
         sensor_map_[sensor_id] = out_sensor;
      }
      else
      {
         out_sensor = dynamic_cast<IntegerSensor *>(tmp_sens);
      }


      return out_sensor;
   }
}

#endif

