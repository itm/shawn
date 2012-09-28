/**
 * @file evidence_integrator.h
 * Defines the information integrator vanet::EvidenceIntegrator.
 *
 * @author Walter Bamberger
 *
 * $Revision: 636 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/information_integration/evidence_integrator.h $
 * $Id: evidence_integrator.h 636 2011-11-15 12:03:53Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_INFORMATION_INTEGRATION_EVIDENCE_INTEGRATOR_H
#define __SHAWN_APPS_VANET_INFORMATION_INTEGRATION_EVIDENCE_INTEGRATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/information_knowledge.h"
#include "../utility.h"
#include <stdexcept>
#include <iosfwd>

namespace vanet
{

class KnowledgeBase;
class TrustComputer;

/**
 * @class EvidenceIntegrator
 *
 * Integrates information in the knowledge base based on evidence fusion.
 *
 * To obtain an opinion about a piece of information, the positive evidence
 * of each proposed value is summed up; the negative evidence is omited.
 * With this positive evidence, an evidence vector for all possible values
 * is computed. The value with the strongest evidence is taken then. The
 * evidence for this value makes up the positive evidence for the resulting
 * opinion, all the other evidence is taken as the negative evidence.
 *
 * To decide whether and where to split an InformationKnowledge object,
 * the observations and reports are processed in their temporal order.
 * As soon as one value gains a cumulative amount of evidence higher than
 * a threshold, this value is taken as the forward value. The same process
 * is performed starting from the temporal end of all observations and
 * reports. The result is the backward value. If both values are the same,
 * the report needs not be split. Otherwise, the report is split at the
 * point in time, when the threshold is first exceeded.
 */
class EvidenceIntegrator
{
public:
   /**
    * Initialises the evidence based integrator.
    *
    * A reference to the vehicle's knowledge base is necessary to get
    * fast access to some knowledge through the indexing infrastructure of
    * the knowledge base and to modify this indexing infrastructure in case
    * of new or changed knowledge. So, this integrator needs write access
    * to the public methods of the knowledge base.
    *
    * @param kb
    *              A reference to the vehicle's knowledge base
    * @param vehicleLabel
    *              The label of the vehicle this evidence integrator belongs to
    * @param sc
    *              The simulation controller from which the configuration
    *              parameters can be retrieved
    * @param logger
    *              A reference to a Shawn Logger object for debugging output
    *
    * @throw std::bad_alloc
    *              The requested configuration could not be set up. The evidence
    *              integrator is still in a valid state, but not in the one
    *              configured by the user.
    * @throw std::runtime_error
    *              The configuration could not be retrieved. The evidence
    *              integrator is still in a valid state, but not in the one
    *              configured by the user.
    */
   EvidenceIntegrator( KnowledgeBase& kb, const std::string& vehicleLabel,
            const shawn::SimulationController& sc,
            const __LOGGER_OBJECT & logger );

   /**
    * Cleans up the evidence based integrator.
    *
    * @throw None
    */
   virtual
   ~EvidenceIntegrator();

   /**
    * Provides the trust computer this evidence integrator uses. Using a
    * trust computer is optional. So you must first check with
    * #hasTrustComputer, whether a trust computer is configured.
    *
    * @throw std::runtime_error
    *              If no trust computer is configured
    *              (hasTrustComputer returns false)
    * @todo Should be const with const reference as the result
    */
   TrustComputer&
   getTrustComputer();

   /**
    * Checks whether this evidence integrator uses a trust computer.
    *
    * @return True if a trust computer is configured
    * @throw  None
    */
   bool
   hasTrustComputer() const;

   /**
    * Checks whether this evidence integrator uses a reputation computer.
    *
    * @return True if a reputation computer is configured
    * @throw  None
    */
   bool
   hasReputationComputer() const;

