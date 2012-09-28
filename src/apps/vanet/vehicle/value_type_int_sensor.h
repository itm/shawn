/**
 * @file value_type_int_sensor.h
 * Defines the sensor class vanet::ValueTypeIntSensor.
 *
 * @author Walter Bamberger
 *
 * $Revision: 642 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/value_type_int_sensor.h $
 * $Id: value_type_int_sensor.h 642 2011-12-08 14:46:37Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_VEHICLE_VALUE_TYPE_INT_SENSOR_H
#define __SHAWN_APPS_VANET_VEHICLE_VALUE_TYPE_INT_SENSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../environment/value_type_int_reading.h"
#include "apps/reading/sensors/sensor.h"
#include "sys/misc/random/normal_random_variable.h"

namespace vanet
{

/**
 * Sensor that reads an integer value and a type value
 * from a ValueTypeIntReading.
 *
 * This sensor detects events of various types in its environment. You can think
 * of an event as a speed limit sign, price of petrol at a petrol station, a
 * traffic jam or anything similar. (The events are described more precisely in
 * the documentation of the ValueTypeIntReading.) The type value represents the
 * type of the event (like a speed limit sign), the integer value an optional
 * value assigned with the type (like a maximum speed of 60 km/h).
 *
 * This sensor connects to a ValueTypeIntReading. The reading represents
 * the environment and the sensor takes up its values when there is an event.
 *
 * This sensor implements an additive Gaussian error model. A random value drawn
 * from a Gaussian random variable with zero mean is added to the integer value
 * received from the @c ValueTypeIntReading; the type value is not modified
 * (for simplification). The sensor determines the variance of the
 * Gaussian automatically.
 *
 * To ease the configuration of the error for many vehicles, error type and
 * configuration sets are introduced. Every sensor is of a certain error type.
 * It is given in the constructor. The error type specifies how well a sensor
 * can detect events of a certain event type. You can think of it as a
 * typification of the sensor quality. The private array
 * @c varianceConfigurations_ contains the mapping from error types to the
 * variances of the Gaussian random variable for every event type. While the
 * error type is determined randomly for every sensor during the simulation,
 * the user can specify configuration sets in the configuration file. Every
 * configuration set names a complete mapping of error types to variances.
 * Currently three configuration sets are implemented:
 *
 * <dl>
 * <dt>no_errors</dt><dd>The variances of all error types and event types are
 * zero.</dd>
 * <dt>equal_errors</dt><dd>The variances for all event types are equal within
 * a error type. So the error depends only on the sensor, not on the perceived
 * event.</dd>
 * <dt>mixed_errors</dt><dd>The variances depend on the error type and on the
 * event type.</dd>
 * </dl>
 *
 * You can choose one of these configuration sets in the Shawn configuration
 * file using the key @c sensor_error. For example, you could write
 * @code
 * sensor_error=mixed_errors
 * @endcode
 * somewhere before @c simulation is called (more precisely, before the first
 * ValueTypeIntSensor object is created).
 */
class ValueTypeIntSensor : public reading::Sensor
{
public:
   enum ErrorType
   {
      NoError, ErrorType1, ErrorType2, ErrorType3
   };

   /**
    * Initialises the sensor with the reading and the error variances defined in
    * the configuration.
    */
   ValueTypeIntSensor( const ValueTypeIntReading& reading,
            const shawn::SimulationController& simulationController,
            ErrorType errorType );

   ValueTypeIntSensor( const ValueTypeIntReading& reading,
            const shawn::SimulationController& simulationController );

   virtual
   ~ValueTypeIntSensor()
   {
   }

   virtual int
   getErrorType() const
   {
      return errorType_;
   }

   virtual double
   getErrorVariance( InformationDescription::Type type ) const
            throw ( std::out_of_range )
   {
      if ( type == InformationDescription::IntLong )
         return errorIntLong_.variance();

      else if ( type == InformationDescription::IntMedium )
         return errorIntMedium_.variance();

      else if ( type == InformationDescription::IntShort )
         return errorIntShort_.variance();

      else if ( type == InformationDescription::IntSituational )
         return errorIntSituational_.variance();

      else
         throw(std::domain_error(
                  "ValueTypeIntSensor: Invalid type in getErrorVariance"));
   }

   virtual void
   setNode( const shawn::Node& node )
   {
      nodePtr_ = &node;
   }

   triplet<shawn::Vec, InformationDescription::Type, int>
   value() const;

private:
   static const double varianceConfigurations_[][4][4];
   static const double meanConfigurations_[][4][4];
   const ValueTypeIntReading& reading_;
   const shawn::Node* nodePtr_;
   const ErrorType errorType_;
   shawn::NormalRandomVariable errorIntSituational_;
   shawn::NormalRandomVariable errorIntShort_;
   shawn::NormalRandomVariable errorIntMedium_;
   shawn::NormalRandomVariable errorIntLong_;

   void
   initRandomVariables(
            const shawn::SimulationController& simulationController );

   static ErrorType
   sample_error_type();
};

}

#endif
#endif /* __SHAWN_APPS_VANET_VEHICLE_VALUE_TYPE_INT_SENSOR_H */
