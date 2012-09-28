/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_task.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/tokenize.h"
#include "sys/logging/logger.h"
#include "sys/world.h"
#include "sys/processor.h"


namespace testbedservice
{

   TestbedServiceVirtualLinkTask::
   TestbedServiceVirtualLinkTask()
   {}
   // ----------------------------------------------------------------------
   TestbedServiceVirtualLinkTask::
   ~TestbedServiceVirtualLinkTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceVirtualLinkTask::
   name( void )
      const throw()
   {
      return "webservice_virtual_link_task";
   }
   // ----------------------------------------------------------------------
   std::string
   TestbedServiceVirtualLinkTask::
   description( void )
      const throw()
   {
      return "Add/remove virtual links.";
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceVirtualLinkTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      const shawn::SimulationEnvironment& se = sc.environment();
      std::string action = se.required_string_param( "action" );
      std::string shawn_urn = se.required_string_param( "shawn_node_urn" );
      std::string virtual_urn = se.required_string_param( "virtual_node_urn" );
      std::string remote_uri = se.required_string_param( "remote_uri" );

      shawn::ChainableTransmissionModel *ctm =
         dynamic_cast<shawn::ChainableTransmissionModel*>
         ( &sc.world_w().transmission_model_w() );

      if ( !ctm )
         throw( std::runtime_error( "VirtualLinkTransmissionModel not found." ) );

      while ( ctm )
      {
         VirtualLinkTransmissionModel *vltm =
            dynamic_cast<VirtualLinkTransmissionModel*>( ctm );
         if ( vltm )
         {
            if ( action == "add" )
               vltm->add_virtual_link( shawn_urn, virtual_urn, remote_uri );
            if ( action == "remove" )
               vltm->remove_virtual_link( shawn_urn, virtual_urn );

            // Transmission Model found, so end while loop
            break;
         }

         if ( ctm->has_next_transm_model() )
            ctm = dynamic_cast<shawn::ChainableTransmissionModel*>
                  ( &ctm->next_transm_model_w() );
         else
            throw( std::runtime_error( "VirtualLinkTransmissionModel not found." ) );
      }
   }

}
#endif
