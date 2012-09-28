/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/transm_models/transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/transm_models/task_chain_transm_model.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/transm_models/reliable_transmission_model_factory.h"
#include "sys/transm_models/stats_chain_transmission_model_factory.h"
#include "sys/transm_models/stats_chain_transmission_model_dump.h"
#include "sys/transm_models/random_drop_transmission_model_factory.h"
#include "sys/transm_models/aloha_transmission_model_factory.h"
#include "sys/transm_models/slotted_aloha_transmission_model_factory.h"
#include "sys/transm_models/maca_transmission_model_factory.h"
#include "sys/transm_models/csma_transmission_model_factory.h"
#include "sys/transm_models/zigbee_csma_transmission_model_factory.h"
#include "sys/transm_models/traces_transmission_model_factory.h"

namespace shawn
{

   void init_transmission_models( SimulationController& sc ) throw()
   {
      sc.transmission_model_keeper_w().add( new ReliableTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new TracesTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new StatsChainTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new RandomDropTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new AlohaTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new SlottedAlohaTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new MacaTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new CsmaTransmissionModelFactory() );
      sc.transmission_model_keeper_w().add( new ZigbeeCsmaTransmissionModelFactory() );
      
      sc.simulation_task_keeper_w().add( new SimulationTaskChainTransmModel() );
      sc.simulation_task_keeper_w().add( new StatsChainTransmissionModelDumpStats() );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/transmission_models_init.cpp,v $
 * Version $Revision: 356 $
 * Date    $Date: 2009-07-23 12:11:42 +0200 (Thu, 23 Jul 2009) $
 *-----------------------------------------------------------------------
 * $Log: transmission_models_init.cpp,v $
 *-----------------------------------------------------------------------*/
