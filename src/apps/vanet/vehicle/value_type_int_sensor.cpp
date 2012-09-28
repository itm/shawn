/**
 * @file value_type_int_sensor.cpp
 * Implements the sensor class vanet::ValueTypeIntSensor.
 *
 * @author Walter Bamberger
 *
 * $Revision: 648 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/value_type_int_sensor.cpp $
 * $Id: value_type_int_sensor.cpp 648 2012-04-30 14:49:32Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "value_type_int_sensor.h"
#ifdef ENABLE_VANET
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/basic_random.h"
#include <math.h>

namespace vanet
{

const double ValueTypeIntSensor::varianceConfigurations_[6][4][4] = { {
// No errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Equal errors (standard deviations 0.00, 0.35, 1.00, 1.50)
         { 0.000, 0.000, 0.000, 0.000 }, // Type 0
         { 0.125, 0.125, 0.125, 0.125 }, // Type 1
         { 1.000, 1.000, 1.000, 1.000 }, // Type 2
         { 2.250, 2.250, 2.250, 2.250 } // Type 3
}, {
// Large mixed errors
// (standard deviations 0.00, 0.40, 0.80, 1.20 and 0.00, 0.50, 1.00, 1.50)
         { 0.000, 0.000, 0.000, 0.000 }, // Type 0
         { 0.160, 0.160, 1.000, 0.160 }, // Type 1
         { 0.640, 0.640, 0.250, 0.640 }, // Type 2
         { 1.440, 1.440, 2.250, 1.440 } // Type 3
}, {
// Medium mixed errors (standard deviations 0.00, 0.33, 0.67 and 1.00)
         { 0.000, 0.000, 0.000, 0.000 }, // Type 0
         { 0.111, 0.111, 0.444, 0.111 }, // Type 1
         { 0.444, 0.444, 0.111, 0.444 }, // Type 2
         { 1.000, 1.000, 1.000, 1.000 } // Type 3
}, {
// Small mixed errors (standard deviations 0.00, 0.10, 0.20, 0.30)
         { 0.000, 0.000, 0.000, 0.000 }, // Type 0
         { 0.010, 0.010, 0.040, 0.010 }, // Type 1
         { 0.040, 0.040, 0.010, 0.040 }, // Type 2
         { 0.090, 0.090, 0.090, 0.090 } // Type 3
}, {
// Various errors
         { 0.000, 0.000, 0.000, 0.000 }, // Type 0
         { 0.250, 0.250, 1.000, 0.250 }, // Type 1
         { 1.000, 1.000, 0.250, 1.000 }, // Type 2
         { 0.250, 0.250, 0.250, 0.250 } // Type 3
} };

const double ValueTypeIntSensor::meanConfigurations_[6][4][4] = { {
// No errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Equal errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Large mixed errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Small mixed errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Medium mixed errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.0, 0.0, 0.0 } // Type 3
}, {
// Various errors
         { 0.0, 0.0, 0.0, 0.0 }, // Type 0
         { 0.0, 0.0, 0.0, 0.0 }, // Type 1
         { 0.0, 0.0, 0.0, 0.0 }, // Type 2
         { 0.0, 0.7, 0.7, 0.7 } // Type 3
} };

// ----------------------------------------------------------------------

ValueTypeIntSensor::ValueTypeIntSensor( const ValueTypeIntReading& reading,
         const shawn::SimulationController& simulationController,
         ErrorType errorType ) :
         reading_(reading), errorType_(errorType)
{
   initRandomVariables(simulationController);
}

// ----------------------------------------------------------------------

ValueTypeIntSensor::ValueTypeIntSensor( const ValueTypeIntReading& reading,
         const shawn::SimulationController& simulationController ) :
         reading_(reading), errorType_(sample_error_type())
{
   initRandomVariables(simulationController);
}

// ----------------------------------------------------------------------

void
ValueTypeIntSensor::initRandomVariables(
         const shawn::SimulationController& simulationController )
{
   static const std::string configString =
            simulationController.environment().optional_string_param(
                     "sensor_error", "no_errors");
   int configIndex = 0;
   if ( configString == "no_errors" )
      configIndex = 0;

   else if ( configString == "equal_errors" )
      configIndex = 1;

   else if ( configString == "large_mixed_errors" )
      configIndex = 2;

   else if ( configString == "medium_mixed_errors" )
      configIndex = 3;

   else if ( configString == "small_mixed_errors" )
      configIndex = 4;

   else if ( configString == "various_errors" )
      configIndex = 5;

   else
      throw(std::runtime_error(
               "Invalid value for configuration parameter sensor_error: "
                        + configString));

   errorIntSituational_.set_mean(
            meanConfigurations_[configIndex][errorType_][0]);
   errorIntSituational_.set_variance(
            varianceConfigurations_[configIndex][errorType_][0]);
   errorIntSituational_.init();

   errorIntShort_.set_mean(meanConfigurations_[configIndex][errorType_][1]);
   errorIntShort_.set_variance(
            varianceConfigurations_[configIndex][errorType_][1]);
   errorIntShort_.init();

   errorIntMedium_.set_mean(meanConfigurations_[configIndex][errorType_][2]);
   errorIntMedium_.set_variance(
            varianceConfigurations_[configIndex][errorType_][2]);
   errorIntMedium_.init();

   errorIntLong_.set_mean(meanConfigurations_[configIndex][errorType_][3]);
   errorIntLong_.set_variance(
            varianceConfigurations_[configIndex][errorType_][3]);
   errorIntLong_.init();
}

// ----------------------------------------------------------------------

ValueTypeIntSensor::ErrorType
ValueTypeIntSensor::sample_error_type()
{
   int random_value = shawn::uniform_discrete_random(5);
   if ( random_value <= 2 )
      return NoError;
   else
      return static_cast<ErrorType>(random_value - 2);
}

// ----------------------------------------------------------------------

triplet<shawn::Vec, InformationDescription::Type, int>
ValueTypeIntSensor::value() const
{
   // Get the sensor value from the Reading.
   triplet<shawn::Vec, InformationDescription::Type, int> valueTriplet =
            reading_.value(nodePtr_->real_position());

   // Add the additive Gaussian error.
   // errorX gives a random value of type double every time it is called.
   double modifiedValue;
   if ( valueTriplet.second == InformationDescription::IntLong )
      modifiedValue = static_cast<double>(valueTriplet.third) + errorIntLong_;
   else if ( valueTriplet.second == InformationDescription::IntMedium )
      modifiedValue = static_cast<double>(valueTriplet.third) + errorIntMedium_;
   else if ( valueTriplet.second == InformationDescription::IntShort )
      modifiedValue = static_cast<double>(valueTriplet.third) + errorIntShort_;
   else
      modifiedValue = static_cast<double>(valueTriplet.third)
               + errorIntSituational_;

   // Round the modified value mathematically correct to an integer
   // and limit it to its value range
   modifiedValue = round(modifiedValue);
   int maxValue = InformationDescription::maxIntValues[valueTriplet.second];
   if ( modifiedValue < 0 )
      valueTriplet.third = 0;
   else if ( modifiedValue > maxValue )
      valueTriplet.third = maxValue;
   else
      valueTriplet.third = static_cast<int>(modifiedValue);

   return valueTriplet;
}

}

#endif /* ENABLE_VANET */
