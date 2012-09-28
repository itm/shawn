/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology_init.h"
#include "sys/simulation/simulation_controller.h"

#include "apps/topology/elevation/elevation_init.h"
#include "apps/topology/generator/generator_init.h"
#include "apps/topology/node_gen/node_gen_init.h"
#include "apps/topology/node_mod/node_mod_init.h"
#include "apps/topology/point_gen/point_gen_init.h"
#include "apps/topology/point_mod/point_mod_init.h"
#include "apps/topology/topology/topology_topology_init.h"

#include "apps/topology/node_gen/node_gen_keeper.h"


extern "C" void init_topology( shawn::SimulationController& sc )
{
   if( sc.keeper_by_name<topology::NodeGeneratorKeeper>(topology::NodeGeneratorKeeper::KEEPER_NAME)
       == NULL )
      {
         topology::init_topology_elevation(sc);
         topology::init_topology_generator(sc);
         topology::init_topology_node_gen(sc);
         topology::init_topology_node_mod(sc);
         topology::init_topology_point_gen(sc);
         topology::init_topology_point_mod(sc);
         topology::init_topology_topology(sc);
      }
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology_init.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: topology_init.cpp,v $
 *-----------------------------------------------------------------------*/