   /**
    * Integrates a new observation in the structure of the knowledge base and
    * updates all affected knowledge objects. This method should only be used
    * by the knowledge base. Other classes should use
    * KnowledgeBase#addObservation.
    *
    * @param newObservation
    *              The observation to integrate in the knowledge base
    * @return The IntInformationKnowledge to which the observation has been
    *         assigned.
    *
    * @throw std::bad_alloc
    *              The observation could not be added to an InformationKnowedge
    *              object or a new @c InformationKnowledge object – if needed –
    *              could not be created. The knowledge base could be in an
    *              inconsistent state.
    * @throw std::invalid_argument
    *              Could not integrate the observation, because algorithms for
    *              the information type of the observation are not implemented.
    *              The knowledge base may be in an inconsistent state.
    * @throw std::logic_error
    *              An inconsistency in the knowledge base has been found before
    *              modifying it. So the state of the knowledge base is
    *              undefined and already was before calling this method.
    */
   IntInformationKnowledge*
   integrate( IntObservation& newObservation );

   /**
    * Integrates a new report in the structure of the knowledge base and updates
    * all affected knowledge objects. This method should only be used by the
    * knowledge base. Other classes should use KnowledgeBase#addReport.
    *
    * @param newReport
    *              The report to integrate in the knowledge base
    * @return The IntInformationKnowledge to which the report has been assigned
    *
    * @throw std::bad_alloc
    *              The report could not be added to an InformationKnowedge
    *              object or a new @c InformationKnowledge object – if needed –
    *              could not be created. The knowledge base may be in an
    *              inconsistent state.
    * @throw std::invalid_argument
    *              Could not integrate the report, because algorithms for the
    *              information type of the report are not implemented. The
    *              knowledge base may be in an inconsistent state.
    * @throw std::logic_error
    *              An inconsistency in the knowledge base has been found before
    *              modifying it. So the state of the knowledge base is
    *              undefined and already was before calling this method.
    */
   IntInformationKnowledge*
   integrate( IntReport* newReport ) throw ( std::runtime_error );

   /**
    * Updates the values of the given piece of knowledge. It uses the evidence
    * (observations and reports) of the given information knowledge object to
    * derive an estimated truth about the world. It does not modify the
    * structure of the knowledge base or change values of other objects.
    *
    * @param ik
    *              The piece of knowledge to update
    *
    * @throw std::invalid_argument
    *              Thrown if the information type is of a kind for which
    *              updating is not implemented in this method. The given
    *              piece of knowledge is still unmodified.
    * @throw std::logic_error
    *              In case of a logic_error exception that is no
    *              invalid_argument exception, an inconsistency in the knowledge
    *              base has been found.
    */
   void
   update( IntInformationKnowledge& ik ) const throw ( std::runtime_error )
   {
      updateInformationKnowledge(ik, false);
   }

private:
   KnowledgeBase& kb_;
   std::ostream* logStream_;//
   __LOGGER_OBJECT logger_;
   const shawn::SimulationController& simulation_controller_;
   TrustComputer* trustComputer_;
   TrustComputer* reputationComputer_;
   const std::string vehicleLabel_;

   /**
    * Estimates the value of a world property from the given observations and
    * reports moving forward in time through all pieces of evidence.
    *
    * Both lists given by the begin and end iterators must only contain
    * observations and reports about the same world property. (The location and
    * the information type are the same.) The observations and reports must be
    * in temporal order when using the iterators. @c obsvEnd and @c rprtEnd are
    * only used for comparison but their elements are never accessed.
    *
    * This method walks through the observations and the reported observations
    * following the observation times. It continuously assigns degrees of
    * certainty to every possible value of the world property. If one value
    * exceeds a minimum certainty threshold, this value is assumed to be true
    * and returned.
    *
    * Besides this estimated truth about the world, this method also returns
    * iterators pointing to the final observation and report that was used to
    * reach the threshold. So all elements the closed range between the begin
    * iterators and the returned iterators make up enough evidence to represent
    * valid knowledge about the world. All elements behind the returned
    * iterators may also belong to the same piece of knowledge but need not.
    *
    * If the processing of an observation or report throws an exception, this
    * exception is catched. Then this method continues with the next observation
    * or report is taken. Thus this method throws no exception.
    *
    * @param obsvBegin
    *              iterator to the first element of the observation list
    * @param obsvEnd
    *              iterator behind the last element of the observation list
    * @param rprtBegin
    *              iterator to the first element of the report list
    * @param rprtEnd
    *              iterator behind the last element of the report list
    * @return the value estimated and the iterators to the last observation and
    *         report used to estimate the found value
    *
    * @throw std::out_of_range
    *              inconsistent knowledge base
    */
   template<class It1, class It2>
      triplet<int, It1, It2>
               forwardValue( It1 obsvBegin, It1 obsvEnd, It2 rprtBegin,
                        It2 rprtEnd ) const;

