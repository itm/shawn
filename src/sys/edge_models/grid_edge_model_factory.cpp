/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/grid_edge_model_factory.h"
#include "sys/edge_models/grid_edge_model.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   GridEdgeModelFactory::
   GridEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   GridEdgeModelFactory::
   ~GridEdgeModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   GridEdgeModelFactory::
   name( void )
      const throw()
   {
      return std::string("grid");
   }
   // ----------------------------------------------------------------------
   std::string
   GridEdgeModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A GridEdgeModel");
   }
   // ----------------------------------------------------------------------
   EdgeModel*
   GridEdgeModelFactory::
   create( const SimulationController& sc )
      throw()
   {
      GridEdgeModel* gem = new GridEdgeModel;
      gem->set_initial_size( sc.environment().optional_int_param( GridEdgeModel::INITIAL_GRID_SIZE_PARAMETER, 8 ) );
      gem->set_transmission_scale( sc.environment().optional_double_param( GridEdgeModel::INITIAL_CELL_SIZE_PARAMETER, 1.5 ) );
      gem->set_closeness_fraction( sc.environment().optional_double_param( GridEdgeModel::CLOSENESS_FRACTION_PARAMETER,.01) );
      return gem;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/grid_edge_model_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: grid_edge_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
