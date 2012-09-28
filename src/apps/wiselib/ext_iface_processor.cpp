/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/ext_iface_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node.h"
#include <cmath>


namespace wiselib
{
   ExtIfaceProcessor::
   ExtIfaceProcessor()
   {}
   // ----------------------------------------------------------------------
   ExtIfaceProcessor::
   ~ExtIfaceProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   ExtIfaceProcessor::
   boot()
      throw()
   {
#ifdef ENABLE_TESTBEDSERVICE
      set_node( owner_w() );
      testbedservice_proc_boot();
#endif
   }
   // ----------------------------------------------------------------------
   bool
   ExtIfaceProcessor::
   process_message( const shawn::ConstMessageHandle& mh )
      throw()
   {
      const ExtIfaceWiselibMessage* message = dynamic_cast<const ExtIfaceWiselibMessage*> ( mh.get() );
      if ( message != NULL )
      {
         if ( message->source().id()!=owner().id()
        		 && (message->destination() == owner().id()
                              || message->destination() == BROADCAST_ADDRESS) )
         {
            for ( ReceiverListIterator
                     it = delegates_.begin();
                     it != delegates_.end();
                     ++it )
               (*it)( message->source().id(), message->payload_size(), message->payload() );
            if(!ext_delegates_.empty()){
                node_id_t const from=message->source().id();
                double const ux=owner().real_position().x();
                double const uy=owner().real_position().y();
                double const uz=owner().real_position().z();
                double const vx=owner().world().find_node_by_id(from)->real_position().x();
                double const vy=owner().world().find_node_by_id(from)->real_position().y();
                double const vz=owner().world().find_node_by_id(from)->real_position().z();
                double const dist=std::sqrt((vx-ux)*(vx-ux)+(vy-uy)*(vy-uy)+(vz-uz)*(vz-uz));
                ExtendedData ex;
                ex.set_link_metric((0xffff*dist/owner_w().transmission_range())+0.5);
                for ( ExtendedReceiverListIterator
                         it = ext_delegates_.begin();
                         it != ext_delegates_.end();
                         ++it )
                   (*it)(from, message->payload_size(), message->payload(), ex);
            }
         }
         return true;
      }

      // TODO: const webservice::VirtualLinkMessage* message = ...

      return shawn::Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
#ifdef ENABLE_TESTBEDSERVICE
   void
   ExtIfaceProcessor::
   receive_vlink( testbedservice::VirtualLinkMessage *vlm )
      throw()
   {
      if ( vlm->destination == owner().id()
               || vlm->destination == BROADCAST_ADDRESS )
      {
//          std::cout << "--- pass to wiselib" << std::endl;
         for ( ReceiverListIterator
                     it = delegates_.begin();
                     it != delegates_.end();
                     ++it )
               (*it)( vlm->source, vlm->payload_size, vlm->payload );
      }
   }
   // ----------------------------------------------------------------------
   void
   ExtIfaceProcessor::
   process_message( testbedservice::TestbedserviceMessage& message )
      throw()
   {
      for ( UartReceiverListIterator
               it = uart_delegates_.begin();
               it != uart_delegates_.end();
               ++it )
         (*it)( message.size, message.buffer );
   }
#endif
   // ----------------------------------------------------------------------
   void
   ExtIfaceProcessor::
   timeout(shawn::EventScheduler& es,
           shawn::EventScheduler::EventHandle eh,
           double time,
           shawn::EventScheduler::EventTagHandle& tag_handle) throw()
   {
      ExtIfaceWiselibEventTag *tag = dynamic_cast<ExtIfaceWiselibEventTag*>( tag_handle.get() );
      if ( state() == Active && tag && tag->delegate() )
         tag->delegate()( tag->userdata() );
   }

}
#endif
