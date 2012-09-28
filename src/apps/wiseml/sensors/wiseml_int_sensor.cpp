/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/sensors/wiseml_int_sensor.h"
#ifdef ENABLE_READING
#ifdef ENABLE_WISEML

namespace wiseml
{
   // ----------------------------------------------------------------------
   int
   WisemlIntSensor::
   value() const
   {
      // Convert string representation to double:
      return atoi(raw_sensor_->raw_value().c_str());
   }
   // ----------------------------------------------------------------------
   WisemlIntSensor::
   WisemlIntSensor(WisemlRawSensor* raw_sensor)
   :WisemlSensor(raw_sensor)
   {
      
   }
   // ----------------------------------------------------------------------
}

#endif
#endif
