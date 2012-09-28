/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/processor/example_testbedservice_processor_factory.h"
#include "apps/testbedservice/processor/example_testbedservice_processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>

namespace testbedservice
{

   // ----------------------------------------------------------------------
   void
   ExampleTestbedServiceProcessorFactory::
   register_factory( shawn::SimulationController& sc )
   throw()
   {
      sc.processor_keeper_w().add( new ExampleTestbedServiceProcessorFactory );
   }
   // ----------------------------------------------------------------------
   ExampleTestbedServiceProcessorFactory::
   ExampleTestbedServiceProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   ExampleTestbedServiceProcessorFactory::
   ~ExampleTestbedServiceProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   ExampleTestbedServiceProcessorFactory::
   name( void )
   const throw()
   {
      return "exampletestbedservice";
   }
   // ----------------------------------------------------------------------
   std::string
   ExampleTestbedServiceProcessorFactory::
   description( void )
   const throw()
   {
      return "Example Testbed Service Processor";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   ExampleTestbedServiceProcessorFactory::
   create( void )
   throw()
   {
      return new ExampleTestbedServiceProcessor;
   }
}

#endif
