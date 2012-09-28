/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/sensors/wiseml_raw_sensor.h"
#ifdef ENABLE_READING
#ifdef ENABLE_WISEML

namespace wiseml
{
   WisemlRawSensor::
   WisemlRawSensor()
   :  value_("")
   {}
   // ----------------------------------------------------------------------
   WisemlRawSensor::
   ~WisemlRawSensor()
   {}
   // ----------------------------------------------------------------------
   void
   WisemlRawSensor::
   set_raw_value( string value )
   {
      value_ = value;
   }
   // ----------------------------------------------------------------------
   string
   WisemlRawSensor::
   raw_value( )
   {
      return value_;
   }
}

#endif
#endif
