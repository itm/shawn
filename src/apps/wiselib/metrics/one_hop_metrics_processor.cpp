/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/metrics/one_hop_metrics_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/taggings/basic_tags.h"


namespace wiselib
{

   OneHopMetricsProcessor::
   OneHopMetricsProcessor()
      : messages_           ( 10 ),
         message_size_      ( 10 ),
         sink_              ( -1 ),
         transmit_interval_ ( 100 ),
         delay_             ( 100 ),
         wiselib_radio_( os_ ),
         wiselib_timer_( os_ ),
         wiselib_clock_( os_ ),
         wiselib_debug_( os_ )
   {}
   // ----------------------------------------------------------------------
   OneHopMetricsProcessor::
   ~OneHopMetricsProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   OneHopMetricsProcessor::
   boot( void )
      throw()
   {
      INFO( logger(), "OneHopMetricsProcessor::booting" );

      os_.proc = this;
      one_hop_metrics_.init( wiselib_radio_, wiselib_timer_, wiselib_clock_, wiselib_debug_ );

      const shawn::SimulationEnvironment& se = owner().world().
                                       simulation_controller().environment();
      messages_ = se.optional_int_param( "messages", messages_ );
      message_size_ = se.optional_int_param( "message_size", message_size_ );
      sink_ = se.optional_int_param( "sink", sink_ );
      transmit_interval_ = se.optional_int_param( "transmit_interval", transmit_interval_ );
      delay_ = se.optional_int_param( "delay", delay_ );

      if( owner().id() == sink_ )
      {
         wiselib_timer_.set_timer<OneHopMetricsProcessor,
                              &OneHopMetricsProcessor::start_protocol_broadcast>(
            delay_, this, 0 );

         wiselib_timer_.set_timer<OneHopMetricsProcessor,
                              &OneHopMetricsProcessor::evaluation>(
            transmit_interval_ * messages_ + 3000, this, 0 );

         wiselib_timer_.set_timer<OneHopMetricsProcessor,
                              &OneHopMetricsProcessor::protocol_loop>(
            transmit_interval_ * messages_ + 15000, this, 0 );
      }
   }
   // ----------------------------------------------------------------------
   void
   OneHopMetricsProcessor::
   protocol_loop( void* userdata )
      throw()
   {
      wiselib_timer_.set_timer<OneHopMetricsProcessor,
                              &OneHopMetricsProcessor::start_protocol_broadcast>(
            delay_, this, 0 );

      wiselib_timer_.set_timer<OneHopMetricsProcessor,
                           &OneHopMetricsProcessor::evaluation>(
         transmit_interval_ * messages_ + 3000, this, 0 );

      wiselib_timer_.set_timer<OneHopMetricsProcessor,
                              &OneHopMetricsProcessor::protocol_loop>(
            transmit_interval_ * messages_ + 15000, this, 0 );
   }
   // ----------------------------------------------------------------------
   void
   OneHopMetricsProcessor::
   evaluation( void* userdata )
      throw()
   {
      INFO( logger(), "Packet Stats:" );

      OneHopMetrics::MetricsData& md = one_hop_metrics_.metrics_data();

      int cnt = 0;
      for ( OneHopMetrics::MetricsDataContainerIterator
               it = md.link_metrics.begin();
               it != md.link_metrics.end();
               ++it )
      {
         INFO( logger(), "Received "
            << (*it).packets_received
            << " of " << messages_
            << " packets from " << (*it).id );
         cnt++;
      }

      int nodes = md.link_metrics.size();
      INFO( logger(), "Total packets Received: " << md.total_received << " of " << nodes * messages_ );
      INFO( logger(), "Start time: " << md.start_time );
      INFO( logger(), "Stop  time: " << md.stop_time );
      INFO( logger(), "Total time: " << md.stop_time - md.start_time );
      if (cnt > 0)
         INFO( logger(), "Evaluation: duration " << (md.stop_time - md.start_time) << " nodes " << cnt
            << " total-rcvd " << md.total_received << " total-sent " << (nodes * messages_)
            << " fraction " << (double)md.total_received / (double)(nodes * messages_)
            << " msgs " << messages_ << " size " << message_size_ << " transmit_interval " << transmit_interval_ );
   }
   // ----------------------------------------------------------------------
   void
   OneHopMetricsProcessor::
   start_protocol_broadcast( void* userdata )
      throw()
   {
      INFO( logger(), "Sending control packet at " << owner().id() );
      one_hop_metrics_.start( transmit_interval_, messages_, message_size_ );
   }

}
#endif
