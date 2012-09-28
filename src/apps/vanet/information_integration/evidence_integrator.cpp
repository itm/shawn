/**
 * @file evidence_integrator.cpp
 * Implements the information integrator vanet::EvidenceIntegrator.
 *
 * @author Walter Bamberger
 *
 * $Revision: 636 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/information_integration/evidence_integrator.cpp $
 * $Id: evidence_integrator.cpp 636 2011-11-15 12:03:53Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "evidence_integrator.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"
#include "../trust/credit_trust_model.h"
#include "../trust/trust_bamberger_2010.h"
#include "../trust/regret_reputation_model.h"
#include "../statistics/statistics_task.h"
#include "sys/simulation/simulation_controller.h"
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace boost;
using namespace shawn;
using namespace std;

namespace vanet
{

EvidenceIntegrator::EvidenceIntegrator( KnowledgeBase& kb,
         const string& vehicleLabel, const SimulationController& sc,
         const __LOGGER_OBJECT & logger ) :
         kb_(kb), logStream_(0), logger_(logger), simulation_controller_(sc), trustComputer_(
                  0), reputationComputer_(0), vehicleLabel_(vehicleLabel)
{
   static const string trustComputerString =
            sc.environment().optional_string_param("trust_computer", "");

   static const string reputationComputerString =
            sc.environment().optional_string_param("reputation_computer", "");

   if ( reputationComputerString == "regret" )
   {
      reputationComputer_ = new RegretReputationModel(logger, kb);
   }
   else
   {
      reputationComputer_ = 0;
   }

   if ( trustComputerString == "credit" )
   {
      if ( reputationComputer_ != 0 )
         trustComputer_ = new CREDITModel(logger, kb, reputationComputer_);
      else
         trustComputer_ = new CREDITModel(logger, kb);
   }
   else if ( trustComputerString == "bamberger2010" )
   {
      trustComputer_ = new TrustBamberger2010(*this, kb, vehicleLabel, sc);
   }
   else if ( trustComputerString == "" )
   {
      // Simulation without trust
      trustComputer_ = 0;
   }
   else
   {
      /// @todo Error and exit
   }

   // Knowledge logging.
   static const tokenizer<> logLabels(
            sc.environment().optional_string_param("logLabels", ""));
   if ( find(logLabels.begin(), logLabels.end(), vehicleLabel)
            != logLabels.end() )
   {
      ofstream* logFStream = new ofstream();
      logFStream->open((vehicleLabel + ".knwldg.log").c_str());
      logFStream->setf(ios_base::fixed, ios_base::floatfield);
      logFStream->precision(4);
      logStream_ = logFStream;
   }
}

// ----------------------------------------------------------------------

EvidenceIntegrator::~EvidenceIntegrator()
{
   if ( logStream_ )
      delete logStream_;

   if ( trustComputer_ )
      delete trustComputer_;

   if ( reputationComputer_ )
      delete reputationComputer_;
}

// ----------------------------------------------------------------------

template<class It1, class It2>
   triplet<int, It1, It2>
   EvidenceIntegrator::forwardValue( It1 obsvBegin, It1 obsvEnd, It2 rprtBegin,
            It2 rprtEnd ) const
   {
      It1 obsvIt = obsvBegin;
      It2 rprtIt = rprtBegin;

      IntOpinion fusedOpinion;
      IntOpinion::ConstEvidenceIterator maxElement =
               fusedOpinion.beginEvidence();
      int lastTimeStamp = 0;

      while ( (obsvIt != obsvEnd || rprtIt != rprtEnd) && *maxElement < 1000.0f )
      {
         // Increase the evidence vector
         if ( obsvIt == obsvEnd )
         {
            if ( trustComputer_ )
            {
               pair<float, float> trustPair =
                        trustComputer_->computeTrustValue(*rprtIt);
               IntOpinion discountedOpinion =
                        (*rprtIt)->opinion.beliefDiscountingBy(trustPair.first);

               fusedOpinion.cumulativeFusionToSelf(discountedOpinion);
            }
            else
            {
               fusedOpinion.cumulativeFusionToSelf((*rprtIt)->opinion);
            }
            lastTimeStamp = (*rprtIt)->time;
            ++rprtIt;
         }
         else if ( rprtIt == rprtEnd )
         {
            fusedOpinion.cumulativeFusionToSelf((*obsvIt)->opinion);
            lastTimeStamp = (*obsvIt)->time;
            ++obsvIt;
         }
         else
         {
            // Both iterators are valid. (Because of the while condition,
            // both iterators cannot be invalid at the same time.
            if ( (*rprtIt)->time < (*obsvIt)->time )
            {
               if ( trustComputer_ )
               {
                  pair<float, float> trustPair =
                           trustComputer_->computeTrustValue(*rprtIt);
                  IntOpinion discountedOpinion =
                           (*rprtIt)->opinion.beliefDiscountingBy(
                                    trustPair.first);

                  fusedOpinion.cumulativeFusionToSelf(discountedOpinion);
               }
               else
               {
                  fusedOpinion.cumulativeFusionToSelf((*rprtIt)->opinion);
               }
               lastTimeStamp = (*rprtIt)->time;
               ++rprtIt;
            }
            else
            {
               fusedOpinion.cumulativeFusionToSelf((*obsvIt)->opinion);
               lastTimeStamp = (*obsvIt)->time;
               ++obsvIt;
            }
         }

         maxElement = max_element(fusedOpinion.beginEvidence(),
                  fusedOpinion.endEvidence());
      }

      // Correct the iterators
      if ( obsvIt != obsvBegin )
         --obsvIt;
      if ( rprtIt != rprtBegin )
         --rprtIt;

      int chosenValue = static_cast<int> (maxElement
               - fusedOpinion.beginEvidence());
      return make_triplet(chosenValue, obsvIt, rprtIt);
   }

// ----------------------------------------------------------------------

TrustComputer&
EvidenceIntegrator::getTrustComputer()
{
   if ( trustComputer_ )
      return *trustComputer_;
   else
      throw(runtime_error("No trust model configured in EvidenceIntegrator"));
}

// ----------------------------------------------------------------------

bool
EvidenceIntegrator::hasTrustComputer() const
{
   return trustComputer_ != 0;
}

// ----------------------------------------------------------------------

bool
EvidenceIntegrator::hasReputationComputer() const
{
   return reputationComputer_ != 0;
}

// ----------------------------------------------------------------------

IntInformationKnowledge*
EvidenceIntegrator::integrate( IntObservation& newObservation )
{
   /// @todo Could and probably should use findInformation(location, type, time)
   /// See integrateReport

   /*
    * Find the newest InformationKnowledge about the topic of the observation.
    * (The topic corresponds to the touple (location, type).)  If there is no
    * corresponding InformationKnowledge, this method creates a new one. If
    * there is such an InformationKnowledge, the new observation is added and
    * the InformationKnowledge is updated.
    */
   InformationKnowledge* newestInfoPtr = kb_.findNewestInformationAbout(
            newObservation.location, newObservation.type);
   if ( newestInfoPtr == 0 )
   {
      // The right touple (location, type) was not in the grid.
      // Create a new InformationKnowledge for that
      IntInformationKnowledge* intInfoPtr = new IntInformationKnowledge();
      intInfoPtr->begin = newObservation.time;
      intInfoPtr->end = newObservation.time;
      intInfoPtr->location = newObservation.location;
      intInfoPtr->type = newObservation.type;
      intInfoPtr->next(0);
      intInfoPtr->previous(0);
      intInfoPtr->observations.insert(&newObservation);
      kb_.addInformationKnowledge(intInfoPtr);
      if ( logStream_ )
         *logStream_ << "O: " << newObservation;
      updateInformationKnowledge(*intInfoPtr);
      return intInfoPtr;

   }
   else
   {
      /*
       * An InformationKnowledge about the same touple (location, type) is
       * already in the knowledge base. Get the most recent one. Then add the
       * new observation to that InformationKnowledge and update the structure
       * around and the values of the InformationKnowledge.
       */
      IntInformationKnowledge* newestIntInfoPtr =
               dynamic_cast<IntInformationKnowledge*> (newestInfoPtr);
      // Test whether the right touple (location, type) was in the grid
      if ( newestIntInfoPtr != 0 )
      {
         newestIntInfoPtr->observations.insert(&newObservation);
         if ( logStream_ )
            *logStream_ << "O: " << newObservation;
         splitAndUpdateInformationKnowledge(*newestIntInfoPtr);
         return newestIntInfoPtr;
      }
      else
      {
         ostringstream ss;
         ss << "EvidenceIntegrator: Could not integrate an observation of type "
                  << newestInfoPtr->type << ".";
         throw invalid_argument(ss.str());
      }
   }
}

