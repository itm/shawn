/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __WISEML_SENSOR_FACTORY_DOUBLE_H
#define __WISEML_SENSOR_FACTORY_DOUBLE_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/wiseml/sensors/wiseml_sensor_factory.h"
#include "apps/wiseml/sensors/wiseml_double_sensor.h"
#include "sys/node.h"

namespace wiseml
{
   /**
    * Factory for creating and managing double sensors.
    */
   class WisemlDoubleSensorFactory
         : public WisemlSensorFactory
   {
   public:
      WisemlDoubleSensorFactory( void );
      virtual ~WisemlDoubleSensorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual WisemlDoubleSensor* create(string capability, shawn::Node &node);
   };
   DECLARE_HANDLES(WisemlDoubleSensorFactory);
}
#endif
#endif
#endif
