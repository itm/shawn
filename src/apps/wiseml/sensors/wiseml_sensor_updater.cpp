/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/sensors/wiseml_sensor_updater.h"
#ifdef ENABLE_WISEML
#include "apps/wiseml/sensors/wiseml_sensor_factory.h"
namespace wiseml
{
   WisemlSensorUpdater::
   WisemlSensorUpdater()
   {   
   }
   // ----------------------------------------------------------------------
   WisemlSensorUpdater::
   ~WisemlSensorUpdater()
   {
      for(map<string, WisemlRawSensor*>::iterator it = 
         reg_sensors_.begin(); it != reg_sensors_.end(); 
         ++it)
      {
         delete (it->second);
         remove_sensor(it->first);
      }
   }
   // ----------------------------------------------------------------------
   void 
   WisemlSensorUpdater::
   register_updater( SimulationController &sc )
   {
      reading::SensorKeeper *keep = 
         sc.keeper_by_name_w<reading::SensorKeeper>("SensorKeeper");
      for(SFIterator it = keep->begin_handles_w(); 
         it != keep->end_handles_w(); ++it)
      {
         WisemlSensorFactory *fact = 
            dynamic_cast<WisemlSensorFactory*>(it->second.get());

         if(fact != NULL)
         {
            fact->set_data_reader(this);
         }
      }
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   void 
   WisemlSensorUpdater::
   update_sensor( string sensor_id, string value )
   {
	   if(sensor_exists(sensor_id))
      {
         reg_sensors_[sensor_id]->set_raw_value(value);
      }
   }
   // ----------------------------------------------------------------------
   void
   WisemlSensorUpdater::
   remove_sensor(string sensor_id)
   {
      reg_sensors_.erase(sensor_id);
   }
   // ----------------------------------------------------------------------
   WisemlRawSensor*
   WisemlSensorUpdater::
   operator[](string sensor_id)
   {
      if(sensor_exists(sensor_id))
      {
         return reg_sensors_[sensor_id];
      }
      else
	   {
         WisemlRawSensor* sensor = new WisemlRawSensor();
		   reg_sensors_[sensor_id] = sensor;
         return sensor;
	   }
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   bool
   WisemlSensorUpdater::
   sensor_exists(string sensor_id)
   {
      reg_sensors_.insert(make_pair<string, WisemlRawSensor*>("test", NULL));
      if(reg_sensors_.empty())
         return false;
	   map<string, WisemlRawSensor*>::const_iterator it = 
		   reg_sensors_.find(sensor_id);

	   if(it!=reg_sensors_.end())
	   {
         return true;
	   }
      else
      {
         return false;
      }
   }
}

#endif
