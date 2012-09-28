/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/metrics/individual_link_metrics_processor_factory.h"
#include "apps/wiselib/metrics/individual_link_metrics_processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace wiselib
{

   // ----------------------------------------------------------------------
   void
   IndividualLinkMetricsProcessorFactory::
   register_factory( SimulationController& sc )
   throw()
   {
      sc.processor_keeper_w().add( new IndividualLinkMetricsProcessorFactory );
   }
   // ----------------------------------------------------------------------
   IndividualLinkMetricsProcessorFactory::
   IndividualLinkMetricsProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   IndividualLinkMetricsProcessorFactory::
   ~IndividualLinkMetricsProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   IndividualLinkMetricsProcessorFactory::
   name( void )
   const throw()
   {
      return "individual_link_metrics";
   }
   // ----------------------------------------------------------------------
   std::string
   IndividualLinkMetricsProcessorFactory::
   description( void )
   const throw()
   {
      return "Running Wiselib Metrics algorithms.";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   IndividualLinkMetricsProcessorFactory::
   create( void )
   throw()
   {
      return new IndividualLinkMetricsProcessor;
   }
}

#endif
