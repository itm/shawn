/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISELIB_METRICS_INDIVIDUAL_LINK_PROCESSOR_H
#define __SHAWN_APPS_WISELIB_METRICS_INDIVIDUAL_LINK_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/ext_iface_processor.h"
#include "sys/processor.h"
#include "sys/event_scheduler.h"

#include "external_interface/shawn/shawn_os.h"
#include "external_interface/shawn/shawn_radio.h"
#include "external_interface/shawn/shawn_clock.h"
#include "external_interface/shawn/shawn_timer.h"
#include "external_interface/shawn/shawn_debug.h"
#include "algorithms/metrics/individual_link/individual_link_metrics.h"
#include "util/pstl/vector_static.h"
#include <vector>

typedef wiselib::ShawnOsModel Os;
typedef wiselib::ShawnClockModel<Os> ShawnClock;
typedef wiselib::vector_static<Os, ShawnClock::time_t, 100> MetricsDataItemContainer;
// typedef wiselib::IndividualLinkMetrics<Os, Os::Timer, ShawnClock, MetricsDataItemContainer, Os::Debug, 40> ShawnIndividualLinkMetrics;
typedef wiselib::IndividualLinkMetrics<Os, Os::Radio, Os::Timer, ShawnClock, Os::Debug, MetricsDataItemContainer> ShawnIndividualLinkMetrics;

namespace wiselib
{

   /**
    */
   class IndividualLinkMetricsProcessor
       : public virtual ExtIfaceProcessor
   {
   public:

      ///@name Constructor/Destructor
      ///@{
      IndividualLinkMetricsProcessor();
      virtual ~IndividualLinkMetricsProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      /**
       */
      virtual void boot( void ) throw();
      ///@}

      void run( void* userdata ) throw();
      void evaluation( void* userdata ) throw();

      void fire( void* userdata ) throw();

   private:

      int sender_;
      int receiver_;
      int transmit_interval_;
      int messages_;
      int delay_;

      int cnt_;

      ShawnIndividualLinkMetrics individual_metrics_;
      ShawnOs os_;
      Os::Radio wiselib_radio_;
      Os::Timer wiselib_timer_;
      ShawnClock wiselib_clock_;
      Os::Debug wiselib_debug_;
   };


}

#endif
#endif