// ----------------------------------------------------------------------

IntInformationKnowledge*
EvidenceIntegrator::integrate( IntReport* newReport ) throw ( runtime_error )
{
   /// @todo Refactor this method

   /* Find the newest InformationKnowledge about the topic of the report.
    * (The topic corresponds to the touple (location, type).)  If there is no
    * corresponding InformationKnowledge, this method creates a new one. If
    * there is such an InformationKnowledge, the new report is added and the
    * InformationKnowledge is updated.
    */

   // First, find the InformationKnowledge that is most similar
   // to the report.
   InformationKnowledge* infoPtr = kb_.findInformation(newReport->location,
            newReport->type, newReport->time);
   if ( infoPtr )
   {
      IntInformationKnowledge* intInfoPtr =
               dynamic_cast<IntInformationKnowledge*> (infoPtr);
      // Test whether the C++ type matches
      if ( intInfoPtr )
      {
         // Find out whether I received a report about the same statement
         // before. The statement is the same, if – in addition to
         // location and type – the observation time and the sender are the
         // same. Then, only values (opinion and value) of that statement
         // may be different – and the reception time, of course.

         // Find all reports with the same observation time
         pair<IntReportList::iterator, IntReportList::iterator> rprtRange =
                  intInfoPtr->reports.equal_range(newReport->time);

         // Find in addition the same sender
         for ( IntReportList::iterator it = rprtRange.first; it
                  != rprtRange.second; ++it )
         {
            if ( (*it)->sender == newReport->sender )
            {
               // The found report and the new report match
               // in (location, type, time, sender). So, the vehicle received a
               // message about the same observation again.
               if ( (*it)->opinion == newReport->opinion && (*it)->value
                        == newReport->value )
               {
                  // Nothing has changed. No split and update of the
                  // InformationKnowledge is necessary. Only update the
                  // reception attributes.
                  (*it)->receptionLocation = newReport->receptionLocation;
                  (*it)->receptionTime = newReport->receptionTime;
                  delete newReport;
                  return intInfoPtr;
               }
               else
               {
                  // The opinion or the value has changed. Thus, split and
                  // update is necessary, after the new values have
                  // been saved.
                  if ( logStream_ )
                  {
                     *logStream_ << "R: " << *newReport << "\n"
                              << "    Only the value has changed. Split and update."
                              << endl;
                  }
                  (*it)->opinion = newReport->opinion;
                  (*it)->receptionLocation = newReport->receptionLocation;
                  (*it)->receptionTime = newReport->receptionTime;
                  (*it)->value = newReport->value;
                  splitAndUpdateInformationKnowledge(*intInfoPtr);
                  delete newReport;
                  return (*it)->associatedInformation();
               }
            }
         }

         // Report was not yet known. Insert it.
         if ( logStream_ )
         {
            *logStream_ << "R: " << *newReport << "\n"
                     << "    New report received. Insert, split and update."
                     << endl;
         }
         intInfoPtr->reports.insert(newReport);
         newReport->associatedInformation(intInfoPtr);
         splitAndUpdateInformationKnowledge(*intInfoPtr);

         // Inform the knowledge base
         kb_.integratedNewReport(newReport);

         // Inform the statistics task
         float trustValue = 0.0;
         if ( trustComputer_ )
            trustValue = trustComputer_->computeTrustValue(newReport).first;
         StatisticsTask& st = StatisticsTask::instance();
         st.integratedReport(newReport, vehicleLabel_);

         return newReport->associatedInformation();
      }
      else
      {
         delete newReport;
         ostringstream ss;
         ss << "KnowledgeBase is type inconsistent: type " << infoPtr->type
                  << " should match with IntInformationKnowledge.";
         throw(runtime_error(ss.str()));
      }
   }
   else
   {
      // The right touple (location, type) was not in the grid.
      // Create a new InformationKnowledge for that
      if ( logStream_ )
      {
         *logStream_ << "R: " << *newReport << "\n"
                  << "    New information knowledge created." << endl;
      }
      IntInformationKnowledge* intInfoPtr = new IntInformationKnowledge();
      intInfoPtr->begin = newReport->time;
      intInfoPtr->end = newReport->time;
      intInfoPtr->location = newReport->location;
      intInfoPtr->type = newReport->type;
      intInfoPtr->previous(0);
      intInfoPtr->next(0);
      intInfoPtr->reports.insert(newReport);
      newReport->associatedInformation(intInfoPtr);
      kb_.addInformationKnowledge(intInfoPtr);
      updateInformationKnowledge(*intInfoPtr);
      kb_.integratedNewReport(newReport);
      return intInfoPtr;
   }
}

