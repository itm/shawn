/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/transm_models/stats_chain_transmission_model_dump.h"
#include "sys/transm_models/stats_chain_transmission_model.h"

// #include "sys/transm_models/chainable_transmission_model.h"
// #include "sys/transm_models/task_chain_transm_model.h"

#include <string>

namespace shawn
{


   StatsChainTransmissionModelDumpStats::
   StatsChainTransmissionModelDumpStats()
   {}
   // ----------------------------------------------------------------------
   StatsChainTransmissionModelDumpStats::
   ~StatsChainTransmissionModelDumpStats()
   {}
   // ----------------------------------------------------------------------
   void
   StatsChainTransmissionModelDumpStats::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      const StatsChainTransmissionModel* sctm =
         find_stats_model(sc.world().transmission_model());
      if( sctm==NULL )
         {
            WARN( logger(),
                  "No stats_chain transmission model found -- not dumping." );
         }
      else
         sctm->dump_stats(std::cout);
   }
   // ----------------------------------------------------------------------
   std::string
   StatsChainTransmissionModelDumpStats::
   name( void )
      const throw()
   {
      return std::string("dump_transmission_stats");
   }
   // ----------------------------------------------------------------------
   std::string
   StatsChainTransmissionModelDumpStats::
   description( void )
      const throw()
   {
      return std::string("prints the transmission statististics, if a stats_chain transmission model is in use");
   }
   // ----------------------------------------------------------------------
   const StatsChainTransmissionModel*
   StatsChainTransmissionModelDumpStats::
   find_stats_model( const TransmissionModel& tmodel )
      const throw()
   {
      const TransmissionModel* tm = &tmodel;
      const ChainableTransmissionModel* ctm;
      const StatsChainTransmissionModel* sctm;

      while( (ctm=dynamic_cast<const ChainableTransmissionModel*>(tm))
             != NULL )
         {
            if( (sctm=dynamic_cast<const StatsChainTransmissionModel*>(ctm))
                != NULL )
               return sctm;
            if( !ctm->has_next_transm_model() )
               return NULL;
            tm = &ctm->next_transm_model();
         }
      return NULL;
   }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/stats_chain_transmission_model_dump.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: stats_chain_transmission_model_dump.cpp,v $
 *-----------------------------------------------------------------------*/
