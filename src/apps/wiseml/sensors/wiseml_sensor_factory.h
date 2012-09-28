/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __WISEML_SENSORFACTORY_H
#define __WISEML_SENSORFACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_WISEML
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/sensors/sensor_factory.h"
#include "apps/reading/sensors/sensor.h"
#include "apps/wiseml/sensors/wiseml_sensor_updater.h"
#include "sys/node.h"

namespace wiseml
{
   /** 
    * Base class for all WiseML sensor factories.
    */
   class WisemlSensorFactory
         : public reading::SensorFactory
   {
   public:
      WisemlSensorFactory( void );
      virtual ~WisemlSensorFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void set_data_reader( WisemlSensorUpdater *data_reader);
      
   protected:
      WisemlSensorUpdater *data_reader_;
   };
   DECLARE_HANDLES(WisemlSensorFactory);
}
#endif
#endif
