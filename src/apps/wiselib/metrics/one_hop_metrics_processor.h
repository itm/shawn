/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISELIB_METRICS_PROCESSOR_H
#define __SHAWN_APPS_WISELIB_METRICS_PROCESSOR_H

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
#include "algorithms/metrics/one_hop_link/one_hop_link_metrics.h"
#include "util/pstl/vector_static.h"
#include <vector>

typedef wiselib::ShawnOsModel Os;
typedef wiselib::ShawnClockModel<Os> ShawnClock;
// typedef wiselib::vector_static<Os, wiselib::OneHopLinkMetricsDataItem<
//    Os::Radio::node_id_t>, 10> MetricsDataItemContainer;
typedef std::vector<wiselib::OneHopLinkMetricsDataItem<Os::Radio::node_id_t> >
   MetricsDataItemContainer;

typedef wiselib::OneHopLinkMetrics<Os, Os::Radio, Os::Timer, ShawnClock,
   MetricsDataItemContainer, Os::Debug> OneHopMetrics;


namespace wiselib
{

   /**
    */
   class OneHopMetricsProcessor
       : public virtual ExtIfaceProcessor
   {
   public:

      ///@name Constructor/Destructor
      ///@{
      OneHopMetricsProcessor();
      virtual ~OneHopMetricsProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      /**
       */
      virtual void boot( void ) throw();
      ///@}

      void protocol_loop( void* userdata ) throw();
      void start_protocol_broadcast( void* userdata ) throw();
      void evaluation( void* userdata ) throw();

   private:

      int messages_;
      int message_size_;
      int sink_;
      int transmit_interval_;
      int delay_;

      OneHopMetrics one_hop_metrics_;
      ShawnOs os_;
      Os::Radio wiselib_radio_;
      Os::Timer wiselib_timer_;
      ShawnClock wiselib_clock_;
      Os::Debug wiselib_debug_;
   };


}

#endif
#endif
