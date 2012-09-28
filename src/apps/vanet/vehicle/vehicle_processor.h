/**
 * @file vehicle_processor.h
 * Defines vanet::VehicleProcessor.
 *
 * @author Walter Bamberger
 *
 * $Revision: 628 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/vehicle_processor.h $
 * $Id: vehicle_processor.h 628 2011-10-25 10:26:43Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_VEHICLE_PROCESSOR_H
#define __SHAWN_APPS_VANET_VEHICLE_PROCESSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../environment/information_description.h"
#include "sys/processor.h"
#include <vector>
#include <map>

namespace shawn
{
class SimulationController;
}

namespace reading
{
class Sensor;
}

namespace vanet
{

class Disseminator;
class KnowledgeBase;
class StatisticsTask;
class ValueTypeIntSensor;

/**
 * Processor that performs the information processing in the VANET like a moving
 * vehicle.
 */
class VehicleProcessor : public shawn::Processor
{
public:
   VehicleProcessor();

   virtual
   ~VehicleProcessor();

   virtual void
   boot( void ) throw ();

   virtual bool
   process_message( const shawn::ConstMessageHandle& ) throw ();

   virtual void
   work() throw ();

private:
   friend class StatisticsTask;
   friend class VehicleProcessorFactory;

   /// @todo Should better contain components and long-term configuration?
   /// So rename it?
   struct Components
   {
      KnowledgeBase* knowledgeBase;
      Disseminator* disseminator;
      Disseminator* recommendationDisseminator;
      ValueTypeIntSensor* sensor;
      std::map<InformationDescription::Type, std::vector<float> > sensorEvidenceLists;

      Components() :
               knowledgeBase(0), disseminator(0), recommendationDisseminator(0), sensor(
                        0)
      {
      }
   };

   /// @name Transient Configuration
   /// @{

   /**
    * Mathematical constant
    */
   static const double pi;
   const int disseminationPhase;
   const int disseminationModul;

   /// @}
   /// @name Components
   /// @{

   /**
    * Saves the vehicle configuration across several trips.
    *
    * After a trip, the vehicle processor is destroyed by the
    * shawn::NodeMovementCreator. The configuration of every vehicle is saved
    * in this map, so it can be restored if a vehicle processor with the same
    * label is created later again. It is like the persistent storage unit of
    * a real vehicle.
    */
   static std::map<std::string, Components> labelComponentsMap;
   ValueTypeIntSensor* sensor;
   KnowledgeBase* kb;
   Disseminator* disseminator;
   Disseminator* recommendationDisseminator;
   std::map<InformationDescription::Type, std::vector<float> > * sensorEvidenceListsPtr;

   /// @}
   /// @name Status
   /// @{

   int bootTime;

   /// @}

   /**
    * Determines the dissemination type and creates a disseminator accordingly.
    */
   static Disseminator*
   setupDisseminator( const std::string& vehicleLabel, shawn::Node& node,
            const KnowledgeBase& knowledgeBase ) throw ( std::runtime_error,
                     std::bad_alloc );

   /**
    * Creates a disseminator for recommendation.
    */
   static Disseminator*
   setupRecommendationDisseminator( shawn::Node& node,
            const KnowledgeBase& knowledgeBase ) throw ( std::runtime_error,
                     std::bad_alloc );
   /**
    * Creates a sensor, assigns it to the reading::Reading and determines its
    * error type.
    */
   ValueTypeIntSensor*
   setupSensor() throw ( std::runtime_error, std::bad_alloc );

   /**
    * Pre-computes the evidence distribution template #sensorEvidenceListsPtr.
    *
    * The probability distribution assigned to a perceived value of the sensor
    * depends on the error variance of the sensor. An intermediary result of the
    * probability distribution is pre-computed here once. It serves as a
    * template. With it, the probability distribution can easily be computed
    * later for every perception of the sensor value in #read_sensor_values().
    */
   void
   setupSensorEvidenceLists(
            const ValueTypeIntSensor* sensor,
            std::map<InformationDescription::Type, std::vector<float> >& evidenceLists );

   /**
    * Polls the sensor and saves the perceived value in the knowledge base.
    *
    * This method request a new sensor value. It drops the value if it is
    * already in the knowledge base. Otherwise it adds it to the knowledge base.
    */
   void
   read_sensor_values() throw ( std::out_of_range );
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_VEHICLE_PROCESSOR_H */
