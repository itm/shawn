/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/ws_handler/network_control.h"
#include "apps/testbedservice/core/wsnapi_serverH.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/bind.hpp>

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_generator.h"
#endif

namespace testbedservice
{
   static testbedservice::NetworkControl *network_control_ = 0;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   NetworkControl::
   NetworkControl()
      : controller_ (0),
         sc_( 0 )
   {}
   // ----------------------------------------------------------------------
   NetworkControl::
   ~NetworkControl()
   {}
   // ----------------------------------------------------------------------
   void
   NetworkControl::
   init( shawn::SimulationController& sc, TestbedServiceClient& controller )
      throw()
   {
      sc_ = &sc;
      controller_ = &controller;
      network_control_ = this;
   }
   // ----------------------------------------------------------------------
   char*
   NetworkControl::
   wiseml_network( void ) throw()
   {
      boost::lock_guard<boost::mutex> generate_lock( generate_mutex_ );

      wiseml::WisemlGenerator generator( *sc_ );
      // TODO: Make this threadsafe with respect to Shawn; -> add to EventScheduler?
      std::string s = generator.generate_wiseml(false);

      delete network_representation_;
      network_representation_ = new char[ s.length() + 1 ];
      memcpy( network_representation_, s.c_str(), s.length() + 1 );

      return network_representation_;
   }
}

namespace wsnapi_server
{

   static bool network_control_available( void )
   {
      if ( testbedservice::network_control_ )
      {
         return true;
      }
      else
         std::cerr << "Error processing ws call - network_control_ not set." << std::endl;

      return false;
   }

   // -----------------------------------------------------------------------
   // ------------- Web Service Calls Network Control -----------------------
   // -----------------------------------------------------------------------
   int __ns1__getNetwork( soap*,
            ns3__getNetwork *shawnts__getNetwork_,
            ns3__getNetworkResponse *shawnts__getNetworkResponse_ )
   {
#ifdef ENABLE_WISEML
      if ( network_control_available() )
      {
         char *network = testbedservice::network_control_->wiseml_network();
         std::cout << network << std::endl;
         shawnts__getNetworkResponse_->return_ = network;
      }
#else
      std::cerr << "Error processing getNetwork() - wiseml module is not enabled." << std::endl;
#endif
      return SOAP_OK;
   }
}

#endif
