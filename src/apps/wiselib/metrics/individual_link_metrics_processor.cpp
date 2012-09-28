/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/metrics/individual_link_metrics_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/taggings/basic_tags.h"

#include "boost/date_time/posix_time/posix_time.hpp"

namespace wiselib
{

   IndividualLinkMetricsProcessor::
   IndividualLinkMetricsProcessor()
      : sender_             ( -1 ),
         receiver_          ( -1 ),
         transmit_interval_ ( 100 ),
         messages_          ( 100 ),
         delay_             ( 100 ),
         wiselib_radio_( os_ ),
         wiselib_timer_( os_ ),
         wiselib_clock_( os_ ),
         wiselib_debug_( os_ )
   {}
   // ----------------------------------------------------------------------
   IndividualLinkMetricsProcessor::
   ~IndividualLinkMetricsProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   IndividualLinkMetricsProcessor::
   boot( void )
      throw()
   {
      INFO( logger(), "IndividualLinkMetricsProcessor::booting" );

      os_.proc = this;
      individual_metrics_.init( wiselib_radio_, wiselib_timer_, wiselib_clock_, wiselib_debug_ );

      const shawn::SimulationEnvironment& se = owner().world().
                                       simulation_controller().environment();
      sender_ = se.optional_int_param( "sender", sender_ );
      receiver_ = se.optional_int_param( "receiver", receiver_ );
      transmit_interval_ = se.optional_int_param( "transmit_interval", transmit_interval_ );
      messages_ = se.optional_int_param( "messages", messages_ );
      delay_ = se.optional_int_param( "delay", delay_ );

//       if( owner().id() == sender_ )
//       {
//          boost::posix_time::ptime now1 =
//                boost::posix_time::microsec_clock::local_time();
//          INFO( logger(), "#start at " << owner().id() << " at time " << now1 );
// 
//          for ( int i = 0; i < messages_; i ++ )
//          {
//             uint8_t buffer[80];
//             wiselib_radio_.send( receiver_, sizeof(buffer), buffer );
// 
// //             INFO( logger(), "#send at " << owner().id() << " at time " << owner().world().current_time() );
//          }
//          boost::posix_time::ptime now2 =
//                boost::posix_time::microsec_clock::local_time();
//          INFO( logger(), "#fin at " << owner().id() << " at time " << now2 );
// 
//          INFO( logger(), "#duration " << now2 - now1 );
//       }

// Following was just for a quick test at TUBS - with real node ids 401..430
//      receiver_ = owner().id() + 400;
//      if( receiver_ >= 401 && receiver_ <= 430 )

      if( owner().id() == sender_ )
      {
         wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                              &IndividualLinkMetricsProcessor::run>(
            delay_, this, 0 );

         wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                              &IndividualLinkMetricsProcessor::evaluation>(
            transmit_interval_ * messages_ + 3000, this, 0 );
      }
   }
   // ----------------------------------------------------------------------
   void
   IndividualLinkMetricsProcessor::
   run( void* userdata )
      throw()
   {
      INFO( logger(), "Sending control packet at " << owner().id() << " with " << receiver_ );
      individual_metrics_.start( receiver_, transmit_interval_, messages_ );
   }
   // ----------------------------------------------------------------------
   void
   IndividualLinkMetricsProcessor::
   evaluation( void* userdata )
      throw()
   {
      ShawnIndividualLinkMetrics::MetricsData& md = individual_metrics_.metrics_data();
      if ( md.lost == md.total_sent )
      {
         wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                              &IndividualLinkMetricsProcessor::run>(
            delay_ + 3000, this, 0 );

         wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                              &IndividualLinkMetricsProcessor::evaluation>(
            transmit_interval_ * messages_ + 15000, this, 0 );

         return;
      }

//       INFO( logger(), "Individual Link Stats:" );
//       INFO( logger(), "  Data Items  :" );
      double min = 0xffffffff, max = 0, avg = 0;
      int cnt = 0;
      for ( ShawnIndividualLinkMetrics::DataItemContainerIterator
               it = md.link_times.begin();
               it != md.link_times.end();
               ++it )
      {
//          INFO( logger(), "    -> Item " << cnt++ << ": " << *it );

         if ( *it > max )
            max = *it;
         if ( *it < min )
            min = *it;
         avg += *it;
      }
      avg /= md.link_times.size();

//       INFO( logger(), "  Total sent  : " << md.total_sent );
//       INFO( logger(), "  Lost        : " << md.lost );
//       INFO( logger(), "  Min duration: " << min * 1000.0f << " ms" );
//       INFO( logger(), "  Max duration: " << max * 1000.0f << " ms" );
//       INFO( logger(), "  Avg duration: " << avg * 1000.0f << " ms" );
INFO( logger(), "evaluation at " << owner().id()
   << " min " << (min * 1000.0f)
   << " max " << (max * 1000.0f)
   << " avg " << (avg * 1000.0f)
   << " lost " << md.lost
   << " sent " << md.total_sent );

      wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                           &IndividualLinkMetricsProcessor::run>(
         delay_ + 3000, this, 0 );

      wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                           &IndividualLinkMetricsProcessor::evaluation>(
         transmit_interval_ * messages_ + 15000, this, 0 );
   }
   // ----------------------------------------------------------------------
   void
   IndividualLinkMetricsProcessor::
   fire( void* userdata )
      throw()
   {
      cnt_++;
      if ( cnt_ < messages_ )
      {
         wiselib_timer_.set_timer<IndividualLinkMetricsProcessor,
                                 &IndividualLinkMetricsProcessor::fire>(
               10, this, 0 );
      }
      else
         INFO( logger(), "#fin at " << owner().id() << " at time " << owner().world().current_time() );
   }

}
#endif
