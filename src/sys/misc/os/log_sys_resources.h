/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_OS_LOG_SYS_RESOURCES_H
#define __SHAWN_SYS_MISC_OS_LOG_SYS_RESOURCES_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{
   class SimulationController;

   class LogSysResourcesTask
      : public SimulationTask
   {
   public:
      LogSysResourcesTask();
      virtual ~LogSysResourcesTask();
      
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:
      void log_numeric( const std::string&, int ) const throw();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/log_sys_resources.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: log_sys_resources.h,v $
 *-----------------------------------------------------------------------*/
