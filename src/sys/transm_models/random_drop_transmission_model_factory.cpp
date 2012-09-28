/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/random_drop_transmission_model.h"
#include "sys/transm_models/random_drop_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

#include <sstream>

namespace shawn
{
    const std::string RandomDropTransmissionModelFactory::MODEL_NAME("random_drop_chain");

    RandomDropTransmissionModelFactory::
        RandomDropTransmissionModelFactory()
    {
        std::ostringstream oss;
        oss << "A RandomDropTransmissionModelFactory (\""<< MODEL_NAME <<
            "\") drops messages with a given $probability (0.0 to 1.0)";

        descr_ = oss.str();
    }

    // ----------------------------------------------------------------------
    RandomDropTransmissionModelFactory::
        ~RandomDropTransmissionModelFactory()
    {}

    // ----------------------------------------------------------------------
    std::string
        RandomDropTransmissionModelFactory::
        name( void )
        const throw()
    {
        return MODEL_NAME;
    }

    // ----------------------------------------------------------------------
    std::string
        RandomDropTransmissionModelFactory::
        description( void ) 
        const throw()
    {
        return descr_;
    }

    // ----------------------------------------------------------------------
    TransmissionModel*
        RandomDropTransmissionModelFactory::
        create( const SimulationController& )
        throw()
    {
        return new RandomDropTransmissionModel;
    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/random_drop_transmission_model_factory.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: random_drop_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