// ----------------------------------------------------------------------

void
EvidenceIntegrator::splitAndUpdateInformationKnowledge(
         IntInformationKnowledge& ik ) const
{
   // Check whether the InformationKnowledge must be split, since the
   // value may have changed over time.
   /// @todo: Forward value has no check for the right information type.
   /// It simply assumes some.
   triplet<int, IntObservationList::const_iterator,
            IntReportList::const_iterator> fwdSplit = forwardValue(
            ik.observations.begin(), ik.observations.end(), ik.reports.begin(),
            ik.reports.end());

   triplet<int, IntObservationList::const_reverse_iterator,
            IntReportList::const_reverse_iterator> bwdSplit = forwardValue(
            ik.observations.rbegin(), ik.observations.rend(),
            ik.reports.rbegin(), ik.reports.rend());

   if ( logStream_ )
      *logStream_ << "    Forward value: " << fwdSplit.first
               << ", backward value: " << bwdSplit.first << endl;

   if ( fwdSplit.first != bwdSplit.first )
   {
      IntInformationKnowledge& newIK = splitInformationKnowledge(ik,
               bwdSplit.second.base(), bwdSplit.third.base());
      updateInformationKnowledge(ik);
      updateInformationKnowledge(newIK);
   }
   else
   {
      updateInformationKnowledge(ik);
   }
}

