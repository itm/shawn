/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANSM_MODELS_SLOTTED_ALOHA_TRANSMISSION_MODEL_FACTORY_H
#define __SHAWN_SYS_TRANSM_MODELS_SLOTTED_ALOHA_TRANSMISSION_MODEL_FACTORY_H

#include <string>

#include "sys/transm_models/transmission_model_factory.h"

namespace shawn
{

    ///Creates instances of SlottedAlohaTransmissionModel
    class SlottedAlohaTransmissionModelFactory
        : public TransmissionModelFactory
    {
    public:
        SlottedAlohaTransmissionModelFactory();
        virtual ~SlottedAlohaTransmissionModelFactory();
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        virtual TransmissionModel* create( const SimulationController& ) throw();
    };

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/transm_models/slotted_aloha_transmission_model_factory.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: slotted_aloha_transmission_model_factory.h,v $
 *-----------------------------------------------------------------------*/
