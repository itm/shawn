/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/sys_tasks_init.h"
#include "sys/simulation/basic_tasks.h"
#include "sys/worlds/world_tasks.h"
#include "sys/misc/misc_init.h"
#include "sys/edge_models/edge_models_init.h"
#include "sys/transm_models/transmission_models_init.h"
#include "sys/comm_models/communication_models_init.h"
#include "sys/misc/random/basic_random.h"
#include "sys/distance_estimates/distance_estimates_init.h"
#include "sys/taggings/taggings_init.h"
#include "sys/node_movements/node_movements_init.h"
#include "sys/logging/logging_init.h"
#include "sys/xml/xml_init.h"

//#include "sys/simulation/small/small_simulation_controller.h"

namespace shawn
{

   void init_sys_tasks(SimulationController& sc ) throw()
   {
      init_random_sequence();
      init_basic_tasks( sc );
      init_world_tasks( sc );
      init_misc( sc );
      init_edge_models( sc );
      init_transmission_models( sc );
      init_communication_models( sc );
      init_distance_estimates( sc );
      init_taggings( sc );
      init_node_movements( sc );
      init_logging( sc );
      init_xml( sc );
   }
}/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/sys_tasks_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: sys_tasks_init.cpp,v $
 *-----------------------------------------------------------------------*/
