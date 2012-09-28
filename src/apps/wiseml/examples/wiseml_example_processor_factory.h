/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_WISEML_EXAMPLE_PROCESSOR_FACTORY_H
#define __SHAWN_WISEML_EXAMPLE_PROCESSOR_FACTORY_H
#include "_apps_enable_cmake.h"

#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES

#include "sys/processors/processor_factory.h"

namespace shawn 
{ 
    class SimulationController; 
    class Processor;
}

namespace wiseml
{
   /**
    * A factory for creating WiseML example processors.
    */
   class WisemlExampleProcessorFactory
      : public shawn::ProcessorFactory
   {
   public:
      WisemlExampleProcessorFactory();
      virtual ~WisemlExampleProcessorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual shawn::Processor* create( void ) throw();

      static void register_factory( shawn::SimulationController& ) throw();
   };

}

#endif
#endif
#endif
