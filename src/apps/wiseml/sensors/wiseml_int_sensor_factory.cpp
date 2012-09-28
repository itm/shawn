/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/sensors/wiseml_int_sensor_factory.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_READING

namespace wiseml
{
   WisemlIntSensorFactory::
   WisemlIntSensorFactory( void )
   {}
   // ----------------------------------------------------------------------
   WisemlIntSensorFactory::
   ~WisemlIntSensorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   WisemlIntSensorFactory::
   name( void )
      const throw()
   {
      return "wiseml_int_sensor";
   }
   // ----------------------------------------------------------------------
   std::string
   WisemlIntSensorFactory::
   description( void )
      const throw()
   {
      return "Base class for creating WiseML integer sensors";
   }
   // ----------------------------------------------------------------------
   WisemlIntSensor*
   WisemlIntSensorFactory::
   create(string capability, shawn::Node &node)
   {
      string sensor_id = node.label() + ":" + capability;

      reading::Sensor* sensor = find_sensor(sensor_id);

      WisemlIntSensor* out_sensor = NULL;
      if(sensor == NULL)
      {
         out_sensor = 
            new WisemlIntSensor((*data_reader_)[sensor_id]);
         sensor_map_[sensor_id] = out_sensor;
      }
      else
      {
         out_sensor = dynamic_cast<WisemlIntSensor*>(sensor);
      }

      return out_sensor;
   }
}
#endif
#endif
