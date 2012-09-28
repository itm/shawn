/**
 * @file vehicle_processor.cpp
 * Implements vanet::VehicleProcessor.
 *
 * @author Walter Bamberger
 *
 * $Revision: 633 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/vehicle_processor.cpp $
 * $Id: vehicle_processor.cpp 633 2011-11-11 09:57:15Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "vehicle_processor.h"
#ifdef ENABLE_VANET

#include "value_type_int_sensor.h"
#include "../dissemination/modifying_disseminator.h"
#include "../dissemination/observation_disseminator.h"
#include "../dissemination/recommendation_disseminator.h"
#include "../knowledge_base/knowledge_base.h"
#include "../vanet_message.h"
#include "../utility.h"
#include "apps/reading/readings/reading_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/tokenize.h"
#include "sys/node.h"
#include <boost/tokenizer.hpp>
#include <algorithm>

using namespace boost;
using namespace std;
using namespace shawn;
using namespace reading;

namespace vanet
{

const double VehicleProcessor::pi = 3.14159265358979323846;
map<string, VehicleProcessor::Components> VehicleProcessor::labelComponentsMap;

// ----------------------------------------------------------------------

VehicleProcessor::VehicleProcessor() :
         kb(0), disseminationModul(5), disseminationPhase(
                  uniform_discrete_random(4))
{
   DEBUG(logger(), "VehicleProcessor ctor");
}

// ----------------------------------------------------------------------

VehicleProcessor::~VehicleProcessor()
{
   DEBUG(logger(), "VehicleProcessor dtor");
   kb->prune();
}

// ----------------------------------------------------------------------

void
VehicleProcessor::boot( void ) throw ()
{
   const string& vehicleLabel = owner().label();
   const SimulationController& simulationController =
            owner().world().simulation_controller();

#if defined(ENABLE_LOGGING)
   set_logger_name("processor." + vehicleLabel);
#endif
   DEBUG(
            logger(),
            "Boot at " << owner().real_position() << " with disseminationPhase " << disseminationPhase);
   // Remember when the vehicle started its trip
   bootTime = owner().simulation_round();

   map<string, Components>::iterator foundIt = labelComponentsMap.find(
            vehicleLabel);
   if ( foundIt == labelComponentsMap.end() )
   {
      Components components;

      // All components are deleted in the VehicleProcessorFactory in the end
#if defined(ENABLE_LOGGING)
      components.knowledgeBase = new KnowledgeBase(simulationController,
               vehicleLabel, logger());
#else
      components.knowledgeBase = new KnowledgeBase(simulationController,
               vehicleLabel, __LOGGER_OBJECT());
#endif
      /// @todo May throw an exception
      components.disseminator = setupDisseminator(vehicleLabel, owner_w(),
               *components.knowledgeBase);
      components.recommendationDisseminator = setupRecommendationDisseminator(
               owner_w(), *components.knowledgeBase);
      /// @todo May throw an exception
      components.sensor = setupSensor();

      setupSensorEvidenceLists(components.sensor,
               components.sensorEvidenceLists);

      pair<map<string, Components>::iterator, bool> insertResult =
               labelComponentsMap.insert(make_pair(vehicleLabel, components));
      if ( insertResult.second == true )
      {
         foundIt = insertResult.first;
      }
      else
      {
         /// @todo I may not throw an exception in boot.
         throw(runtime_error(
                  "Initial creation of vehicle components failed (id: "
                           + vehicleLabel + ")"));
      }
   }

   Components& components = foundIt->second;
   disseminator = components.disseminator;
   recommendationDisseminator = components.recommendationDisseminator;
   disseminator->setNode(owner_w());
   recommendationDisseminator->setNode(owner_w());
   kb = components.knowledgeBase;
   //   kb->setLogger(logger());
   sensor = components.sensor;
   sensor->setNode(owner());
   sensorEvidenceListsPtr = &components.sensorEvidenceLists;
}

// ----------------------------------------------------------------------

bool
VehicleProcessor::process_message( const ConstMessageHandle& mh ) throw ()
{
   // First, test whether this message has been send by me. I am only
   // interested in messages of other cars.
   if ( mh->source().id() != owner().id() )
   {
      // Test for the type of the message
      const VanetIntMessage* intMsg =
               dynamic_cast<const VanetIntMessage*>(mh.get());
      if ( intMsg != 0 )
      {
         // Received a VanetIntMessage from another car.
         // Save it as a report in the knowledge base.
         DEBUG(logger(),
                  "Received a VANET message from " << mh->source().label());
         try
         {
            // Record a meeting for sender
            kb->registerMeeting(intMsg->source().label(),
                     owner().simulation_round());

            IntReport* r = new IntReport(owner().simulation_round(),
                     owner().real_position(), *intMsg);
            kb->addReport(r);
         }
         catch ( const runtime_error& e )
         {
            ERROR(logger(), e.what());
         }
         return true; // VehicleProcessor recognised this message type.
      }
      else
      {
         const VanetTrustMessage* trustMsg =
                  dynamic_cast<const VanetTrustMessage*>(mh.get());
         if ( trustMsg != 0 )
         {
            // Received a VanetTrustMessage from another car.
            // Save it as a trustReport in the knowledge base.
            try
            {
               TrustReport* tr = new TrustReport(owner().simulation_round(),
                        owner().real_position(), *trustMsg);
               kb->addReport(tr);
            }
            catch ( const runtime_error& e )
            {
               ERROR(logger(), e.what());
            }
            return true; // VehicleProcessor recognised this message type.
         }
         else
            DEBUG(logger(),
                     "Received an unknown message type from another car.");
      }
   }

   return false; // VehicleProcessor did not recognise this message type.
}

// ----------------------------------------------------------------------

void
VehicleProcessor::work() throw ()
{
   DEBUG(logger(), "at " << owner().real_position());
   read_sensor_values();
   if ( simulation_round() % disseminationModul == disseminationPhase )
   {
      DEBUG(logger(), "Disseminate");
      disseminator->disseminate();
      recommendationDisseminator->disseminate();
   }
}

// ----------------------------------------------------------------------

void
VehicleProcessor::read_sensor_values( void ) throw ( out_of_range )
{
   triplet<shawn::Vec, InformationDescription::Type, int> sensorValue =
            sensor->value();

   // The ValueTypeIntSensor detects events. There may be no event
   // at the current location (IntNone). Only if there is an event,
   // we save it as an observation.
   if ( sensorValue.second != InformationDescription::IntNone )
   {
      // The doublet (location, type) identifies a specific property of
      // the world. Look whether I observed this property already.
      // (The dynamic cast returns 0, if it fails.)
      IntObservation * newest =
               dynamic_cast<IntObservation*>(kb->findNewestObservationAbout(
                        sensorValue.first, sensorValue.second));

      // If I observed the property already and if the newest observation
      // was within the current trip and if the value did not change,
      // then I say it is still the same observation.
      if ( newest && newest->time >= bootTime
               && newest->value == sensorValue.third )
      {
         // It is still the same observation. So I only update the observation time.
         newest->time = owner().simulation_round();
         INFO(
                  logger(),
                  "Observed an event it already knows. t:" << newest->time << ", l:" << newest->location << ", m:" << newest->type << ", v:" << newest->value);
      }
      else
      {
         // It is a new observation. So I add it to the knowledge base.

         /*
          * Create the new observation object and copy the attribute of the
          * sensor value.
          */
         IntObservation* o = new IntObservation();
         o->time = owner().simulation_round();
         o->location = sensorValue.first;
         o->type = sensorValue.second;
         o->value = sensorValue.third;

         /*
          * Fill the opinion. It is similar to a probability distribution,
          * though it is not. I fill an evidence vector here. But following
          * Bamberger et al. (2010), the total evidence for one observation
          * should be 1. So the sum over the evidence vector should be one.
          * (This is similar to a probability distribution.)
          *
          * Creating the distribution involves the following steps: Take the
          * distribution template for the right information type from the
          * sensorEvidenceLists. It has its maximum evidence at the element 4.
          * Shift the distribution template in the way that the maximum
          * evidence is assigned to the assumed right value – the value
          * received from the sensor (o->value). Finally normalise the
          * distribution so that the sum of evidence is 1.
          */
         // May throw out_of_range exception.
         const vector<float>& distributionTemplate = sensorEvidenceListsPtr->at(
                  o->type);
         vector<float>::const_iterator templateStartIt =
                  distributionTemplate.begin() + (4 - o->value);
         copy(templateStartIt, templateStartIt + 5, o->opinion.beginEvidence());
         normalizeSum(o->opinion.beginEvidence(), o->opinion.endEvidence(),
                  1.0f);

         kb->addObservation(o);
         INFO(
                  logger(),
                  "Observed a new event and added it to the knowledge base: t:" << o->time << ", l:" << o->location << ", m:" << o->type << ", v:" << o->value << ", o:" << o->opinion);
      }
   }
}

