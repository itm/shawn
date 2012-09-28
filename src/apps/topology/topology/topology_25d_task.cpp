/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/topology_25d_task.h"
#include "apps/topology/topology/topology_25d.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"

using namespace shawn;
using namespace reading;

namespace topology
{

   Topology25DTask::
   Topology25DTask()
   {}
   // ----------------------------------------------------------------------
   Topology25DTask::
   ~Topology25DTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   Topology25DTask::
   name( void )
      const throw()
   { return "topology_25d"; }
   // ----------------------------------------------------------------------
   std::string
   Topology25DTask::
   description( void )
      const throw()
   { return "creates a 2.5D topology named $name. base 2d topology is $base, elevation is $elevation. (optional) upper elevation is $upper_elevation."; }
   // ----------------------------------------------------------------------
   void
   Topology25DTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string out_n = sc.environment().required_string_param("name");
      std::string base_n = sc.environment().required_string_param("base");
      std::string elev_n = sc.environment().required_string_param("elevation");
      std::string upper_n = sc.environment().optional_string_param("upper_elevation","");

      double sens =
         sc.environment().optional_double_param("sensitivity",
                                                upper_n.empty() ? EPSILON : 0.0 );

      ReadingKeeper& rk = *sc.keeper_by_name_w<reading::ReadingKeeper>("ReadingKeeper");;
      ReadingHandle base_r = rk.find_w(base_n); assert( base_r.is_not_null() );
      BoolReadingHandle base = dynamic_cast<BoolReading*>(base_r.get());
      if( base == NULL )
         throw std::runtime_error(base_n + std::string(" cannot be a base topology -- it is no BoolReading") );


      ReadingHandle elev_r = rk.find_w(elev_n); assert( elev_r.is_not_null() );
      DoubleReadingHandle elev = dynamic_cast<DoubleReading*>(elev_r.get());
      if( elev == NULL )
         throw std::runtime_error(elev_n + std::string(" cannot be an elevation -- it is no DoubleReading") );


      DoubleReadingHandle upper=NULL;
      if( !upper_n.empty() )
         {
            ReadingHandle upper_r = rk.find_w(upper_n); assert( upper_r.is_not_null() );
            upper = dynamic_cast<DoubleReading*>(upper_r.get());
            if( upper == NULL )
               throw std::runtime_error(upper_n + std::string(" cannot be an (upper) elevation -- it is no DoubleReading") );
         }

      Topology25D* topo = new Topology25D;
      topo->set_base_topology(base);
      topo->set_elevation(elev);
      if( upper != NULL )
         topo->set_upper_elevation(upper);
      topo->set_name(out_n);
      topo->set_sensitivity(sens);
      topo->init();
      rk.add(topo);
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/topology_25d_task.cpp,v $
 * Version $Revision: 352 $
 * Date    $Date: 2009-05-28 12:13:50 +0200 (Thu, 28 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: topology_25d_task.cpp,v $
 *-----------------------------------------------------------------------*/
