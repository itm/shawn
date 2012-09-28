/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_RAW_SENSOR_H
#define __SHAWN_APPS_WISEML_RAW_SENSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include <string>
//using namespace shawn;
using namespace std;
namespace wiseml
{
   /** \brief Sensor helper class
    * Internally used by WiseML sensors.
    */
   class WisemlRawSensor
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      WisemlRawSensor();
      virtual ~WisemlRawSensor();
      ///@}

      /**
       * Sets the raw value of the sensor
       */
      virtual void set_raw_value( string value );
      /**
       * Returns the raw value
       */
      virtual string raw_value();

   protected:
      string value_;
   };
}

#endif
#endif
