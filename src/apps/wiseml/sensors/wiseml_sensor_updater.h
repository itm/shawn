/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SENSOR_UPDATER_H
#define __SHAWN_APPS_WISEML_SENSOR_UPDATER_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/sensors/wiseml_sensor.h"
#include "apps/wiseml/sensors/wiseml_raw_sensor.h"
#include "apps/reading/sensors/sensor_keeper.h"
#include "sys/simulation/simulation_controller.h"
 
using namespace shawn;
using namespace std;
namespace wiseml
{
   /** \brief Updates attached sensors
    * This class is not used directly, but inherited by WiseMlTimestamps.
    */
   class WisemlSensorUpdater
   {
   public:
      WisemlSensorUpdater();
      virtual ~WisemlSensorUpdater();
      /**
       * Iterates all WiseML sensor factories that are already on the sensor keeper
       * and registers itself as updating service.
       */
      void register_updater(SimulationController &sc);
      /**
       * Removes the raw sensor with the given id.
       */
      void remove_sensor( string sensor_id );
      /**
       * Updates the raw sensor <id> with the given value.
       */
      void update_sensor( string sensor_id, string value );
      /**
       * Returns the raw sensor with the given id. If not existing, a new raw
       * sensor ist created and returned.
       */
      WisemlRawSensor* operator[](string sensor_id);
      /**
       * Returns wether a sensor with given id exists or not.
       */
      bool sensor_exists(string sensor_id);
      /**
       * A map containing all raw sensors sorted by their id string.
       */
	   map<string, WisemlRawSensor*> reg_sensors_;

   protected:
      typedef map<string, RefcntPointer<reading::SensorFactory> >::iterator SFIterator;
   };
}

#endif
#endif
