/**
 * @file information_knowledge.h
 * Defines the data containers vanet::InformationKnowledge and
 * vanet::IntInformationKnowledge together with their lists
 * vanet::InformationKnowledgeList and vanet::IntInformationKnowledgeList.
 *
 * @author Walter Bamberger
 *
 * $Revision: 636 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/information_knowledge.h $
 * $Id: information_knowledge.h 636 2011-11-15 12:03:53Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_INFORMATION_KNOWLEDGE_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_INFORMATION_KNOWLEDGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "observation.h"
#include "report.h"

namespace vanet
{

/**
 * Record of a piece of information about the environment. It can originate
 * from an own perception (Observation), from a message (Report)
 * received from another node or from a combination of several such sources.
 *
 * A piece of information is valid for a certain location #location and
 * a certain time span (#begin and #end). It is described by
 * the kind of the value (#type), the value itself (defined in the sub
 * classes) and the vehicles opinion about this piece of information
 * (#opinion).
 *
 * This class is just an interface. Sub classes must be defined for various
 * data types of the value (e.g. @c int and @c double as well as compound
 * data types).
 */
class InformationKnowledge
{
public:
   /** Location in the environment. */
   shawn::Vec location;

   /** Kind of information (what the value quantifies). */
   InformationDescription::Type type;

   /** Earliest time when this piece of information is known to be right. */
   int begin;

   /** Latest time when this piece of information is known to be right. */
   int end;

   /**
    * Time when this piece of information has been modified by this vehicle at last.
    * This flag is helpful to speed up pruning, for example.
    */
   int last_changed;

   /**
    * The vehicle's subjective opinion about this piece of information.
    * @todo Should better be in the sub class?
    */
   IntOpinion opinion;

   /**
    * Virtual destructor for polymorphism.
    */
   virtual
   ~InformationKnowledge()
   {
   }

   /**
    * Provides the information knowledge about the same thing but earlier
    * in time.
    */
   virtual InformationKnowledge*
   previous() = 0;

   /**
    * Provides the information knowledge about the same thing but later in time.
    */
   virtual InformationKnowledge*
   next() = 0;
};

class InformationKnowledgeTemporalLess : public std::binary_function<
         InformationKnowledge*, InformationKnowledge*, bool>
{
public:
   bool
   operator()( const InformationKnowledge* const x,
            const InformationKnowledge* const y ) const
   {
      return x->begin < y->begin;
   }
};

typedef std::multiset<InformationKnowledge*, InformationKnowledgeTemporalLess>
         InformationKnowledgeList;

// ----------------------------------------------------------------------

class IntInformationKnowledge : public InformationKnowledge
{
private:
   IntInformationKnowledge* previous_;
   IntInformationKnowledge* next_;

public:
   int value;

   // (The following two list should ideally available from the base class too.)
   // List of observations sorted by the observation time
   IntObservationList observations;
   // List of reports sorted by the observation time
   IntReportList reports;

   virtual
   ~IntInformationKnowledge()
   {
   }

   virtual IntInformationKnowledge*
   next()
   {
      return next_;
   }

   virtual void
   next( IntInformationKnowledge* n )
   {
      next_ = n;
   }

   virtual IntInformationKnowledge*
   previous()
   {
      return previous_;
   }

   virtual void
   previous( IntInformationKnowledge* p )
   {
      previous_ = p;
   }

   friend std::ostream&
   operator<<( std::ostream& os, const IntInformationKnowledge& ik )
   {
      return os << ik.location << ", type:" << ik.type << ", begin:"
               << ik.begin << ", end:" << ik.end << ", value:" << ik.value
               << ", " << ik.opinion;
   }
};

typedef std::multiset<IntInformationKnowledge*,
         InformationKnowledgeTemporalLess> IntInformationKnowledgeList;

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_INFORMATION_KNOWLEDGE_H */
