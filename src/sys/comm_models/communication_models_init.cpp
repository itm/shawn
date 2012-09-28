/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/comm_models/communication_model_factory.h"
#include "sys/comm_models/communication_model_keeper.h"

#include "sys/comm_models/rim_comm_model_factory.h"
#include "sys/comm_models/disk_graph_communication_model_factory.h"
//#include "sys/comm_models/quasi_disk_graph_communication_model_factory.h"
#include "sys/comm_models/manual_communication_model_factory.h"
#include "sys/comm_models/permalink_communication_model_factory.h"
#include "sys/comm_models/link_probability_communication_model_factory.h"
#include "sys/comm_models/multiple_communication_model_factory.h"
#include "sys/comm_models/stochastic_comm_model_factory.h"

namespace shawn
{

   void init_communication_models( SimulationController& sc ) throw()
   {
        sc.communication_model_keeper_w().add( new RimCommModelFactory );
        sc.communication_model_keeper_w().add( new ManualCommunicationModelFactory );
        sc.communication_model_keeper_w().add( new DiskGraphCommunicationModelFactory );
        //        sc.communication_model_keeper_w().add( new QuasiDiskGraphCommunicationModelFactory );
        sc.communication_model_keeper_w().add( new MultipleCommunicationModelFactory );
        sc.communication_model_keeper_w().add( new PermalinkCommunicationModelFactory );
        sc.communication_model_keeper_w().add( new LinkProbabilityCommunicationModelFactory );
        sc.communication_model_keeper_w().add( new StochasticCommunicationModelFactory );
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_models_init.cpp,v $
 * Version $Revision: 114 $
 * Date    $Date: 2007-12-12 14:49:16 +0100 (Wed, 12 Dec 2007) $
 *-----------------------------------------------------------------------
 * $Log: communication_models_init.cpp,v $
 *-----------------------------------------------------------------------*/
