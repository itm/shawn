/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "apps/autocast/autocast/autocast_processor_factory.h"
#include "apps/autocast/autocast/autocast_processor.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn;

namespace autocast
{

   const std::string autocast::AutoCastProcessorFactory::PROC_NAME = "AutoCast";

   void
   AutoCastProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
	  std::cout << "AutoCastProcessorFactory registered" << std::endl;
      sc.processor_keeper_w().add( new AutoCastProcessorFactory );
   }
   // ----------------------------------------------------------------------
   AutoCastProcessorFactory::
   AutoCastProcessorFactory()
   {
	   std::cout << "AutoCastProcessorFactory ctor" << std::endl;
   }
   // ----------------------------------------------------------------------
   AutoCastProcessorFactory::
   ~AutoCastProcessorFactory()
   {
	   std::cout << "AutoCastProcessorFactory dtor" << std::endl;
   }
   // ----------------------------------------------------------------------
   std::string
   AutoCastProcessorFactory::
   name( void )
      const throw()
   { return autocast::AutoCastProcessorFactory::PROC_NAME; }
   // ----------------------------------------------------------------------
   std::string 
   AutoCastProcessorFactory::
   description( void )
      const throw()
   {
      return "AutoCast processor";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   AutoCastProcessorFactory::
   create( void )
      throw()
   {
      return new AutoCastProcessor;
   }


}

#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
