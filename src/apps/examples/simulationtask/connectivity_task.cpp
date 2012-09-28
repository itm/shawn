/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "apps/examples/examples_init.h"
#ifdef ENABLE_EXAMPLES

#include "apps/examples/simulationtask/connectivity_task.h"
#include "sys/communication_model.h"
#include "sys/edge_model.h"
#include "sys/world.h"
#include <fstream>
#include <limits>


namespace examples
{

   ConnectivityTask::
   ConnectivityTask()
   {}
   // ----------------------------------------------------------------------
   ConnectivityTask::
   ~ConnectivityTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   ConnectivityTask::
   name( void )
      const throw()
   {
      return "connectivity";
   }
   // ----------------------------------------------------------------------
   std::string
   ConnectivityTask::
   description( void )
      const throw()
   {
      return "get the average connectivity";
   }
   // ----------------------------------------------------------------------
   void
   ConnectivityTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      double connectivity = 0.0, count = 0.0;
      double min = std::numeric_limits<double>::max();
      double max = std::numeric_limits<double>::min();

      for( shawn::World::const_node_iterator
               it = sc.world().begin_nodes();
               it != sc.world().end_nodes();
               ++it )
      {
         double actual_conn = sc.world().edge_model().nof_adjacent_nodes( *it ) - 1;

         min = std::min(min, actual_conn);
         max = std::max(max, actual_conn);
         connectivity += actual_conn;
         
         count++;
      }

      double result = connectivity / count;
      INFO( logger(), "Mean connectivity: " << result );
      INFO( logger(), "Min connectivity: " << min );
      INFO( logger(), "Max connectivity: " << max );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/roemer_dummy/connectivity_task.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/06/07 20:02:03 $
 *-----------------------------------------------------------------------
 * $Log: connectivity_task.cpp,v $
 *-----------------------------------------------------------------------*/
