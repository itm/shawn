/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#ifdef ENABLE_EXAMPLES

#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/examples/processors/vis_energy_processor_factory.h"
#include "apps/vis/examples/processors/vis_energy_processor.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace vis
{

   void
   VisEnergyProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new VisEnergyProcessorFactory(sc) );
   }
   // ----------------------------------------------------------------------
   VisEnergyProcessorFactory::
   VisEnergyProcessorFactory(SimulationController& sc)
   {
		sc_ = &sc;
      //cout << "HelloworldProcessorFactory ctor" << &auto_reg_ << endl;
   }
   // ----------------------------------------------------------------------
   VisEnergyProcessorFactory::
   ~VisEnergyProcessorFactory()
   {
      //cout << "HelloworldProcessorFactory dtor" << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   VisEnergyProcessorFactory::
   name( void )
      const throw()
   { return "vis_energy"; }
   // ----------------------------------------------------------------------
   std::string 
   VisEnergyProcessorFactory::
   description( void )
      const throw()
   {
      return "simple HelloWorld demo processor with a random start up";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   VisEnergyProcessorFactory::
   create( void )
      throw()
   {
	double lower = sc_->environment().optional_double_param( "lowerBound",0);
	double upper = sc_->environment().optional_double_param( "upperBound",6);
		return new VisEnergyProcessor(lower,upper);
   }
}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/
