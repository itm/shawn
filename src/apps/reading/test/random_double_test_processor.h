/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_READING_DOUBLE_TEST_PROCESSOR_H__
#define __SHAWN_APPS_READING_DOUBLE_TEST_PROCESSOR_H__
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING
#ifdef ENABLE_EXAMPLES

#include "apps/examples/processor/helloworld_random_processor.h"
#include "apps/reading/sensors/simple_sensors.h"
#include "sys/event_scheduler.h"
#include "sys/processor.h"
#include <set>

namespace reading
{
   class DoubleReadingTestProcessor
      : public helloworld::HelloworldRandomProcessor
   {
   public:
      DoubleReadingTestProcessor();
      virtual ~DoubleReadingTestProcessor();

      void attach_reading( const DoubleReading &reading );

      virtual void work( void ) throw();

   protected:
      DoubleSensor *sensor_;
   };
}

#endif
#endif
#endif