// ----------------------------------------------------------------------

Disseminator*
VehicleProcessor::setupDisseminator( const string& vehicleLabel, Node& node,
         const KnowledgeBase& knowledgeBase ) throw ( runtime_error, bad_alloc )
{
   const SimulationController& simulationController =
            node.world().simulation_controller();

   // Malicious dissemination
   static const tokenizer<> modifyingDisseminatorList(
            simulationController.environment().optional_string_param(
                     "modifying_disseminator", ""));

   if ( find(modifyingDisseminatorList.begin(), modifyingDisseminatorList.end(),
            vehicleLabel) != modifyingDisseminatorList.end() )
   {
      // It is deleted in the VehicleProcessorFactory in the end
      return new ModifyingDisseminator(node, knowledgeBase);
   }
   else
   {
      // It is deleted in the VehicleProcessorFactory in the end
      return new ObservationDisseminator(node, knowledgeBase);
   }

}

// ----------------------------------------------------------------------

Disseminator*
VehicleProcessor::setupRecommendationDisseminator( Node& node,
         const KnowledgeBase& knowledgeBase ) throw ( runtime_error, bad_alloc )
{
   return new RecommendationDisseminator(node, knowledgeBase);
}
// ----------------------------------------------------------------------

ValueTypeIntSensor*
VehicleProcessor::setupSensor() throw ( runtime_error, bad_alloc )
{
   const SimulationController& simulationController =
            owner().world().simulation_controller();

   const ReadingKeeper* readingKeeperPtr = simulationController.keeper_by_name<
            ReadingKeeper>("ReadingKeeper");
   if ( readingKeeperPtr )
   {
      // If there is no ValueTypeIntReading in the reading keeper, find throws
      // a runtime_error exception.
      const ValueTypeIntReading * testIntegerReadingPtr =
               dynamic_cast<const ValueTypeIntReading*>(readingKeeperPtr->find(
                        "ValueTypeIntReading").get());
      if ( testIntegerReadingPtr )
      {
         static tokenizer<> modifyingDisseminators(
                  simulationController.environment().optional_string_param(
                           "modifying_disseminator", ""));
         if ( find(modifyingDisseminators.begin(), modifyingDisseminators.end(),
                  owner().label()) != modifyingDisseminators.end() )
         {
            // The sensor is deleted in the VehicleProcessorFactory in the end
            DEBUG(logger(), "Set up sensors for modifying disseminator");
            return new ValueTypeIntSensor(*testIntegerReadingPtr,
                     simulationController, ValueTypeIntSensor::NoError);
         }
         else
         {
            // The sensor is deleted in the VehicleProcessorFactory in the end
            return new ValueTypeIntSensor(*testIntegerReadingPtr,
                     simulationController);
         }
      }
      else
      {
         throw(runtime_error(
                  "Conversion of reading to ValueTypeIntReading failed."));
      }
   }
   else
   {
      throw(runtime_error("ReadingKeeper not found"));
   }
}

