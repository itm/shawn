/**
 * @file observation.h
 * Defines the data containers vanet::Observation and vanet::IntObservation
 * together with their lists vanet::ObservationList and
 * vanet::IntObservationList.
 *
 * @author Walter Bamberger
 *
 * $Revision: 526 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/observation.h $
 * $Id: observation.h 526 2011-04-11 18:23:46Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_OBSERVATION_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_OBSERVATION_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../opinion.h"
#include "../environment/information_description.h"
#include "sys/vec.h"
#include <set>

namespace vanet
{

/**
 * Record of a piece of information perceived with the vehicle's sensor.
 *
 * An observation is made at a certain point in space (#location) and at a
 * certain point in time (#time). It is described by the kind of the value
 * (#type), the value itself (defined in the sub classes) and the vehicles
 * opinion about this piece of information (also defined in the sub class).
 *
 * This class is just an interface. Sub classes must be defined for various
 * data types of the value (e.g. @c int and @c double as well as compound
 * data types).
 */
class Observation
{
public:
   /** Place where the sensor perceived this piece of information. */
   shawn::Vec location;

   /** Time when the sensor perceived this piece of information. */
   int time;

   /** Kind of the perceived information (i.e. the meaning of the value). */
   InformationDescription::Type type;

   /**
    * Virtual destructor for polymorphism.
    */
   virtual
   ~Observation()
   {
   }
};

class ObservationTemporalLess : public std::binary_function<Observation*,
         Observation*, bool>
{
public:
   bool
   operator()( const Observation* const x, const Observation* const y ) const
   {
      return x->time < y->time;
   }
};

/**
 * List of observations ordered by the observation time.
 */
typedef std::multiset<Observation*, ObservationTemporalLess> ObservationList;

// ----------------------------------------------------------------------

class IntObservation : public Observation
{
public:
   IntOpinion opinion;
   int value;

   friend std::ostream&
   operator<<( std::ostream& os, const IntObservation& o )
   {
      return os << o.location << ", type:" << o.type << ", time:" << o.time
               << ", value:" << o.value << ", " << o.opinion;
   }
};

/**
 * List of integer observations ordered by the observation time.
 */
typedef std::multiset<IntObservation*> IntObservationList;

}

#endif /* VANET_ENABLE */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_OBSERVATION_H */
