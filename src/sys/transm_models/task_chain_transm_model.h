/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSM_MODEL_TASK_CHAIN_H
#define __SHAWN_SYS_TRANSM_MODEL_TASK_CHAIN_H

#include "sys/simulation/simulation_task.h"

namespace shawn
{
    class SimulationController;

    ///Appends a transmission model to the current chain of transmission models
    /** If the last transmission model in the chain is not of type ChainableTransmissionModel
    * the task fails with a std::runtime_error
    */
    class SimulationTaskChainTransmModel
        : public SimulationTask
    {
    public:

        ///@name Construction, destruction and lifecycle support
        ///@{
        SimulationTaskChainTransmModel();
        virtual ~SimulationTaskChainTransmModel();
        ///@}

        ///@name Task implementation
        ///@{
        virtual void run( SimulationController& ) throw( std::runtime_error );
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        ///@}
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/task_chain_transm_model.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: task_chain_transm_model.h,v $
 *-----------------------------------------------------------------------*/
