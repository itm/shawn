/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_READING_SIMPLE_SENSORS_H
#define __SHAWN_APPS_READING_SIMPLE_SENSORS_H
#include "_apps_enable_cmake.h"

#ifdef	ENABLE_READING
#include "apps/reading/readings/simple_reading.h"
#include "apps/reading/sensors/sensor.h"
#include "apps/reading/sensors/reading_sensor.h"
#include "sys/node.h"

namespace reading
{
   class DoubleSensor
      :public ReadingSensor
   {
   public:
      DoubleSensor(const DoubleReading &rdg, const shawn::Node &node);
      virtual ~DoubleSensor();

      virtual double value();
   };

   class IntegerSensor
      :public ReadingSensor
   {
   public:
      IntegerSensor(const IntegerReading &rdg, const shawn::Node &node);
      virtual ~IntegerSensor();

      virtual int value();
   };
}
#endif
#endif // __SHAWN_APPS_READING_SIMPLE_SENSORS_H
