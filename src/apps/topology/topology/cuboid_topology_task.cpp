/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/cuboid_topology_task.h"
#include "apps/topology/topology/cuboid_topology.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"

using namespace shawn;
using namespace reading;

namespace topology
{

   CuboidTopologyTask::
   CuboidTopologyTask()
   {}
   // ----------------------------------------------------------------------
   CuboidTopologyTask::
   ~CuboidTopologyTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CuboidTopologyTask::
   name( void )
      const throw()
   { return "cuboid_topology"; }
   // ----------------------------------------------------------------------
   std::string
   CuboidTopologyTask::
   description( void )
      const throw()
   { return "creates a 3D cuboidal topology named $name on [$x1,$x2] x [$y1,$y2] x [$z1,$z2]"; }
   // ----------------------------------------------------------------------
   void
   CuboidTopologyTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double x1 = sc.environment().required_double_param("x1");
      double x2 = sc.environment().required_double_param("x2");
      double y1 = sc.environment().required_double_param("y1");
      double y2 = sc.environment().required_double_param("y2");
      double z1 = sc.environment().required_double_param("z1");
      double z2 = sc.environment().required_double_param("z2");
      std::string n = sc.environment().optional_string_param("name","");

      if( x1 > x2 ) throw std::runtime_error("cuboid has negative X size");
      if( y1 > y2 ) throw std::runtime_error("cuboid has negative Y size");
      if( z1 > z2 ) throw std::runtime_error("cuboid has negative Z size");

      topology_keeper_w(sc).add( new CuboidTopology(Box(Vec(x1,y1,z1),
                                                        Vec(x2,y2,z2)),
                                                    n) );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/cuboid_topology_task.cpp,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: cuboid_topology_task.cpp,v $
 *-----------------------------------------------------------------------*/
