/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSM_MODEL_DUMP_STATS_H
#define __SHAWN_SYS_TRANSM_MODEL_DUMP_STATS_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{
   class SimulationController;
   class StatsChainTransmissionModel;
   class TransmissionModel;

   class StatsChainTransmissionModelDumpStats
      : public SimulationTask
   {
   public:

      ///@name Construction, destruction and lifecycle support
      ///@{
      StatsChainTransmissionModelDumpStats();
      virtual ~StatsChainTransmissionModelDumpStats();
      ///@}

      ///@name Task implementation
      ///@{
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      ///@}

   protected:
      const StatsChainTransmissionModel* find_stats_model( const TransmissionModel& ) const throw();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/stats_chain_transmission_model_dump.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: stats_chain_transmission_model_dump.h,v $
 *-----------------------------------------------------------------------*/
