/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/session_control.h"
#include "apps/testbedservice/util/ws_helpers.h"
#include "apps/testbedservice/core/session_managementH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/bind.hpp>


namespace testbedservice
{
   static testbedservice::SessionControl *session_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   SessionControl::
   SessionControl()
      : controller_ (0),
         sc_( 0 ),
         wsn_api_uri_ ( "WSN-API-URI NOT SET!!" )
   {}
   // ----------------------------------------------------------------------
   SessionControl::
   ~SessionControl()
   {}
   // ----------------------------------------------------------------------
   void
   SessionControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      session_control_ = this;

      std::string host = sc.environment().required_string_param( "testbedservice_server_host" );
      std::string port = sc.environment().required_string_param( "testbedservice_server_port" );

      wsn_api_uri_ = "http://" + host + ":" + port + "/";
   }
   // ----------------------------------------------------------------------
   std::string
   SessionControl::
   wsn_api_uri( void )
      throw()
   {
      return wsn_api_uri_;
   }

}

namespace session_management
{

   static bool session_control_available( void )
   {
      if ( testbedservice::session_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - session_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Session Control -----------------------
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__getInstance( struct soap* soap,
             ns1__getInstance *ns1__getInstance_,
             ns1__getInstanceResponse *ns1__getInstanceResponse_ )
   {
      if ( !session_control_available() )
         return SOAP_OK;

      std::string controller_uri;

      std::string uri = testbedservice::session_control_->wsn_api_uri();
      std::cerr << "SessionManagement: getInstance called." << std::endl;
      std::cerr << "  -> WSN URI is: " << uri << std::endl;
      std::cerr << "  -> Reservation IDs: " << std::endl;

      for ( int i = 0; i < ns1__getInstance_->__sizesecretReservationKey; i++ )
      {
         std::string reservation_id;
         if ( ns1__getInstance_->secretReservationKey[i] )
            reservation_id =
               std::string( ns1__getInstance_->secretReservationKey[i]->urnPrefix )
               + ","
               + std::string( ns1__getInstance_->secretReservationKey[i]->secretReservationKey );
         std::cerr << "     ---> ResId: " << controller_uri << std::endl;
      }

      if ( ns1__getInstance_->controller )
         controller_uri = std::string( ns1__getInstance_->controller );
      std::cerr << "  -> Controller URI: " << controller_uri << std::endl;

      testbedservice::session_control_->controller().set_controller_uri( controller_uri );

      ns1__getInstanceResponse_->return_ = testbedservice::allocate_string( soap, uri );

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__free( struct soap*,
            ns1__free *ns1__free_,
            ns1__freeResponse *ns1__freeResponse_ )
   {
      // TODO: Implement
      std::cerr << "SessionManagement: free not yet implemented." << std::endl;

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__getNetwork( struct soap*,
            ns3__getNetwork *ns3__getNetwork_,
            ns3__getNetworkResponse *ns3__getNetworkResponse_ )
   {
      // TODO: Implement
      std::cerr << "SessionManagement: getNetwork not yet implemented." << std::endl;

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__getConfiguration( struct soap*,
            ns1__getConfiguration *ns1__getConfiguration_,
            ns1__getConfigurationResponse *ns1__getConfigurationResponse_)
   {
      // TODO: Implement
      std::cerr << "SessionManagement: getConfiguration not yet implemented." << std::endl;

      return SOAP_OK;
   }
   // -----------------------------------------------------------------------
   SOAP_FMAC5 int SOAP_FMAC6 __ns1__areNodesAlive( soap*,
            ns1__areNodesAlive *ns1__areNodesAlive_,
            ns1__areNodesAliveResponse *ns1__areNodesAliveResponse_ )
   {
      // TODO: Implement
      std::cerr << "SessionManagement: areNodesAlive not yet implemented." << std::endl;

      return SOAP_OK;
   }

}

#endif