// ----------------------------------------------------------------------

IntInformationKnowledge&
EvidenceIntegrator::splitInformationKnowledge( IntInformationKnowledge& ik,
         IntObservationList::const_iterator obsvSplitBegin,
         IntReportList::const_iterator rprtSplitBegin ) const
{
   /*
    * Create a new piece of knowledge that will contain the separated
    * observations and reports
    */
   IntInformationKnowledge& newIK = *(new IntInformationKnowledge());
   newIK.next(ik.next());
   newIK.previous(&ik);
   ik.next(&newIK);
   newIK.location = ik.location;
   newIK.type = ik.type;
   kb_.addInformationKnowledge(&newIK);

   /*
    * Move the observations and reports starting from the split points to the
    * new InformationKnowledge object. Then associate the moved elements with
    * the new InformationKnowledge object.
    */
   newIK.observations.insert(obsvSplitBegin, ik.observations.end());
   ik.observations.erase(obsvSplitBegin, ik.observations.end());
   newIK.reports.insert(rprtSplitBegin, ik.reports.end());
   ik.reports.erase(rprtSplitBegin, ik.reports.end());
   for ( IntReportList::iterator rprtIt = newIK.reports.begin(); rprtIt
            != newIK.reports.end(); ++rprtIt )
      (*rprtIt)->associatedInformation(&newIK);

   return newIK;
}

