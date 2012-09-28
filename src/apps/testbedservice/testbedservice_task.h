/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_TASK_H_
#define __SHAWN_APPS_TESTBEDSERVICE_TASK_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/testbedservice/core/testbedservice_server.h"
#include "apps/testbedservice/core/session_management.h"
#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/ws_handler/experiment_control.h"
#include "apps/testbedservice/ws_handler/network_control.h"
#include "apps/testbedservice/ws_handler/node_control.h"
#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "apps/testbedservice/ws_handler/session_control.h"

namespace testbedservice
{

   class TestbedServiceTask
      : public shawn::SimulationTask
   {
   public:
      TestbedServiceTask();
      virtual ~TestbedServiceTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
      // --------------------------------------------------------------------
      inline const TestbedServiceClient& testbedservice_client( void ) throw()
      { return service_client_; }
      // --------------------------------------------------------------------
      inline TestbedServiceClient& testbedservice_client_w( void ) throw()
      { return service_client_; }

   private:
      TestbedServiceServer testbedservice_server_;
      SessionManagement session_management_;
      TestbedServiceClient service_client_;

      ExperimentControl experiment_control_;
      NetworkControl network_control_;
      NodeControl node_control_;
      VirtualLinkControl virtual_link_control_;
      SessionControl session_control_;
   };

}

#endif
#endif
