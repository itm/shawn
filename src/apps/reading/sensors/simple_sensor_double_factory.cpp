/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/sensors/simple_sensor_double_factory.h"

namespace reading
{
   SimpleSensorDoubleFactory::
   SimpleSensorDoubleFactory( void )
   {}
   // ----------------------------------------------------------------------
   SimpleSensorDoubleFactory::
   ~SimpleSensorDoubleFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimpleSensorDoubleFactory::
   name( void )
      const throw()
   {
      return "simple_sensor_double";
   }
   // ----------------------------------------------------------------------
   std::string
   SimpleSensorDoubleFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   Sensor*
   SimpleSensorDoubleFactory::
   find_sensor( std::string name )
   {
      SensorMap::const_iterator it = sensor_map_.find(name);
         return it==sensor_map_.end()
            ? NULL
            : it->second;
   }
   // ----------------------------------------------------------------------
   DoubleSensor*
   SimpleSensorDoubleFactory::
   create(DoubleReading &reading, shawn::Node node)
   {
      std::string sensor_id = reading.name();
      sensor_id += "_";
      sensor_id += node.label();

      Sensor* tmp_sens = find_sensor(sensor_id);
      DoubleSensor* out_sensor;
      if(tmp_sens == NULL)
      {
         DoubleSensor* out_sensor = new DoubleSensor(reading, node);
         sensor_map_[sensor_id] = out_sensor;
      }
      else
      {
         out_sensor = dynamic_cast<DoubleSensor *>(tmp_sens);
      }


      return out_sensor;
   }
}

#endif

