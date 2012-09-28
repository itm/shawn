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

#ifndef __SHAWN_LEGACYAPPS_AUTOCASTAPP_PROCESSOR_FACTORY_H
#define __SHAWN_LEGACYAPPS_AUTOCASTAPP_PROCESSOR_FACTORY_H

#include "_apps_enable_cmake.h"
//#include "config/helloworld.h"
#ifdef ENABLE_AUTOCAST

#include "sys/processors/processor_factory.h"

namespace shawn 
{ 
    class SimulationController; 
    class Processor;
}


namespace autocast
{

   class AutoCastAppProcessorFactory
      : public shawn::ProcessorFactory
   {
   public:
      AutoCastAppProcessorFactory();
      virtual ~AutoCastAppProcessorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual shawn::Processor* create( void ) throw();

      static void register_factory( shawn::SimulationController& ) throw();
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
