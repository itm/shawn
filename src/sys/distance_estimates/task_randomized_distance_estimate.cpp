/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/task_randomized_distance_estimate.h"
#include "sys/distance_estimates/randomized_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/misc/random/random_variable_keeper.h"
#include <limits>

namespace shawn
{
   SimulationTaskRandomizedDistanceEstimate::
   SimulationTaskRandomizedDistanceEstimate()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskRandomizedDistanceEstimate::
   ~SimulationTaskRandomizedDistanceEstimate()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskRandomizedDistanceEstimate::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string name =  sc.environment().optional_string_param("name","");
      std::string mult =  sc.environment().optional_string_param("multiplier","");
      std::string offset = sc.environment().optional_string_param("offset","");
      double chop = sc.environment().optional_double_param("chop_low", std::numeric_limits<double>::min());
      bool resample = sc.environment().optional_bool_param("resample_chopped",false);

      RandomizedDistanceEstimate *rde = new RandomizedDistanceEstimate;

      try {
         rde->set_chop_lower(chop);
         rde->set_resample_chopped(resample);
         if( !mult.empty() )
            rde->set_multiplier( sc.random_variable_keeper().find(mult) );
         if( !offset.empty() )
            rde->set_offset( sc.random_variable_keeper().find(offset) );
         if( !name.empty() )
            rde->set_name(name);
         rde->init();
      }
      catch( std::runtime_error& ) {
         delete rde;
         throw;
      }
    
      sc.distance_estimate_keeper_w().add(rde);
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskRandomizedDistanceEstimate::
   name( void )
      const throw()
   {
      return std::string("create_randomized_distance_estimate");
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskRandomizedDistanceEstimate::
   description( void )
      const throw()
   {
      return std::string("$multiplier $offset $chop_low $resample_chopped");
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/task_randomized_distance_estimate.cpp,v $
 * Version $Revision: 105 $
 * Date    $Date: 2007-11-13 08:53:43 +0100 (Tue, 13 Nov 2007) $
 *-----------------------------------------------------------------------
 * $Log: task_randomized_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
