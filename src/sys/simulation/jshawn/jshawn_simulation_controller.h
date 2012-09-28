/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "shawn_config.h"

#ifndef __SHAWN_SYS_SIMULATION_JSHAWN_JSHAWN_SIMULATION_CONTROLLER_H
#define __SHAWN_SYS_SIMULATION_JSHAWN_JSHAWN_SIMULATION_CONTROLLER_H

#include "sys/simulation/shawn_simulation_controller.h"
#include <string>
#include <iostream>

using namespace std;

namespace shawn
{
   /** 
   */
   class JShawnSimulationController : public ShawnSimulationController
   {

   public:
	   virtual void execute_task(SimulationTaskHandle) throw( std::runtime_error );

   private:
	   virtual void start_module(std::ostream&, std::string);
	   virtual void end_module(std::ostream&, std::string);
   };
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/jshawn/jshawn_simulation_controller.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: jshawn_simulation_controller.h,v $
 *-----------------------------------------------------------------------*/
