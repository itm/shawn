/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/metrics/one_hop_metrics_processor_factory.h"
#include "apps/wiselib/metrics/one_hop_metrics_processor.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace wiselib
{

   // ----------------------------------------------------------------------
   void
   OneHopMetricsProcessorFactory::
   register_factory( SimulationController& sc )
   throw()
   {
      sc.processor_keeper_w().add( new OneHopMetricsProcessorFactory );
   }
   // ----------------------------------------------------------------------
   OneHopMetricsProcessorFactory::
   OneHopMetricsProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   OneHopMetricsProcessorFactory::
   ~OneHopMetricsProcessorFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   OneHopMetricsProcessorFactory::
   name( void )
   const throw()
   {
      return "one_hop_metrics";
   }
   // ----------------------------------------------------------------------
   std::string
   OneHopMetricsProcessorFactory::
   description( void )
   const throw()
   {
      return "Running Wiselib Metrics algorithms.";
   }
   // ----------------------------------------------------------------------
   shawn::Processor*
   OneHopMetricsProcessorFactory::
   create( void )
   throw()
   {
      return new OneHopMetricsProcessor;
   }
}

#endif
