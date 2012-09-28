/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/os/log_sys_resources.h"
#include "sys/misc/os/sys_resources.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/simulation/simulation_task.h"


namespace shawn
{

   LogSysResourcesTask::
   LogSysResourcesTask()
   {}
   // ----------------------------------------------------------------------
   LogSysResourcesTask::
   ~LogSysResourcesTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   LogSysResourcesTask::
   name( void )
      const throw()
   { return std::string("log_sys_resources"); }
   // ----------------------------------------------------------------------
   std::string
   LogSysResourcesTask::
   description( void )
      const throw()
   { return std::string("prints out a summary on the system resources used up so far"); }
   // ----------------------------------------------------------------------
   void
   LogSysResourcesTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      log_numeric("Memory usage (MB):   ", SystemResources::instance().memory_usage_mbytes());
      log_numeric("User time (s):       ", SystemResources::instance().user_time_seconds());
      log_numeric("System time (s):     ", SystemResources::instance().system_time_seconds());
      log_numeric("Total time (s):      ", SystemResources::instance().time_seconds());
      log_numeric("Wall clock time (s): ", SystemResources::instance().wc_time_seconds());
   }
   // ----------------------------------------------------------------------
   void
   LogSysResourcesTask::
   log_numeric( const std::string& s, int v )
      const throw()
   {
      if( v<0 )
         { INFO( logger(), s << "(unknown)" ); }
      else
         { INFO( logger(), s << v ); }
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/log_sys_resources.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: log_sys_resources.cpp,v $
 *-----------------------------------------------------------------------*/
