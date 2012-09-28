/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_EXAMPLE_TESTBEDSERVICE_PROCESSOR_H
#define __SHAWN_APPS_EXAMPLE_TESTBEDSERVICE_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/processor/testbedservice_processor.h"
#include "sys/processor.h"

namespace testbedservice
{

   /**
    */
   class ExampleTestbedServiceProcessor
      : public shawn::Processor,
         public TestbedServiceProcessor
   {
   public:

      ///@name Constructor/Destructor
      ///@{
      ExampleTestbedServiceProcessor();
      virtual ~ExampleTestbedServiceProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      virtual void boot() throw();
      ///@}

      ///@name Message Reception
      ///@{
      virtual void process_message( TestbedserviceMessage& message ) throw();
      ///@}

   private:
      // ...
   };


}

#endif
#endif
