/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSM_MODELS_RANDOM_DROP_FACTORY_H
#define __SHAWN_SYS_TRANSM_MODELS_RANDOM_DROP_FACTORY_H

#include <string>

#include "sys/transm_models/transmission_model_factory.h"

namespace shawn
{

    ///Creates instances of RandomDropTransmissionModel
    class RandomDropTransmissionModelFactory
        : public TransmissionModelFactory
    {
    public:
        static const std::string MODEL_NAME;

        RandomDropTransmissionModelFactory();
        virtual ~RandomDropTransmissionModelFactory();
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        virtual TransmissionModel* create( const SimulationController& ) throw();
    private:
        std::string descr_;

    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/random_drop_transmission_model_factory.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: random_drop_transmission_model_factory.h,v $
 *-----------------------------------------------------------------------*/
