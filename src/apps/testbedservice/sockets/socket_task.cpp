/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/sockets/socket_task.h"
#include "sys/processors/processor_keeper.h"
#include "sys/worlds/processor_world_factory.h"
#include "sys/misc/random/basic_random.h"
#include "sys/logging/logger.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include "sys/processor.h"


namespace testbedservice
{

   SocketTask::
   SocketTask()
   {}
   // ----------------------------------------------------------------------
   SocketTask::
   ~SocketTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   SocketTask::
   name( void )
      const throw()
   {
      return "socket_task";
   }
   // ----------------------------------------------------------------------
   std::string
   SocketTask::
   description( void )
      const throw()
   {
      return "Start socket server for remote access; connection to portal server";
   }
   // ----------------------------------------------------------------------
   void
   SocketTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );
//       socket_server_.start_server( sc );
//       socket_client_.start_client( sc );
   }

}
#endif
