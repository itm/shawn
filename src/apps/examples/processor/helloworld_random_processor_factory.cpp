/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_EXAMPLES

#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/examples/processor/helloworld_random_processor_factory.h"
#include "apps/examples/processor/helloworld_random_processor.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace helloworld
{

   void
   HelloworldRandomProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new HelloworldRandomProcessorFactory(sc) );
   }
   // ----------------------------------------------------------------------
   HelloworldRandomProcessorFactory::
   HelloworldRandomProcessorFactory(SimulationController& sc)
   {
		sc_ = &sc;
      //cout << "HelloworldProcessorFactory ctor" << &auto_reg_ << endl;
   }
   // ----------------------------------------------------------------------
   HelloworldRandomProcessorFactory::
   ~HelloworldRandomProcessorFactory()
   {
      //cout << "HelloworldProcessorFactory dtor" << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   HelloworldRandomProcessorFactory::
   name( void )
      const throw()
   { return "helloworld_random"; }
   // ----------------------------------------------------------------------
   std::string 
   HelloworldRandomProcessorFactory::
   description( void )
      const throw()
   {
      return "simple HelloWorld demo processor with a random start up";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   HelloworldRandomProcessorFactory::
   create( void )
      throw()
   {
	double lower = sc_->environment().optional_double_param( "lowerBound",0);
	double upper = sc_->environment().optional_double_param( "upperBound",6);
		return new HelloworldRandomProcessor(lower,upper);
   }
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/
