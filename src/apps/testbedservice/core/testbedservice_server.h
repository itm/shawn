/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SERVER_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SERVER_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/wsnapi_serverH.h"
#include "sys/simulation/simulation_controller.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <list>

struct soap;

namespace testbedservice
{

   class TestbedServiceServer
   {

      class WorkerThread
      {
      public:
         WorkerThread();
         virtual ~WorkerThread();

         void run( std::string host, int port );
         void print_soap_ip( int idx, int socket );

         static int http_get( struct soap *soap );
         static int pass_file( struct soap *soap, std::string file );

         static std::string wsdl_path;

      private:
         bool initialized_;
         struct soap soap_;
      };

   public:
      TestbedServiceServer();
      ~TestbedServiceServer();
      // --------------------------------------------------------------------
      bool start_server( const shawn::SimulationController& );

   private:
      std::string host_;
      int port_;
      boost::thread *runner_;

      WorkerThread worker_;
   };

}

#endif
#endif
