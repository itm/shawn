/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __READING_SENSOR_FACTORY_H
#define __READING_SENSOR_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_READING
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/sensors/sensor.h"

namespace reading
{
   class SensorFactory
         : public shawn::KeeperManaged
   {
   public:
      typedef std::map<std::string,Sensor*> SensorMap;
      SensorFactory( void );
      virtual ~SensorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual Sensor* find_sensor( std::string name );
      SensorMap sensor_map_;
   };
   DECLARE_HANDLES(SensorFactory);
}
#endif
#endif

