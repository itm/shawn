/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <iostream>

#include "sys/processors/processor_keeper.h"
#include "apps/localization/localization_processor_factory.h"
#include "apps/localization/localization_processor.h"
#include "sys/simulation/simulation_controller.h"


using namespace shawn;

namespace localization
{

   void
   LocalizationProcessorFactory::
   register_factory( SimulationController& sc )
      throw()
   {
      sc.processor_keeper_w().add( new LocalizationProcessorFactory(false) );
      sc.processor_keeper_w().add( new LocalizationProcessorFactory(true) );
   }
   // ----------------------------------------------------------------------
   LocalizationProcessorFactory::
   LocalizationProcessorFactory( bool anchor )
      : create_anchors_( anchor )
   {}
   // ----------------------------------------------------------------------
   LocalizationProcessorFactory::
   ~LocalizationProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   LocalizationProcessorFactory::
   name( void )
      const throw()
   {
      if( create_anchors_ )
         return "localization_anchor";
      else
         return "localization";
   }
   // ----------------------------------------------------------------------
   std::string
   LocalizationProcessorFactory::
   description( void )
      const throw()
   {
      if( create_anchors_ )
         return "localization_anchor";
      else
         return "localization";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   LocalizationProcessorFactory::
   create( void )
      throw()
   {
      LocalizationProcessor* lp = new LocalizationProcessor;
	  if( create_anchors_ ){
         lp->set_proc_type( LocalizationProcessor::anchor );
	  }
      return lp;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_processor_factory.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/
