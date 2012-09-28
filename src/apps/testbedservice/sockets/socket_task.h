/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SOCKET_TASK_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SOCKET_TASK_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/testbedservice/sockets/socket_server.h"
#include "apps/testbedservice/sockets/socket_client.h"

namespace testbedservice
{

   class SocketTask
      : public shawn::SimulationTask
   {
   public:
      SocketTask();
      virtual ~SocketTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   private:
      SocketServer socket_server_;
      SocketClient socket_client_;
   };

}

#endif
#endif
