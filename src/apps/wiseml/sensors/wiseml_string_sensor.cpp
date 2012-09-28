/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/sensors/wiseml_string_sensor.h"
#ifdef ENABLE_READING
#ifdef ENABLE_WISEML

namespace wiseml
{
   // ----------------------------------------------------------------------
   string
   WisemlStringSensor::
   value() const
   {
      // As RawSensors already contain string values, just return them:
      return raw_sensor_->raw_value();
   }
   // ----------------------------------------------------------------------
   WisemlStringSensor::
   WisemlStringSensor(WisemlRawSensor* raw_sensor)
   :WisemlSensor(raw_sensor)
   {
      
   }
   // ----------------------------------------------------------------------
}

#endif
#endif
