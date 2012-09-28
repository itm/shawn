/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "apps/autocast/autocast_app/autocast_app_processor_factory.h"
#include "apps/autocast/autocast_app/autocast_app_processor.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn;

namespace autocast
{

   void
   AutoCastAppProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      std::cout << "AutoCastAppProcessorFactory registered" << std::endl;
      sc.processor_keeper_w().add( new AutoCastAppProcessorFactory );
   }
   // ----------------------------------------------------------------------
   AutoCastAppProcessorFactory::
   AutoCastAppProcessorFactory()
   {
      cout << "AutoCastAppProcessorFactory ctor" << endl;
   }
   // ----------------------------------------------------------------------
   AutoCastAppProcessorFactory::
   ~AutoCastAppProcessorFactory()
   {
      cout << "AutoCastAppProcessorFactory dtor" << endl;
   }
   // ----------------------------------------------------------------------
   std::string
   AutoCastAppProcessorFactory::
   name( void )
      const throw()
   { return "AutoCastApp"; }
   // ----------------------------------------------------------------------
   std::string 
   AutoCastAppProcessorFactory::
   description( void )
      const throw()
   {
      return "AutoCastApp processor";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   AutoCastAppProcessorFactory::
   create( void )
      throw()
   {
	   return new AutoCastAppProcessor;
   }


}

#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
