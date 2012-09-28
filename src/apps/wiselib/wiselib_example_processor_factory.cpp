/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/wiselib_example_processor_factory.h"
#include "apps/wiselib/wiselib_example_processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace wiselib
{

   // ----------------------------------------------------------------------
   void
   WiselibExampleProcessorFactory::
   register_factory( SimulationController& sc )
   throw()
   {
      sc.processor_keeper_w().add( new WiselibExampleProcessorFactory );
   }
   // ----------------------------------------------------------------------
   WiselibExampleProcessorFactory::
   WiselibExampleProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   WiselibExampleProcessorFactory::
   ~WiselibExampleProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   WiselibExampleProcessorFactory::
   name( void )
   const throw()
   {
      return "wiselib";
   }
   // ----------------------------------------------------------------------
   std::string
   WiselibExampleProcessorFactory::
   description( void )
   const throw()
   {
      return "Wiselib Example Processor";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   WiselibExampleProcessorFactory::
   create( void )
   throw()
   {
      return new WiselibExampleProcessor;
   }
}

#endif