   /**
    * Updates the structure around and the values of the given information
    * knowledge element. When an observation or report is added to an information
    * knowledge element, the knowledge structure and knowledge values must be
    * updated.
    *
    * This method tests if this piece of knowledge actually refers to two
    * different pieces of knowledge because the value has changed over time.
    * For example, a traffic jam comes up at some time and ends at another time.
    * To realise this test, first the value of IntInformationKnowledge is
    * computed with #forwardValue starting with the oldest observation and
    * report. Then the same is done starting with the newest observation and
    * report (using reverse iterators). Both are then compared. If they are
    * different, the value of the piece of knowledge has changed over time and
    * the information knowledge is thus split in two information knowledge
    * elements by calling #splitInformationKnowledge. This is the split process.
    * It manages the structure of the web of information knowledge elements.
    *
    * To update the values of the given knowledge element and the new knowledge
    * element (if a split occurred), this method calls
    * #updateInformationKnowledge.
    *
    * @param ik
    *              the piece of knowledge the structure and values of which
    *              should be updated
    *
    * @throw std::invalid_argument
    *              Thrown if the information type is of a kind for which
    *              updating is not implemented in this method. The knowledge
    *              base may be in an inconsistent state.
    * @throw std::logic_error
    *              An inconsistency in the knowledge base has been found
    * @throw std::bad_alloc
    *              A new IntInformationKnowledge object could not be created in
    *              case of a split
    */
   void
   splitAndUpdateInformationKnowledge( IntInformationKnowledge& ik ) const;

   /**
    * Splits the given piece of knowledge at the given observation and report.
    * This results in a new piece of knowledge containing all observations and
    * reports starting from the split positions. The observations and reports
    * before the split points remain in the given piece of knowledge. The new
    * piece of knowledge is returned in the end.
    *
    * This method only moves observations and reports and copies the values
    * of InformationKnowledge::location and InformationKnowledge::type to the
    * new piece of information. The value and opinion fields are not touched.
    * You must call #updateInformationKnowledge to update these fields.
    *
    * In case of a bad_alloc exception, the knowledge base is still in a
    * consistent state. Its cache values may be modified though. So this method
    * is not transaction safe.
    *
    * @param ik
    *              The piece of knowledge to split
    * @param obsvSplitBegin
    *              The first observation to move to the new piece of knowledge
    * @param rprtSplitBegin
    *              The first report to move to the new piece of knowledge
    * @return The created piece of knowledge
    *
    * @throw std::bad_alloc
    *              The new IntInformationKnowledge object could not be created
    *              or not be inserted in the knowledge base.
    */
   IntInformationKnowledge&
   splitInformationKnowledge( IntInformationKnowledge& ik,
            IntObservationList::const_iterator obsvSplitBegin,
            IntReportList::const_iterator rprtSplitBegin ) const;

   /**
    * Updates the fields of the given IntInformationKnowledge based on the
    * observations and reports it contains. The updated fields are @c begin,
    * @c end, @c opinion and @c value. The other fields like @c location and
    * @c type are assumed to be fixed.
    *
    * Optionally this method can log intermediate values to #logStream_. It does
    * so per default, but you can turn logging off with the argument
    * @c withLogging. Disabling logging for some update operations may help to
    * keep a clear log file.
    *
    * @param ik
    *              The piece of knowledge the values of which are updated
    * @param withLogging
    *              If @c true, intermediate values are logged in @c logStream_
    *
    * @throw std::invalid_argument
    *              Thrown if the information type is of a kind for which
    *              updating is not implemented in this method. The given
    *              piece of knowledge is still unmodified.
    * @throw std::logic_error
    *              In case of a logic_error exception that is no
    *              invalid_argument exception, an inconsistency in the knowledge
    *              base has been found.
    */
   void
   updateInformationKnowledge( IntInformationKnowledge& ik,
            bool withLogging = true ) const;
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_INFORMATION_INTEGRATION_EVIDENCE_INTEGRATOR_H */
