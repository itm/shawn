/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/generator/populate_task.h"
#include "apps/topology/topology_keepers.h"
#include "apps/topology/point_gen/point_gen.h"
#include "apps/topology/point_gen/point_gen_keeper.h"
#include "apps/topology/node_gen/node_gen.h"
#include "apps/topology/node_gen/node_gen_keeper.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"
#include "apps/reading/readings/simple_reading.h"
#include <limits>

using namespace shawn;
using namespace reading;

namespace topology
{

   PopulateTask::
   PopulateTask()
   {}
   // ----------------------------------------------------------------------
   PopulateTask::
   ~PopulateTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PopulateTask::
   name( void )
      const throw()
   { return "populate"; }
   // ----------------------------------------------------------------------
   std::string
   PopulateTask::
   description( void )
      const throw()
   { return "..."; }
   // ----------------------------------------------------------------------
   void
   PopulateTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world(sc);

	  const reading::BoolReading& topo = base_topology(sc);
      PointGenerator& point_gen = point_generator(sc);
      NodeGenerator& node_gen = node_generator(sc);

      point_gen.pre_generate(sc,&topo);
      node_gen.pre_generate(sc,&topo);

      int count;
      if( point_gen.is_infinite_process() )
         count = sc.environment().required_int_param("count");
      else
         count = sc.environment().optional_int_param("count",
                                                     std::numeric_limits<int>::max());
      for( int i=0; i<count; ++i )
         {
            Vec pos;
            if( !point_gen.generate_point(pos) )
               break;
            //modify_point();
            Node* node = node_gen.generate_node(pos);
            if( node == NULL )
               break;
            // modify_node()

            //DEBUG( logger(), node->label() << " @ " << pos );
         }
      point_gen.post_generate();
      node_gen.post_generate();
   }
   // ----------------------------------------------------------------------
   const BoolReading&
   PopulateTask::
   base_topology( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string name = sc.environment().required_string_param("topology");
      ConstReadingHandle crh = topology_keeper(sc).find(name);
      const BoolReading* br =
         dynamic_cast<const BoolReading*>(crh.get());
      if( br == NULL )
         throw std::runtime_error(name+std::string(" is no valid topology"));
      return *br;
   }
   // ----------------------------------------------------------------------
   PointGenerator&
   PopulateTask::
   point_generator( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      return *point_gen_keeper_w(sc).find_w(sc.environment().optional_string_param("point_gen","uniform_auto"));
   }
   // ----------------------------------------------------------------------
   NodeGenerator&
   PopulateTask::
   node_generator( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      return *node_gen_keeper_w(sc).find_w(sc.environment().optional_string_param("node_gen","default"));
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/generator/populate_task.cpp,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: populate_task.cpp,v $
 *-----------------------------------------------------------------------*/
