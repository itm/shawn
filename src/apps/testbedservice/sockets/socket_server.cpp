/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/sockets/socket_server.h"
#include "apps/testbedservice/sockets/proto/Messages.pb.h"
#include "apps/testbedservice/sockets/proto/WSNAppMessages.pb.h"
#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using boost::asio::ip::tcp;
using de::uniluebeck::itm::gtr::messaging::Msg;
using namespace de::uniluebeck::itm::tr::runtime::wsnapp;

typedef Msg TestbedRuntimeMessage;
typedef Message WsnAppMessage;

namespace testbedservice
{

   SocketServer::
   SocketServer()
   {}
   // --------------------------------------------------------------------
   SocketServer::
   ~SocketServer()
   {}
   // --------------------------------------------------------------------
   bool
   SocketServer::
   start_server( const shawn::SimulationController& sc )
   {
//       GOOGLE_PROTOBUF_VERIFY_VERSION;

      port_ = sc.environment().required_int_param( "socket_server_port" );
      runner_ = new boost::thread( boost::bind( &WorkerThread::run, &worker_, port_ ) );

//       boost::thread thrd( boost::bind( &WorkerThread::run, &worker_, port_ ) );
//       thrd.join();

      return true;
   }
   // --------------------------------------------------------------------
   SocketServer::WorkerThread::
   WorkerThread()
      : initialized_( false )
   {}
   // --------------------------------------------------------------------
   SocketServer::WorkerThread::
   ~WorkerThread()
   {}
   // --------------------------------------------------------------------
   void SocketServer::WorkerThread::session( socket_ptr sock )
   {
      std::cout << "New connection established...." << std::endl;
      try
      {
         for (;;)
         {
            // data buffer of 1024 bytes
            unsigned char data[0x400];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
               break; // Connection closed cleanly by peer.
            else if (error)
               throw boost::system::system_error(error); // Some other error.

//             /*std::cout << "Received new message" << std::endl;
//             std::cout << "  -> " << length << " bytes" << std::endl;*/
//             TestbedRuntimeMessage tr_message;
//             tr_message
// TODO: inject_message no longer available
//             inject_message( length, data );
            // uncomment to enable echo test!
//             boost::asio::write(*sock, boost::asio::buffer(data, length));
         }
      }
      catch (std::exception& e)
      {
         std::cerr << "Exception in thread: " << e.what() << "\n";
      }
   }
   // --------------------------------------------------------------------
   void
   SocketServer::WorkerThread::
   run( int port )
   {
      boost::asio::io_service io_service;
      tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
      for (;;)
      {
         socket_ptr sock( new tcp::socket(io_service) );
         a.accept( *sock );
         boost::thread t( boost::bind(&WorkerThread::session, this, sock) );
      }
   }

}

#endif
