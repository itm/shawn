/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/rectangle_topology_task.h"
#include "apps/topology/topology/rectangle_topology.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"

using namespace shawn;
using namespace reading;

namespace topology
{

   RectangleTopologyTask::
   RectangleTopologyTask()
   {}
   // ----------------------------------------------------------------------
   RectangleTopologyTask::
   ~RectangleTopologyTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   RectangleTopologyTask::
   name( void )
      const throw()
   { return "rectangle_topology"; }
   // ----------------------------------------------------------------------
   std::string
   RectangleTopologyTask::
   description( void )
      const throw()
   { return "creates a 2D rectangular topology named $name on [$x1,$x2] x [$y1,$y2]"; }
   // ----------------------------------------------------------------------
   void
   RectangleTopologyTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double x1 = sc.environment().required_double_param("x1");
      double x2 = sc.environment().required_double_param("x2");
      double y1 = sc.environment().required_double_param("y1");
      double y2 = sc.environment().required_double_param("y2");
      std::string n = sc.environment().optional_string_param("name","");

      if( x1 > x2 ) throw std::runtime_error("rectangle has negative X size");
      if( y1 > y2 ) throw std::runtime_error("rectangle has negative Y size");

      topology_keeper_w(sc).add( new RectangleTopology(Box(Vec(x1,y1,0.0),
                                                           Vec(x2,y2,0.0)),
                                                       n) );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/rectangle_topology_task.cpp,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: rectangle_topology_task.cpp,v $
 *-----------------------------------------------------------------------*/
