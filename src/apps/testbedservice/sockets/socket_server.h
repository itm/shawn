/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SOCKET_SERVER_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SOCKET_SERVER_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <list>


namespace testbedservice
{

   class SocketServer
   {

      class WorkerThread
      {
      public:
         WorkerThread();
         virtual ~WorkerThread();

         typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

         void session( socket_ptr sock );
         void run( int port );

      private:
         bool initialized_;
      };

   public:
      SocketServer();
      ~SocketServer();
      // --------------------------------------------------------------------
      bool start_server( const shawn::SimulationController& );

   private:
      int port_;
      boost::thread *runner_;

      WorkerThread worker_;
   };

}

#endif
#endif
