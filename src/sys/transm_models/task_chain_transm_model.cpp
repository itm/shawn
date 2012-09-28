/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/world.h"
#include "sys/transmission_model.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/transm_models/chainable_transmission_model.h"
#include "sys/transm_models/task_chain_transm_model.h"

#include <string>

namespace shawn
{


    SimulationTaskChainTransmModel::
        SimulationTaskChainTransmModel()
    {}
    // ----------------------------------------------------------------------
    SimulationTaskChainTransmModel::
        ~SimulationTaskChainTransmModel()
    {}
    // ----------------------------------------------------------------------
    void
        SimulationTaskChainTransmModel::
        run( SimulationController& sc )
        throw( std::runtime_error )
    {
        TransmissionModel* tm = & sc.world_w().transmission_model_w();
        ChainableTransmissionModel* ctm = dynamic_cast<ChainableTransmissionModel*>(tm);

        if( ctm != NULL ) 
        {
            std::string new_transm_model_name = sc.environment().required_string_param("name");
            TransmissionModelKeeper& tmk = sc.transmission_model_keeper_w();
            TransmissionModelFactoryHandle tmfh = tmk.find_w( new_transm_model_name );
            TransmissionModel* tm = tmfh->create(sc);
            tm->set_world( sc.world_w() );
            tm->init();
            ctm->append_to_chain( *tm );
        } 
        else 
        {
            throw std::runtime_error("Chaining not possible. Current world transmission model is not chainable.");
        }


    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskChainTransmModel::
        name( void )
        const throw()
    {
        return std::string("chain_transm_model");
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskChainTransmModel::
        description( void )
        const throw()
    {
        return std::string("Appends a transmission model specified by $name to the current transmission model. "
                           "The current model must be of type ChainableTransmissionModel.");
    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/task_chain_transm_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: task_chain_transm_model.cpp,v $
 *-----------------------------------------------------------------------*/
