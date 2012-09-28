/**
 * @file knowledge_base.h
 * Defines the simple data base class vanet::KnowledgeBase.
 *
 * @author Walter Bamberger
 *
 * $Revision: 640 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/knowledge_base/knowledge_base.h $
 * $Id: knowledge_base.h 640 2011-12-02 07:56:32Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_KNOWLEDGE_BASE_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_KNOWLEDGE_BASE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "information_knowledge.h"
#include "node_knowledge.h"
#include "trust_attitude.h"
#include "../information_integration/evidence_integrator.h"
#include "../trust/trust_computer.h"
#include "../utility.h"
#include "sys/logging/logging.h"
#include <tr1/unordered_map>
#include <map>

namespace shawn
{
class SimulationController;
}

namespace vanet
{

class StatisticsTask;
class Disseminator;

class KnowledgeBase
{
public:
   friend class StatisticsTask;

   KnowledgeBase( const shawn::SimulationController& sc,
            const std::string& vehicleLabel, const __LOGGER_OBJECT & logger );

   ~KnowledgeBase();

   void
   prune() throw ( std::runtime_error );

   // Should be const const
   EvidenceIntegrator&
   getEvidenceIntegrator();

   ///@name Add knowledge
   ///@{

   template<class T>
      void
      addObservation( T* newObservation ) throw ()
      {
         InformationKnowledge * infoPtr = informationIntegrator.integrate(
                  *newObservation);

         newObservations.insert(newObservation);
         if ( newObservations.size() > 10 )
            newObservations.erase(newObservations.begin());
      }

   /**
    * Inserts a trust attitude in the knowledge base.
    *
    * In case of an exception, the knowledge base is still in a consistent
    * state, but the content may be modified. So this method is not transaction
    * safe.
    *
    * @throw std::bad_alloc the trust attitude could not be added.
    */
   void
   addTrustAttitude( TrustAttitude* newTrustAttitude )
   {
      /* A trust attitude is a function of the node and the information type.
       * Only one trust attitude can be valid for every such tuple. So first,
       * all trust attitudes that match the sender and information type of the
       * new trust attitude must be deleted.
       */
      for ( TrustAttitudeList::iterator t = recentTrustAttitudes.begin(); t
               != recentTrustAttitudes.end(); ++t )
      {
         if ( (*t)->trustedNode == newTrustAttitude->trustedNode && (*t)->type
                  == newTrustAttitude->type )
         {
            recentTrustAttitudes.erase(t);
         }
      }

      // If the list is too long, the oldest trust attitude must be dropped.
      if ( recentTrustAttitudes.size() > 9 )
         recentTrustAttitudes.erase(recentTrustAttitudes.begin());

      // Now the new trust attitude can be inserted in the list.
      recentTrustAttitudes.insert(newTrustAttitude);
   }

   /**
    * Inserts a report in the knowledge base.
    *
    * First, for the same report (sender, location, type, time) is looked
    * for. If it is new, the report is associated with the right
    * InformationKnowledge, the own knowledge is recomputed and the indices
    * are updated.
    */
   template<class T>
      void
      addReport( T* newReport ) throw ( std::runtime_error )
      {
         InformationKnowledge* infoPtr = informationIntegrator.integrate(
                  newReport);
      }

   /**
    * Integrates the InformationKnowledge object in the cache structure of
    * the knowledge base.
    *
    * This method does not change the give InformationKnowledge in any way.
    * So it expects that all values and pointers are already set correctly,
    * especially the connections to other InformationKnowledge objects.
    *
    * In case of a bad_alloc exception, the knowledge base is still in a
    * consistent state. Its cache values may be modified though. So this method
    * is not transaction safe.
    *
    * @arg newInformationKnowledge
    *              The InformationKnowledge to integrate
    *
    * @throw std::bad_alloc
    *              The information knowledge could not be added.
    */
   template<class T>
      void
      addInformationKnowledge( T* newInformationKnowledge )
      {
         locationGrid.insert(
                  make_pair(vecToGridKey(newInformationKnowledge->location),
                           newInformationKnowledge));

         // Maintain a sorted list that contains at most
         // the 10 newest pieces of information
         newestInformation.insert(newInformationKnowledge);
         if ( newestInformation.size() > 10 )
         {
            newestInformation.erase(newestInformation.begin());
         }

      }

   void
   registerMeeting( const std::string& nodeLabel, int meetingTime ) throw ();

   ///@}

   ///@name Notification of changes
   ///@{

   /**
    * @throw std::logic_error
    *              An inconsistency in the knowledge base has been found.
    * @throw std::bad_alloc
    *              Some data structures for the new report could not be created.
    *              The knowledge base may be in an inconsistent state when
    *              this exception occurs.
    */
   template<class T>
      void
      integratedNewReport( T* newReport )
      {
         /* The information integrated added the report in the structures of 
          * information knowledge. Here the report must also be added to the
          * node related structures. In addition, the trust attitude cache
          * must be updated as trust can change with every new report.
          */

         knowledgeOfNodes[newReport->sender].reports.insert(newReport);

         if ( informationIntegrator.hasTrustComputer()
                  && informationIntegrator.hasReputationComputer() )
         {
            std::pair<float, float>
                     trust =
                              informationIntegrator.getTrustComputer().computeTrustValue(
                                       newReport);
            /* The information of trust is added in the list of recentTrustAttitude,
             * only when its reliability is large then the threshold value.(here is 0.6)
             */
            if ( trust.second >= 0.6 )
            {
               TrustAttitude* t = new TrustAttitude();
               t->trustedNode = newReport->sender;
               t->lastInteractionTime = newReport->receptionTime;
               t->trust = trust;
               t->type = newReport->type;
               addTrustAttitude(t);
            }
         }
      }
   ///@}

   ///@name Query for knowledge
   ///@{

   /**
    * Provides a list with the ten newest pieces of information.
    *
    * This method takes the begin attribute of a piece of information as
    * the criterion of age. The fact that something new happened is of interest.
    * If something already is known for quite a while, then another
    * observation, for example, is not very interesting. Because of this,
    * the begin time is the appropriate criterion for filtering this list.
    *
    * In addition, the list is sorted by the begin attribute.
    *
    * @todo Rename to findAllNewInformation
    *
    * @return the list of InformationKnowledge objects
    *         sorted by the time attribute
    */
   const InformationKnowledgeList&
   findNewestPiecesOfInformation() const
   {
      return newestInformation;
   }

   /**
    * @todo Rename to findLatestInformation
    */
   InformationKnowledge*
   findNewestInformationAbout( const shawn::Vec& location,
            const InformationDescription::Type& type ) throw ();

   /**
    * Finds the InformationKnowledge that fits location and type exactly and
    * is the closest one regarding the time.
    *
    * Closest one regarding the time means that it is the newest
    * InformationKnowledge with a begin smaller than time or, if such
    * an InformationKnowledge is not available, this method returns
    * the oldest InformationKnowledge.
    *
    * If and only if the topic (location, type) is not in the knowledge base,
    * then 0 is returned.
    */
   InformationKnowledge
            *
            findInformation( const shawn::Vec& location,
                     const InformationDescription::Type& type, const int& time ) throw ();

   /// @todo Rename to findLatestObservation
   Observation*
   findNewestObservationAbout( const shawn::Vec& location,
            const InformationDescription::Type& type ) throw ();

   /// @todo Rename to findRecentObservations
   const ObservationList&
   findAllNewObservations() const throw ()
   {
      return newObservations;
   }

   const TrustAttitudeList&
   findRecentTrustAttitudes() const throw ()
   {
      return recentTrustAttitudes;
   }

   ReportList
   findAllReports( const std::string& nodeLabel,
            const InformationDescription::Type& type ) const throw ();

   /**
    * Provides a list of all reports received from the given sender.
    *
    * @note
    * This method is more like @c equal_range of a @c multimap. Then it should
    * return iterators. But iterators of what interface of
    * @c multi_index_container should be used? Unfortunately the
    * @c multi_index_container provides no means to change the iterator
    * interface, except of the container itself. This is why a container is
    * returned. (The drawback is that the implementation of an empty result is
    * not strait forward.)
    * @par
    * A way out of this could be, to integrate the @c get method of
    * @c multi_index_container in the @c findAllReports method. Then it needs a
    * template parameter that contains the tag of the index. This way, the
    * user of the API can still get iterators for all possible indices.
    * @par
    * On the other hand, returning a result is more similar to the method @c at.
    * So maybe, the @c findAllReports methods should better be named
    * @c reportsAt(...). (The find methods of the standard container return
    * iterators, not values.) @c at methods throw an out_of_range exception.
    * So in this case the old style was valid? Well unlike other values,
    * returned containers can be empty.
    *
    * @throw None
    */
   const ReportList&
   findAllReports( const std::string& nodeLabel ) const;

   ReportList
   findAllTrustReports( const std::string& nodeLabel,
            const InformationDescription::Type& type ) const throw ();

   ///@}

