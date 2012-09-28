/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SENSOR_DOUBLE_H
#define __SHAWN_APPS_WISEML_SENSOR_DOUBLE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING
#ifdef ENABLE_WISEML
#include "sys/node.h"
#include "apps/reading/sensors/sensor.h"
#include "apps/wiseml/sensors/wiseml_sensor.h"
#include "apps/wiseml/sensors/wiseml_raw_sensor.h"
#include <stdlib.h>

using namespace shawn;
using namespace std;
namespace wiseml
{
   /** \brief Double Sensor
    * Reads out capabilities as double values.
    */
   class WisemlDoubleSensor:
      public WisemlSensor
   {
   public:
      WisemlDoubleSensor(WisemlRawSensor* raw_sensor);
      virtual double value() const;
   };
}

#endif
#endif
#endif
