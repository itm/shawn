/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_EXAMPLETASK_H
#define __SHAWN_APPS_EXAMPLETASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_EXAMPLES

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace exampletask
{
   class ExampleTask
      : public shawn::SimulationTask
   {
   public:
      ExampleTask();
      virtual ~ExampleTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: exampletask.h,v $
 *-----------------------------------------------------------------------*/
