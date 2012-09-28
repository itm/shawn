/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "apps/examples/examples_init.h"
#ifdef ENABLE_EXAMPLES

#ifndef __SHAWN_TUBSAPPS_CONNECTIVITY_TASK_H
#define __SHAWN_TUBSAPPS_CONNECTIVITY_TASK_H


#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/simulation/simulation_task.h"


namespace examples
{

   class ConnectivityTask
      : public shawn::SimulationTask
   {
   public:
      ConnectivityTask();
      virtual ~ConnectivityTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/roemer_dummy/connectivity_task.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2005/08/05 10:00:52 $
 *-----------------------------------------------------------------------
 * $Log: connectivity_task.h,v $
 *-----------------------------------------------------------------------*/
