/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/sensors/sensor_factory.h"

namespace reading
{
   SensorFactory::
   SensorFactory( void )
   {}
   // ----------------------------------------------------------------------
   SensorFactory::
   ~SensorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   SensorFactory::
   name( void )
      const throw()
   {
      return "sensor";
   }
   // ----------------------------------------------------------------------
   std::string
   SensorFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   Sensor*
   SensorFactory::
   find_sensor( std::string name )
   {
      SensorMap::const_iterator it = sensor_map_.find(name);
         return it==sensor_map_.end()
            ? NULL
            : it->second;
   }

}

#endif

