/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "apps/reading/test/random_double_test_processor_factory.h"
#include "apps/reading/test/random_double_test_processor.h"
#include "sys/simulation/simulation_controller.h"


using namespace shawn;

namespace reading
{

   void
   RandomDoubleTestProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new RandomDoubleTestProcessorFactory() );
   }
   // ----------------------------------------------------------------------
   RandomDoubleTestProcessorFactory::
   RandomDoubleTestProcessorFactory( )
   {}
   // ----------------------------------------------------------------------
   RandomDoubleTestProcessorFactory::
   ~RandomDoubleTestProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   RandomDoubleTestProcessorFactory::
   name( void )
      const throw()
   {
      return "reading_random_double";
   }
   // ----------------------------------------------------------------------
   std::string
   RandomDoubleTestProcessorFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   RandomDoubleTestProcessorFactory::
   create( void )
      throw()
   {
      #ifdef ENABLE_EXAMPLES
      DoubleReadingTestProcessor* rp = new DoubleReadingTestProcessor;
      return rp;
      #endif
      return NULL;
   }

}// namespace reading
#endif
