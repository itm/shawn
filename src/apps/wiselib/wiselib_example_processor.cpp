/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB
#include "apps/wiselib/wiselib_example_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/taggings/basic_tags.h"


namespace wiselib
{
   WiselibExampleProcessor::
   WiselibExampleProcessor()
      : wiselib_radio_ ( os_ ),
         wiselib_timer_( os_ ),
         wiselib_debug_( os_ )
   {}
   // ----------------------------------------------------------------------
   WiselibExampleProcessor::
   ~WiselibExampleProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   WiselibExampleProcessor::
   boot( void )
      throw()
   {
      os_.proc = this;

      const shawn::SimulationEnvironment& se = owner().world().
                                       simulation_controller().environment();
      rt_algo_ = se.optional_string_param( "routing_algorithm", "dsdv" );

//       routing_.set_sink( true );
      routing_.init( wiselib_radio_, wiselib_debug_ );
      routing_.reg_recv_callback<WiselibExampleProcessor,
         &WiselibExampleProcessor::rcv_routing_message>(this);
      routing_.send(1,0,0);

      if (rt_algo_ == "tree")
      {
//          INFO( logger(), "Start tree routing on " << owner().id() );
         if ( owner().id() == 0 )
            tree_routing_.set_sink( true );
         else
            tree_routing_.set_sink( false );
         tree_routing_.init( wiselib_radio_, wiselib_timer_, wiselib_debug_ );
         tree_routing_.reg_recv_callback<WiselibExampleProcessor,
            &WiselibExampleProcessor::rcv_routing_message>(this);
      }
      else if (rt_algo_ == "dsdv")
      {
         INFO( logger(), "Start DSDV routing on " << owner().id() );
         dsdv_routing_.init( wiselib_radio_, wiselib_timer_, wiselib_debug_ );
         dsdv_routing_.reg_recv_callback<WiselibExampleProcessor,
            &WiselibExampleProcessor::rcv_routing_message>(this);
      }
      else if (rt_algo_ == "dsr")
      {
         INFO( logger(), "Start DSR routing on " << owner().id() );
         dsr_routing_.init( wiselib_radio_, wiselib_timer_, wiselib_debug_ );
         dsr_routing_.reg_recv_callback<WiselibExampleProcessor, 
            &WiselibExampleProcessor::rcv_routing_message>(this);
      }
      else
      {
         ERROR( logger(), "Given routing algorithm '" << rt_algo_ << "' not known." );
         ERROR( logger(), "Try 'tree', 'dsdv', or 'dsr'." );
         abort();
      }
   }
   // ----------------------------------------------------------------------
   void
   WiselibExampleProcessor::
   work( void )
      throw()
   {
      if (rt_algo_ == "tree")
      {
         if ( owner().id() != 0 && owner().world().simulation_round() == 20 )
            tree_routing_.send( 0, 0, 0 );
      }
      else if (rt_algo_ == "dsdv")
      {
         if ( owner().id() != 0 && owner().world().simulation_round() == 20 )
            dsdv_routing_.send( 0, 0, 0 );
      }
      else if (rt_algo_ == "dsr")
      {
         if ( owner().id() != 0 && owner().world().simulation_round() == 1 )
            dsr_routing_.send( 0, 0, 0 );
      }
   }
   // ----------------------------------------------------------------------
   void
   WiselibExampleProcessor::
   rcv_routing_message( int from, long len, unsigned char* data) 
      throw()
   {
      INFO( logger(), "Received at node " << owner().id() << " from " << from << " at "
         << owner().world().simulation_round() );
   }

}
#endif