// ----------------------------------------------------------------------

void
VehicleProcessor::setupSensorEvidenceLists( const ValueTypeIntSensor* sensor,
         map<InformationDescription::Type, vector<float> >& evidenceLists )
{
   const InformationDescription::Type typeSet[4] = {
            InformationDescription::IntSituational,
            InformationDescription::IntShort, InformationDescription::IntMedium,
            InformationDescription::IntLong };
   double variance = 0.0;

   /* Compute the distribution template for every type of information. The
    * sensors may have different error variances depending on the type of
    * information. So different distribution templates must be computed for
    * every information type. */
   for ( const InformationDescription::Type* typePtr = typeSet;
            typePtr != typeSet + 4; ++typePtr )
   {
      /* Initialise the vector with 9 elements, all zero. Nine elements are
       * used, because the value range of the all information types goes from 0
       * to 4. With nine elements, I can realise every possible shape just with
       * moving. */
      vector<float>& distribution = evidenceLists[*typePtr];
      distribution.assign(9, 0.0);

      /* Fill the vector with a normal distribution depending on the variance.
       * The mean is always the element 4. */
      variance = sensor->getErrorVariance(*typePtr);
      if ( variance == 0.0 )
      {
         distribution[4] = 1.0;
      }
      else
      {
         for ( int i = 0; i < 9; ++i )
         {
            /* Compute the sampling points of the normal distribution. This will
             * not result in a probability distribution; the sum of elements
             * will not be 1. The idea is that the sensor has a Gaussian
             * distributed error in the continuous domain. After sampling
             * (quantifying) the sensor value, the error distribution has a
             * curve similar to the one computed below (sufficiently similar,
             * but not the same). The necessary normalisation happens when
             * obtaining the sensor value in #read_sensor_values(). */
            distribution[i] = (1.0 / sqrt(2.0 * pi * variance))
                     * exp(-(4.0 - i) * (4.0 - i) / (2.0 * variance));
         }
      }
   }
}

}

#endif
