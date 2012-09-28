/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_server.h"
#include "apps/testbedservice/core/wsnapi_server.nsmap"
#include "sys/simulation/simulation_environment.h"
#include "sys/node.h"
#include "sys/world.h"
#include <boost/bind.hpp>

namespace testbedservice
{

   TestbedServiceServer::
   TestbedServiceServer()
   {}
   // --------------------------------------------------------------------
   TestbedServiceServer::
   ~TestbedServiceServer()
   {}
   // --------------------------------------------------------------------
   bool
   TestbedServiceServer::
   start_server( const shawn::SimulationController& sc )
   {
      host_ = sc.environment().required_string_param( "testbedservice_server_host" );
      port_ = sc.environment().required_int_param( "testbedservice_server_port" );
      TestbedServiceServer::WorkerThread::wsdl_path =
         sc.environment().required_string_param( "testbedservice_wsdl" );

      runner_ = new boost::thread( boost::bind( &WorkerThread::run, &worker_, host_, port_ ) );
      return true;
   }
   // --------------------------------------------------------------------
   // --------------------------------------------------------------------
   // --------------------------------------------------------------------
   std::string TestbedServiceServer::WorkerThread::wsdl_path = "./";
   // --------------------------------------------------------------------
   TestbedServiceServer::WorkerThread::
   WorkerThread()
      : initialized_( false )
   {}
   // --------------------------------------------------------------------
   TestbedServiceServer::WorkerThread::
   ~WorkerThread()
   {
      if ( initialized_ )
         soap_done( &soap_ ); // close master socket and detach environment
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceServer::WorkerThread::
   run( std::string host, int port )
   {
      int m, s; // master and slave sockets
      soap_init( &soap_ );
      soap_.namespaces = wsnapi_server_namespaces;
      soap_.fget = http_get;
      m = soap_bind( &soap_, host.c_str(), port, 100 );
      if (m < 0)
      {
         soap_print_fault(&soap_, stderr);
         exit(1);
      }
      else
      {
//          std::cerr << "Socket connection successful: master socket = " << m << std::endl;
         initialized_ = true;
         for ( int i = 1; ; i++ )
         {
            s = soap_accept( &soap_ );
            if (s < 0)
            {
               soap_print_fault( &soap_, stderr );
               break;
            }
//             print_soap_ip( i, s );

           // TODO: do this in own thread!
            if ( wsnapi_server::wsnapi_server_serve(&soap_) != SOAP_OK ) // process RPC request
               soap_print_fault(&soap_, stderr); // print error
//             std::cerr << "gsoap::server: request served" << std::endl;
            soap_destroy( &soap_ ); // clean up class instances
            soap_end( &soap_ ); // clean up everything and close socket
         }
      }
   }
   // --------------------------------------------------------------------
   void
   TestbedServiceServer::WorkerThread::
   print_soap_ip( int idx, int socket )
   {
      std::cout << idx << ": accepted connection from IP="
                  << ((soap_.ip >> 24) & 0xFF) << "."
                  << ((soap_.ip >> 16) & 0xFF) << "."
                  << ((soap_.ip >>  8) & 0xFF) << "."
                  << ( soap_.ip        & 0xFF) << " socket=" << socket << std::endl;
   }
   // --------------------------------------------------------------------
   int
   TestbedServiceServer::WorkerThread::
   http_get( struct soap *soap )
   {
std::cout << "REQ: " << soap->path << std::endl;
      char *s = strchr(soap->path, '?');
      if ( s && !strcmp(s, "?wsdl") )
         return pass_file( soap, wsdl_path + "WSNService.wsdl" );

      s = strchr(soap->path, '/');
      if ( s && !strcmp(s, "/WSNTypes.xsd") )
         return pass_file( soap, wsdl_path + "WSNTypes.xsd" );
      if ( s && !strcmp(s, "/CommonTypes.xsd") )
         return pass_file( soap, wsdl_path + "CommonTypes.xsd" );

      return SOAP_GET_METHOD;
   }
   // --------------------------------------------------------------------
   int
   TestbedServiceServer::WorkerThread::
   pass_file( struct soap *soap, std::string path )
   {
std::cout << "OPEN: " << path << std::endl;
      // open WSDL file to copy
      FILE* fd = fopen( path.c_str(), "rb");
      if (!fd)
         return 404; // return HTTP not found error

      soap->http_content = "text/xml"; // HTTP header with text/xml content
      soap_response(soap, SOAP_FILE);
      for (;;)
      {
         size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
         if (!r)
            break;
         if (soap_send_raw(soap, soap->tmpbuf, r))
            break; // can't send, but little we can do about that
      }
      fclose( fd );
      soap_end_send( soap );

      return SOAP_OK;
   }
}

#endif