private:
   __LOGGER_OBJECT logger_;
   const shawn::SimulationController& simulationController_;
   EvidenceIntegrator informationIntegrator;
   int last_pruned;

   // The knowledge
   typedef std::tr1::unordered_map<std::string, NodeKnowledge>
     NodeKnowledgeMap;
   NodeKnowledgeMap knowledgeOfNodes;
   typedef std::pair<short int, short int> LocationGridKey;
   typedef std::multimap<LocationGridKey, InformationKnowledge*> LocationGrid;
   LocationGrid locationGrid;
   InformationKnowledgeList newestInformation;
   ObservationList newObservations;
   TrustAttitudeList recentTrustAttitudes;
   static const ReportList emptyReportList;

   void
   pruneOldNodes();

   void
   pruneOverfull();

   void
   pruneOverfullIntInformation(IntInformationKnowledge& ik);

   void
   removeNode( NodeKnowledgeMap::iterator nodeIt ) throw ( std::runtime_error );

   void
   removeReport( IntReport* report );

   /**
    * @throw None
    */
   static LocationGridKey
   vecToGridKey( const shawn::Vec& vec )
   {
      return std::make_pair(static_cast<short> (vec.x() / 100.0),
               static_cast<short> (vec.y() / 100.0));
   }
};

/**
 * Inserts the given trust report in the knowledge base.
 *
 * This method is a full specialisation of KnowledgeBase::addReport for
 * TrustReport. It is necessary, because a trust report is handled quite
 * differently than other reports.
 */
template<>
   void
   KnowledgeBase::addReport<TrustReport>( TrustReport* newReport );

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_KNOWLEDGE_BASE_H */
