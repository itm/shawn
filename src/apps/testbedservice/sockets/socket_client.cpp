/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/sockets/socket_client.h"
#include "apps/testbedservice/sockets/proto/Messages.pb.h"
#include "apps/testbedservice/sockets/proto/WSNAppMessages.pb.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <sstream>
#include <ctime>

using boost::asio::ip::tcp;
using de::uniluebeck::itm::gtr::messaging::Msg;
using namespace de::uniluebeck::itm::tr::runtime::wsnapp;

typedef Msg TestbedRuntimeMessage;

namespace testbedservice
{

   const std::string LISTENER_MESSAGE = "de.uniluebeck.itm.tr.runtime.wsnapp.WSNApp/LISTENER_MESSAGE";
   const std::string INVOCATION_REQUEST = "de.uniluebeck.itm.tr.runtime.wsnapp.WSNApp/OPERATION_INVOCATION_REQUEST";
   // --------------------------------------------------------------------
   // --------------------------------------------------------------------
   // --------------------------------------------------------------------
   SocketClient::
   SocketClient()
      : io_service_ ( 0 ),
         s_         ( 0 ),
         runner_    ( 0 )
   {}
   // --------------------------------------------------------------------
   SocketClient::
   ~SocketClient()
   {
      if ( s_ )
         s_->close();
      if ( io_service_ )
         io_service_->stop();
      runner_->join();

      delete s_;
      delete io_service_;
      delete runner_;
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   init( const shawn::SimulationController& sc, const std::string& remote_host, const std::string& remote_port )
   {
      remotehost_ = remote_host;
      remoteport_ = remote_port;
      localhost_ = sc.environment().required_string_param( "socket_from_adress" );

      try
      {
         io_service_ = new boost::asio::io_service();
         tcp::resolver resolver(*io_service_);
         tcp::resolver::query query(tcp::v4(), remotehost_, remoteport_);
         tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
         tcp::resolver::iterator end;

         s_ = new tcp::socket(*io_service_);
         boost::system::error_code error = boost::asio::error::host_not_found;
         while (true)
         {
            try
            {
               (*s_).connect(*endpoint_iterator++, error);
               break;
            }
            catch (std::exception& ex)
            {
               // Release resources, then try connecting again.
               std::cerr << "Error while connecting." << std::endl;
               s_->close();
            }
         }

         boost::asio::async_read( *s_,
                                  boost::asio::buffer(buffer_, packet_header),
                                  boost::bind( &SocketClient::handle_header,
                                               this,
                                               boost::asio::placeholders::error ) );
         runner_ = new boost::thread( boost::bind( &boost::asio::io_service::run, io_service_ ) );
      }
      catch (std::exception& e)
      {
         std::cerr << "Exception in start_client: " << e.what() << "\n";
      }
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   add_listener( MessageCallback *callback )
   {
      boost::unique_lock<boost::mutex> list_access_lock( list_access_mutex_ );
      callback_list_.push_back( callback );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   remove_listener( MessageCallback *callback )
   {
      boost::unique_lock<boost::mutex> list_access_lock( list_access_mutex_ );
      callback_list_.remove( callback );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   send_binary_data( const std::string& source,
                     const std::string& destination,
                     uint8_t *buffer, int length )
   {
      Message_BinaryMessage binmessage;
      binmessage.set_binarytype( 10 );
      binmessage.set_binarydata( buffer, length );

      Message message;
      message.set_sourcenodeid( source );
      message.set_timestamp( "---" );
      message.mutable_binarymessage()->CopyFrom(binmessage);

      send_message( destination, message );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   send_debug_data( const std::string& source,
                    const std::string& destination,
                    const std::string& debug,
                    const std::string& level )
   {
      Message_TextMessage textmessage;
      Message_MessageLevel msg_level = Message_MessageLevel_DEBUG;
      Message_MessageLevel_Parse( level, &msg_level );
      textmessage.set_messagelevel( msg_level );
      textmessage.set_msg( debug );

      Message message;
      message.set_sourcenodeid( source );
      message.set_timestamp( "---" );
      message.mutable_textmessage()->CopyFrom(textmessage);

      send_message( destination, message );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   send_message( const std::string& destination, SocketClient::Message& message )
   {
      OperationInvocation operation;
      operation.set_operation( OperationInvocation_Operation_SEND );
      operation.set_arguments( message.SerializeAsString() );

      TestbedRuntimeMessage trmessage;
      trmessage.set_msgtype( INVOCATION_REQUEST );
      trmessage.set_from( localhost_ );
      trmessage.set_to( destination );
      trmessage.set_payload( operation.SerializeAsString() );
      trmessage.set_priority( 1 );
      time_t now = time(0);
      trmessage.set_validuntil( now * 1000 + 5000 );

      uint8_t buf[ trmessage.ByteSize() + 4 ];
      buf[0] = (trmessage.ByteSize() >> 24) & 0xff;
      buf[1] = (trmessage.ByteSize() >> 16) & 0xff;
      buf[2] = (trmessage.ByteSize() >> 8) & 0xff;
      buf[3] = trmessage.ByteSize() & 0xff;
      trmessage.SerializeToArray( buf + 4, trmessage.ByteSize() );

      // TODO: use async_write?
      boost::asio::write( *s_, boost::asio::buffer( buf, sizeof(buf) ) );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   handle_header( const boost::system::error_code& error )
   {
      uint32_t rcvd_length = buffer_[0] << 24 | buffer_[1] << 16 |
                                 buffer_[2] << 8 | buffer_[3];

      // read in packet
      boost::asio::async_read( *s_,
                               boost::asio::buffer(buffer_, rcvd_length),
                               boost::bind( &SocketClient::handle_packet,
                                            this,
                                            boost::asio::placeholders::error,
                                            rcvd_length ) );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   handle_packet( const boost::system::error_code& error, int length )
   {
      TestbedRuntimeMessage trmessage;
      trmessage.ParseFromArray( buffer_, length );
      if ( trmessage.msgtype() != LISTENER_MESSAGE )
      {
         std::cerr << "Client: Unkown input" << std::endl;
      }
      else
      {
         Message message;
         message.ParseFromString( trmessage.payload() );
         call_listeners( message );
      }

      // wait for next header
      boost::asio::async_read( *s_,
                               boost::asio::buffer(buffer_, packet_header),
                               boost::bind( &SocketClient::handle_header,
                                            this,
                                            boost::asio::placeholders::error ) );
   }
   // --------------------------------------------------------------------
   void
   SocketClient::
   call_listeners( SocketClient::Message& message )
   {
      boost::unique_lock<boost::mutex> list_access_lock( list_access_mutex_ );
      for ( CallbackListIterator
               it = callback_list_.begin();
               it != callback_list_.end();
               ++it )
         (*it)->message_received( message );
   }

}

#endif
