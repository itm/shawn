/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/testbedservice_task.h"
#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"
#include <boost/thread.hpp>


namespace testbedservice
{

   TestbedServiceTask::
   TestbedServiceTask()
   {}
   // ----------------------------------------------------------------------
   TestbedServiceTask::
   ~TestbedServiceTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceTask::
   name( void )
      const throw()
   {
      return "testbedservice";
   }
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceTask::
   description( void )
      const throw()
   {
      return "Start testbedservice for remote access; connection to portal server";
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      service_client_.init( sc );

      // search for virtual link transmission model
      VirtualLinkTransmissionModel *vltm = 0;
      shawn::ChainableTransmissionModel *ctm =
         dynamic_cast<shawn::ChainableTransmissionModel*>(
                                 &sc.world_w().transmission_model_w() );
      do
      {
         vltm = dynamic_cast<VirtualLinkTransmissionModel*>( ctm );
         if (vltm)
            break;

         ctm = dynamic_cast<shawn::ChainableTransmissionModel*>(
                                 &ctm->next_transm_model_w() );
         if (ctm)
            continue;
      } while ( ctm->has_next_transm_model() );

      if ( vltm )
      {
         vltm->set_testbedservice_client( service_client_ );
      }
      else
      {
         std::cerr << "Cannot find 'VirtualLinkTransmissionModel'." << std::endl;
         abort();
      }

      // TODO: add these control instances also to control keeper
      experiment_control_.init( sc, service_client_ );
      network_control_.init( sc, service_client_ );
      virtual_link_control_.init( sc, service_client_, vltm );
      node_control_.init( sc, service_client_, virtual_link_control_ );
      session_control_.init( sc, service_client_ );

      testbedservice_server_.start_server( sc );
      session_management_.start_server( sc );
      // wait for server to startup
      std::cout << "wait for server to startup (500ms)..." << std::endl;
      boost::this_thread::sleep( boost::posix_time::milliseconds( 500 ) );

      std::cout << "send test data to controller..." << std::endl;
      service_client_.send_test_data();
      std::cout << "ok" << std::endl;
   }

}
#endif