// ----------------------------------------------------------------------

void
EvidenceIntegrator::updateInformationKnowledge( IntInformationKnowledge& ik,
         bool withLogging ) const
{
   if ( ik.type == InformationDescription::IntSituational || ik.type
            == InformationDescription::IntShort || ik.type
            == InformationDescription::IntMedium || ik.type
            == InformationDescription::IntLong )
   {
      /*
       * Set the begin and the end time.
       */
      if ( ik.observations.size() != 0 && ik.reports.size() != 0 )
      {
         ik.begin = min((*ik.observations.begin())->time,
                  (*ik.reports.begin())->time);
         ik.end = max((*ik.observations.rbegin())->time,
                  (*ik.reports.rbegin())->time);
      }
      else if ( ik.observations.size() != 0 )
      {
         ik.begin = (*ik.observations.begin())->time;
         ik.end = (*ik.observations.rbegin())->time;
      }
      else if ( ik.reports.size() != 0 )
      {
         ik.begin = (*ik.reports.begin())->time;
         ik.end = (*ik.reports.rbegin())->time;
      }
      else
      {
         // May not happen. Either a report or an observation must be known
         // in order to make up an InformationKnowledge.
         throw logic_error(
                  "There is an InformationKnowledge in the knowledge base "
                     "with no observation and no report!");
      }

      /*
       * Record the change in the last_changed flag.
       */
      ik.last_changed = simulation_controller_.world().simulation_round();

      /*
       *  Collect the evidence to determine the overall value and opinion.
       */
      IntOpinion fusedObservationOpinion; // Initialised with zero evidence
      for ( multiset<IntObservation*>::const_iterator obsvIt =
               ik.observations.begin(); obsvIt != ik.observations.end(); ++obsvIt )
      {
         fusedObservationOpinion.cumulativeFusionToSelf((*obsvIt)->opinion);
         if ( logStream_ && withLogging )
            *logStream_ << "    o: " << *(*obsvIt) << "\n";
      }
      IntOpinion fusedReportOpinion; // Initialised with zero evidence
      for ( IntReportList::const_iterator rprtIt = ik.reports.begin(); rprtIt
               != ik.reports.end(); ++rprtIt )
      {
         if ( trustComputer_ )
         {
            // May throw an out_of_range exception (is a logic_error exception).
            pair<float, float> trustPair = trustComputer_->computeTrustValue(
                     *rprtIt);
            IntOpinion discountedOpinion =
                     (*rprtIt)->opinion.beliefDiscountingBy(trustPair.first);

            fusedReportOpinion.cumulativeFusionToSelf(discountedOpinion);
            if ( logStream_ && withLogging )
            {
               *logStream_ << "    r: " << *(*rprtIt) << "\n";
               *logStream_ << "    [" << trustPair.first << ","
                        << trustPair.second << "] " << " ⊗ ... = "
                        << discountedOpinion << " --> " << fusedReportOpinion
                        << "\n";
            }
         }
         else
         {
            fusedReportOpinion.cumulativeFusionToSelf((*rprtIt)->opinion);
            if ( logStream_ && withLogging )
               *logStream_ << "    r: " << *(*rprtIt) << "\n";
         }
      }
      ik.opinion = fusedObservationOpinion.priorityFusion(fusedReportOpinion);

      ik.value = max_element(ik.opinion.beginEvidence(),
               ik.opinion.endEvidence()) - ik.opinion.beginEvidence();
      if ( logStream_ && withLogging )
         *logStream_ << "    --> " << ik << endl;
   }
   else
   {
      ostringstream oss;
      oss << "InformationType " << ik.type
               << " cannot be handled by this knowledge base.";
      throw invalid_argument(oss.str());
   }
}

}

#endif /* ENABLE_VANET */
