/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#ifdef ENABLE_EXAMPLES

#include "apps/wiseml/examples/wiseml_example_processor_factory.h"
#include "apps/wiseml/examples/wiseml_example_processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace wiseml
{

   // ----------------------------------------------------------------------
   void
   WisemlExampleProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new WisemlExampleProcessorFactory );
   }
   
   // ----------------------------------------------------------------------
   WisemlExampleProcessorFactory::
   WisemlExampleProcessorFactory()
   {
      //cout << "WisemlExampleProcessorFactory ctor" << &auto_reg_ << endl;
   }
   
   // ----------------------------------------------------------------------
   WisemlExampleProcessorFactory::
   ~WisemlExampleProcessorFactory()
   {
      //cout << "WisemlExampleProcessorFactory dtor" << endl;
   }
   
   // ----------------------------------------------------------------------
   std::string
   WisemlExampleProcessorFactory::
   name( void )
      const throw()
   { 
	   return "wiseml_example"; 
   }
   
   // ----------------------------------------------------------------------
   std::string 
   WisemlExampleProcessorFactory::
   description( void )
      const throw()
   {
      return "WiseML sensor demo processor";
   }
   
   // ----------------------------------------------------------------------
   shawn::Processor*
   WisemlExampleProcessorFactory::
   create( void )
      throw()
   {
      return new WisemlExampleProcessor;
   }

}

#endif
#endif
