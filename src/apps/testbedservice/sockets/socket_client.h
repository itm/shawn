/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SOCKET_CLIENT_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SOCKET_CLIENT_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include "apps/testbedservice/sockets/proto/WSNAppMessages.pb.h"
#include <boost/thread.hpp>
#include <boost/threadpool.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/asio.hpp>
#include <list>
#include <queue>

namespace testbedservice
{

   class SocketClient
   {
   public:
      typedef de::uniluebeck::itm::tr::runtime::wsnapp::Message Message;
      typedef de::uniluebeck::itm::tr::runtime::wsnapp::Message_TextMessage TextMessage;
      typedef de::uniluebeck::itm::tr::runtime::wsnapp::Message_BinaryMessage BinaryMessage;

      class MessageCallback
      {
         public:
            virtual void message_received( const Message& ) = 0;
      };

      typedef boost::asio::ip::tcp::socket socket;
      typedef boost::asio::io_service io_service;

      typedef std::list<MessageCallback*> CallbackList;
      typedef CallbackList::iterator CallbackListIterator;
      typedef CallbackList::const_iterator ConstCallbackListIterator;

      SocketClient();
      ~SocketClient();
      // --------------------------------------------------------------------
      void init( const shawn::SimulationController&, const std::string& remote_host, const std::string& remote_port );

      void add_listener( MessageCallback *callback );
      void remove_listener( MessageCallback *callback );

      void send_binary_data( const std::string& source,
                             const std::string& destination,
                             uint8_t *buf,
                             int length );
      void send_debug_data( const std::string& source,
                            const std::string& destination,
                            const std::string& debug,
                            const std::string& level);

   private:
      void send_message( const std::string& destination, Message& message );

      void handle_header( const boost::system::error_code& error );
      void handle_packet( const boost::system::error_code& error, int length );

      void call_listeners( Message& message );

      enum {
         packet_header = 4
      };

      io_service *io_service_;
      socket *s_;
      boost::thread *runner_;

      std::string remotehost_;
      std::string remoteport_;
      std::string localhost_;

      std::vector<char> buffer_space_;
      uint8_t buffer_[512];

      boost::mutex list_access_mutex_;
      CallbackList callback_list_;
   };

}

#endif
#endif
