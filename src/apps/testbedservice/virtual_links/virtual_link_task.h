/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TASK_H_
#define __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TASK_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/testbedservice/core/testbedservice_server.h"

namespace testbedservice
{

   class TestbedServiceVirtualLinkTask
      : public shawn::SimulationTask
   {
   public:
      TestbedServiceVirtualLinkTask();
      virtual ~TestbedServiceVirtualLinkTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   };

}

#endif
#endif
