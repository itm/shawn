/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/reading/test/random_double_test_processor.h"
#ifdef ENABLE_READING
#ifdef ENABLE_EXAMPLES
#include "apps/reading/sensors/simple_sensors.h"
#include "apps/reading/readings/simple_reading.h"
#include "apps/reading/readings/reading_keeper.h"
#include "sys/node.h"
#include "sys/taggings/basic_tags.h"

namespace reading
{
   DoubleReadingTestProcessor::DoubleReadingTestProcessor()
   : sensor_( NULL )
   {}
   DoubleReadingTestProcessor::~DoubleReadingTestProcessor() {}

   void DoubleReadingTestProcessor::attach_reading( const DoubleReading &reading )
   {
      const shawn::Node &owner = this->owner();

      sensor_ = new DoubleSensor(reading, owner);
   }

   void DoubleReadingTestProcessor::work( void ) throw()
   {
      helloworld::HelloworldRandomProcessor::work();
      shawn::Node &my_owner = this->owner_w();

      if(sensor_)
      {
         my_owner.write_simple_tag<double>("test_sensor", sensor_->value());
      }
   }
}

#endif
#endif
