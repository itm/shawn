/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TRANSMISSION_MODEL_FACTORY_H_
#define __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_TRANSMISSION_MODEL_FACTORY_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include <string>
#include "sys/transm_models/transmission_model_factory.h"

namespace testbedservice
{

    ///Creates instances of VirtualLinkTransmissionModelFactory
    class VirtualLinkTransmissionModelFactory
        : public shawn::TransmissionModelFactory
    {
    public:
        VirtualLinkTransmissionModelFactory();
        virtual ~VirtualLinkTransmissionModelFactory();
        virtual std::string name( void ) const throw();
        virtual std::string description( void ) const throw();
        virtual shawn::TransmissionModel* create( const shawn::SimulationController& ) throw();
    };

}

#endif
#endif
