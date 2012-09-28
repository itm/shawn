/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_EXTIFACE_PROCESSOR_H
#define __SHAWN_APPS_EXTIFACE_PROCESSOR_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "apps/wiselib/wiselib_message.h"
#include "apps/wiselib/wiselib_event_tag.h"
#include "sys/processor.h"
#include "sys/node.h"
#include "sys/event_scheduler.h"
#include "util/delegates/delegate.hpp"
#include <stdint.h>
#include <list>

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE
#include "apps/testbedservice/processor/testbedservice_processor.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "apps/testbedservice/util/types.h"
#endif

namespace wiselib
{

   /**
    */
   class ExtIfaceProcessor
       : public virtual shawn::Processor,
         public virtual shawn::EventScheduler::EventHandler
#ifdef ENABLE_TESTBEDSERVICE
         , public testbedservice::TestbedServiceProcessor
#endif
   {
   public:
      typedef int node_id_t;
      typedef unsigned char block_data_t;
      typedef long size_t;
      typedef uint8_t message_id_t;

      class ExtendedData{
      public:
         ExtendedData(){}

         uint16_t link_metric() const
         { return link_metric_; };

         void set_link_metric( uint16_t lm )
         { link_metric_ = lm; };

      private:
         uint16_t link_metric_;
      };
      // --------------------------------------------------------------------
      typedef delegate3<void, node_id_t, size_t, block_data_t*> shawn_radio_delegate_t;
      typedef delegate4<void,node_id_t,size_t,block_data_t*,const ExtendedData&> shawn_extended_radio_delegate_t;
      typedef delegate1<void, void*> shawn_timer_delegate_t;
      typedef delegate2<void, size_t, block_data_t*> shawn_uart_delegate_t;

      typedef WiselibEventTag<shawn_timer_delegate_t> ExtIfaceWiselibEventTag;
      typedef WiselibMessage<node_id_t, block_data_t, size_t> ExtIfaceWiselibMessage;

      static const int BROADCAST_ADDRESS = 0xffff;

      ///@name Constructor/Destructor
      ///@{
      ExtIfaceProcessor();
      virtual ~ExtIfaceProcessor();
      ///@}

      ///@name Inherited from Processor
      ///@{
      /**
       */
      virtual void boot() throw();
      /**
       */
      virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
      ///@}

#ifdef ENABLE_TESTBEDSERVICE
      void receive_vlink( testbedservice::VirtualLinkMessage* ) throw();

      virtual void process_message( testbedservice::TestbedserviceMessage& message ) throw();
#endif

      // --------------------------------------------------------------------
      virtual void timeout( shawn::EventScheduler&,
                            shawn::EventScheduler::EventHandle,
                            double,
                            shawn::EventScheduler::EventTagHandle& ) throw();
      // --------------------------------------------------------------------
      void send_wiselib_message( node_id_t id, size_t len, block_data_t *data )
      {
         send( new ExtIfaceWiselibMessage( id, len, data ) );
      };
      // --------------------------------------------------------------------
      template<typename T, void (T::*TMethod)(void*)>
      bool set_timeout( uint32_t millis, T *obj_pnt, void *userdata )
      {
         double time = (millis / 1000.0) + owner().current_time();

         shawn_timer_delegate_t d;
         d = shawn_timer_delegate_t::from_method<T, TMethod>( obj_pnt );

         ExtIfaceWiselibEventTag *et = new ExtIfaceWiselibEventTag( d, userdata );
         owner_w().world_w().scheduler_w().new_event( *this, time, et );

         return true;
      }
      // --------------------------------------------------------------------
      template<class T, void (T::*TMethod)( node_id_t, size_t, block_data_t* )>
      bool reg_recv_callback( T *obj_pnt )
      {
         shawn_radio_delegate_t del = shawn_radio_delegate_t::from_method<T, TMethod>( obj_pnt );
         delegates_.push_back(del);
         return true;
      }
      // --------------------------------------------------------------------
      template<void (*TMethod)( node_id_t, size_t, block_data_t* )>
      bool reg_recv_callback()
      {
         shawn_radio_delegate_t del = shawn_radio_delegate_t::from_function<TMethod>();
         delegates_.push_back(del);
         return true;
      }
      // --------------------------------------------------------------------
      template<class T, void (T::*TMethod)( node_id_t, size_t, block_data_t*, ExtendedData const & )>
      bool reg_recv_callback( T *obj_pnt )
      {
         shawn_extended_radio_delegate_t del = shawn_extended_radio_delegate_t::from_method<T, TMethod>( obj_pnt );
         ext_delegates_.push_back(del);
         return true;
      }
      // --------------------------------------------------------------------
      template<void (*TMethod)( node_id_t, size_t, block_data_t*, ExtendedData const & )>
      bool reg_recv_callback()
      {
         shawn_extended_radio_delegate_t del = shawn_extended_radio_delegate_t::from_function<TMethod>();
         ext_delegates_.push_back(del);
         return true;
      }
      // --------------------------------------------------------------------
#ifdef ENABLE_TESTBEDSERVICE
      template<class T, void (T::*TMethod)( size_t, block_data_t* )>
      bool reg_uart_recv_callback( T *obj_pnt )
      {
         shawn_uart_delegate_t del = shawn_uart_delegate_t::from_method<T, TMethod>( obj_pnt );
         uart_delegates_.push_back(del);
         return true;
      }
#endif

   private:
      typedef std::list<shawn_radio_delegate_t> ReceiverList;
      typedef std::list<shawn_extended_radio_delegate_t> ExtendedReceiverList;
      typedef std::list<shawn_uart_delegate_t> UartReceiverList;
      typedef ReceiverList::iterator ReceiverListIterator;
      typedef ExtendedReceiverList::iterator ExtendedReceiverListIterator;
      typedef UartReceiverList::iterator UartReceiverListIterator;
      ReceiverList delegates_;
      ExtendedReceiverList ext_delegates_;
      UartReceiverList uart_delegates_;
   };

}

#endif
#endif
