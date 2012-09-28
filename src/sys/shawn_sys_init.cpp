/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/shawn_sys_init.h"

#include "sys/simulation/shawn_simulation_controller.h"

namespace shawn
{
   ShawnSimulationController* shawn_init_sys( void ) throw()
   {
      ShawnSimulationController* sc = new ShawnSimulationController();
      return sc;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/shawn_sys_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: shawn_sys_init.cpp,v $
 *-----------------------------------------------------------------------*/
