/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_READING_SENSOR_KEEPER_H
#define __SHAWN_APPS_READING_SENSOR_KEEPER_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING
#include <map>
#include <string>
#include <stdexcept>

#include "shawn_config.h"
#include "sys/util/refcnt_pointer.h"
#include "apps/reading/sensors/sensor_factory.h"
#include "sys/util/handle_keeper.h"

namespace reading
{
    class SensorFactory;

    ///  Keeper that keeps SensorFactories.
    /**  The SensorKeeper is derived from the HandleKeeper class and
    * manages different types of SensorFactories.
    */
    class SensorKeeper
        : public shawn::HandleKeeper<SensorFactory>
    {
    public:
        ///@name construction / destruction
        ///@{
        ///
        SensorKeeper();
        ///
        virtual ~SensorKeeper();
        ///@}
    };
}

#endif
#endif

