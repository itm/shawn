/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/edge_models/list_edge_model_factory.h"
#include "sys/edge_models/fast_list_edge_model_factory.h"
#include "sys/edge_models/lazy_edge_model_factory.h"
#include "sys/edge_models/grid_edge_model_factory.h"

namespace shawn
{

   void init_edge_models( SimulationController& sc ) throw()
   {
      sc.edge_model_keeper_w().add( new ListEdgeModelFactory );
      sc.edge_model_keeper_w().add( new LazyEdgeModelFactory );
      sc.edge_model_keeper_w().add( new GridEdgeModelFactory );
      sc.edge_model_keeper_w().add( new FastListEdgeModelFactory );
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_models/edge_models_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: edge_models_init.cpp,v $
 *-----------------------------------------------------------------------*/
