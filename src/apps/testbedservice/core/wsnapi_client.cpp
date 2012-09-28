/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/wsnapi_client.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/core/wsnapi_clientWSNPortBindingProxy.h"
#include "apps/testbedservice/core/wsnapi_client.nsmap"
#include <boost/bind.hpp>
#include <time.h>
#include <iostream>

using namespace wsnapi_client;

namespace testbedservice
{

   WsnApiClient::
   WsnApiClient()
      : wsnapi_uri_ ( "http://localhost:8080/controller" ),
         sendmode_  ( SEND_MODE_TESTBEDRUNTIME_ISENSE ),
         max_pool_size_ ( 5 ),
         wsnapi_        ( 0 )
   {}
   // --------------------------------------------------------------------
   WsnApiClient::
   ~WsnApiClient()
   {
      delete wsnapi_;
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   init( std::string uri )
   {
      wsnapi_ = new WSNPortBindingProxy();

      if ( !uri.empty() )
      {
         wsnapi_uri_ = uri;
         wsnapi().soap_endpoint = wsnapi_uri_.c_str();
      }
      
      pool_.size_controller().resize( max_pool_size_ );
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   send_message( NodeIdVector dests, TestbedserviceMessage& msg, bool threaded  )
      throw()
   {
      // prepare basic message types
      ns1__send *send = new ns1__send;
      ns3__message *message = new ns3__message;

      send->message = message;
      send->__sizenodeIds = dests.size();
      send->nodeIds = new char*[ dests.size() ];
std::cout << "Webservice Message" << std::endl;
      for ( unsigned int i = 0; i < dests.size(); i++ )
      {
         send->nodeIds[i] = allocate_string( dests.at(i) );
std::cout << "    -> send to " << send->nodeIds[i] << std::endl;
      }
std::cout << "  -> sent " << send->__sizenodeIds << std::endl;

      message->timestamp = msg.timestamp;
      message->sourceNodeId = allocate_string( msg.source );
std::cout << "  -> from " << message->sourceNodeId << std::endl;
std::cout << "  -> at " << message->timestamp << std::endl;

      int offset = 0;
      // if sent to isense nodes controlled via UZL's testbed runtime, first
      // byte in data is isense packet type
      if ( sendmode_ == SEND_MODE_TESTBEDRUNTIME_ISENSE )
         offset = 1;

      uint8_t *buf_copy = new uint8_t[msg.size + offset];
      memcpy( buf_copy + offset, msg.buffer, msg.size );
      
      if ( sendmode_ == SEND_MODE_TESTBEDRUNTIME_ISENSE )
         buf_copy[0] = 10;

      message->binaryData.__ptr = buf_copy;
      message->binaryData.__size = msg.size + offset;

      if ( threaded )
      {
         boost::lock_guard<boost::mutex> pool_lock( pool_mutex_ );
         pool_.schedule( boost::bind( &WsnApiClient::call_send_message, this, send ) );
      }
      else
         call_send_message( send );
   }
   // --------------------------------------------------------------------
   void
   WsnApiClient::
   call_send_message( ns1__send *send )
   {
      boost::lock_guard<boost::mutex> send_lock( send_mutex_ );

      ns1__sendResponse *response = new ns1__sendResponse;
      
      if ( wsnapi().send( send, response ) == SOAP_OK )
      {
         std::cerr << "Send status ok" << std::endl;
      }
      else
      {
         std::cerr << "Send to WSN API failed" << std::endl;
         if ( wsnapi().soap_fault_string() )
            std::cerr << "  Reason: " << wsnapi().soap_fault_string() << std::endl;
         if ( wsnapi().soap_fault_detail() )
            std::cerr << "  Detail: " << wsnapi().soap_fault_detail() << std::endl;
      }
      delete response;

      delete send->message->binaryData.__ptr;
      delete send->message->binaryData.id;
      delete send->message->binaryData.type;
      delete send->message->binaryData.options;

      delete send->message->sourceNodeId;
      delete send->message;
      delete send;
   }

}

#endif
