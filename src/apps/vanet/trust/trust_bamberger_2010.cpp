/**
 * @file trust_bamberger_2010.cpp
 * Implements the trust model class vanet::TrustBamberger2010.
 *
 * @author Walter Bamberger
 *
 * $Revision: 636 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/trust_bamberger_2010.cpp $
 * $Id: trust_bamberger_2010.cpp 636 2011-11-15 12:03:53Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "trust_bamberger_2010.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"
#include "../utility.h"
#include "sys/simulation/simulation_controller.h"
#include <boost/tokenizer.hpp>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <numeric>

using namespace boost;
using namespace shawn;
using namespace std;

namespace vanet
{

struct TrustBamberger2010::ComputationParameters
{
   BinaryOpinion trust;
   BinaryOpinion competence;
   BinaryOpinion predictability;
   double c;
   double errorSum;

   ComputationParameters() :
      c(0.75 / (1.0 - 0.75)), errorSum(0.0)
   {
      /*
       * 0.75 is the default trust, for the case that no evidence is available.
       * This seems more realistic than 0.5, because most vehicle are usually
       * quite reliable.
       */
      float baseTrust[1] = { 0.75 };
      trust.setBaseRate(baseTrust);
      competence.setBaseRate(baseTrust);
      predictability.setBaseRate(baseTrust);

   }
};

// ----------------------------------------------------------------------

TrustBamberger2010::TrustBamberger2010( const EvidenceIntegrator& integrator,
         const KnowledgeBase& kb, const string& vehicleLabel,
         const SimulationController& sc ) :
            integrator_(integrator),
            knowledgeBase_(kb),
            logStream_(0),
            reputationComputer_(0),
            simulationController_(sc),
            withPredictability_(
                     sc.environment().optional_bool_param(
                              "trust_with_predictability", false))
{
   static const tokenizer<> logLabels(
            sc.environment().optional_string_param("logLabels", ""));
   if ( find(logLabels.begin(), logLabels.end(), vehicleLabel)
            != logLabels.end() )
   {
      ofstream* logFStream = new ofstream();
      logFStream->open((vehicleLabel + ".trust.log").c_str());
      logFStream->setf(ios_base::fixed, ios_base::floatfield);
      logFStream->precision(4);
      logStream_ = logFStream;
   }
}

// ----------------------------------------------------------------------

TrustBamberger2010::~TrustBamberger2010()
{
   if ( logStream_ )
      delete logStream_;
}

// ----------------------------------------------------------------------

ostream&
TrustBamberger2010::computeCsvRecord( ostream& os, const Report* report )
{
   computeTrustValueAndCsv(report, &os);
   return os;
}

// ----------------------------------------------------------------------

pair<float, float>
TrustBamberger2010::computeTrustValue( const Report* report )
{
   return computeTrustValueAndCsv(report, 0);
}

// ----------------------------------------------------------------------

BinaryOpinion
TrustBamberger2010::combineTrustInTypes( const Report* reportToTrust,
         const vector<ComputationParameters>& parameterSet ) const
{
   // Index of the parameters that belong to the information type of the
   // given report.
   size_t indexReportParameters = static_cast<size_t> (reportToTrust->type % 4);
   BinaryOpinion trustInThisType = parameterSet[indexReportParameters].trust;

   // Fusion of the trust parameter computed for all the information types
   // different from the one of the given report.
   BinaryOpinion trustInOtherTypes;
   for ( int i = 0; i < 4; ++i )
   {
      if ( i != indexReportParameters )
      {
         trustInOtherTypes.cumulativeFusionToSelf(parameterSet[i].trust);
      }
   }

   // Log debugging information.
   if ( logStream_ )
   {
      const ComputationParameters& psToTrust =
               parameterSet[indexReportParameters];
      *logStream_ << "    --> type:" << reportToTrust->type << ", competence:"
               << psToTrust.competence;
      if ( withPredictability_ )
         *logStream_ << ", predictability:" << psToTrust.predictability;
      *logStream_ << ", trust:" << psToTrust.trust << endl;
   }

   // Final trust opinion: Fusion of the trust parameter of the wanted type
   // and the unwanted types.
   return trustInThisType.priorityFusion(trustInOtherTypes);
}

// ----------------------------------------------------------------------

pair<float, float>
TrustBamberger2010::computeTrustValueAndCsv( const Report* reportToTrust,
         ostream* csvStream )
{
   /*
    * Get all reports and process them one after the other in temporal order.
    * Each type of information is treated separately in this step. So
    * separate computation parameters are necessary for every information
    * type.
    */

   vector<ComputationParameters> parameterSet(4);

   // All reports of the sender. They represent the whole experience
   // with the sender.
   const ReportList::index<Report::receptionTimeTag>::type& reports =
            knowledgeBase_.findAllReports(reportToTrust->sender).get<
                     Report::receptionTimeTag> ();

   if ( logStream_ )
      *logStream_ << "Sender:" << reportToTrust->sender << ", type:"
               << reportToTrust->type << "\n";

   for ( ReportList::index<Report::receptionTimeTag>::type::const_iterator
            oldReportIt = reports.begin(); oldReportIt != reports.end(); ++oldReportIt )
   {
      // Do not use the report I judge about for the judgment
      if ( reportToTrust == *oldReportIt )
         continue;

      const IntReport* oldIntReport = dynamic_cast<IntReport*> (*oldReportIt);
      if ( oldIntReport != 0 && oldIntReport->type > 0 && oldIntReport->type
               < 5 )
      {
         /*
          * Process this old report. It represents one piece of evidence.
          * updateTrustInType does the whole processing of each report.
          */
         ComputationParameters& psOldReport = parameterSet[oldIntReport->type
                  % 4];
         updateTrustInType(oldIntReport, psOldReport);
      }
      else
      {
         throw SimulationFailed(
                  string("TrustBamberger2010 cannot handle reports of class ")
                           + typeid(*oldReportIt).name());
      }
   }

   /*
    * Combine the parameters for every information type in an overall
    * trust value for the wanted information type (the type of the given
    * report).
    *
    * Note: In case that the report list above was empty, all intermediate
    * trust opinion contain zero evidence. The resulting trust opinion below
    * will have zero evidence as well then. This is exactly as intended.
    */

   // Final trust opinion.
   BinaryOpinion trust = combineTrustInTypes(reportToTrust, parameterSet);
   // Final trust value.
   float trustValue = trustToTrustValue(trust);

   // Print out debug information.
   int sumEvidence = trust.evidence(0) + trust.evidence(1);
   if ( csvStream )
   {
      ComputationParameters& psToTrust = parameterSet[reportToTrust->type % 4];
      *csvStream << trustValue << "," << sumEvidence << ",\""
               << psToTrust.competence << "\",\"" << psToTrust.predictability
               << "\",\"" << psToTrust.trust << "\"";
   }

   // Package the trust value for the method's return value.
   return make_pair(trustValue, 1 - trust.ignorance());
}

// ----------------------------------------------------------------------

string
TrustBamberger2010::getCsvHeader() const
{
   return "\"trust value\",\"evidence\",\"competence for type\",\"predictability for type\",\"trust for type\"";
}

// ----------------------------------------------------------------------

pair<bool, IntOpinion>
TrustBamberger2010::makeDecision( vector<pair<float, IntOpinion> > opinionList ) throw ()
{
   static const float threshold = 1.1f;

   /*
    * Discard the third with the lowest trust values.
    * Here in the trust model, all three opinions are the same. So, I can simply
    * discard the last one.
    */
   opinionList.pop_back();

   /*
    * Discount and fuse the opinions
    */
   IntOpinion resultingOpinion;
   for ( vector<pair<float, IntOpinion> >::const_iterator it =
            opinionList.begin(); it != opinionList.end(); ++it )
   {
      IntOpinion discountedRecommendation = it->second.beliefDiscountingBy(
               it->first);
      resultingOpinion.cumulativeFusionToSelf(discountedRecommendation);
   }

   /*
    * Find the expected value and apply the threshold
    */
   float maxExpectation = 0.0;
   for ( size_t i = 0; i < resultingOpinion.size(); ++i )
   {
      float expectation = resultingOpinion.probabilityExpectation(i);
      if ( maxExpectation < expectation )
         maxExpectation = expectation;
   }
   if ( maxExpectation > threshold )
   {
      return make_pair(true, resultingOpinion);
   }
   else
   {
      return make_pair(false, resultingOpinion);
   }
}

// ----------------------------------------------------------------------

void
TrustBamberger2010::updateCompetence( const IntReport* r,
         ComputationParameters& ps ) throw ()
{
   const IntInformationKnowledge* i = r->associatedInformation();
   /// @todo For the reference value, I may not use values of the same sender. Right?
   float
            relativeError =
                     static_cast<float> (abs(r->value - i->value))
                              / static_cast<float> (InformationDescription::maxIntValues[r->type]);
   float ownBelief = i->opinion.belief(i->value);
   ps.competence.addEvidence(0, ownBelief * (1 - relativeError));
   ps.competence.addEvidence(1, ownBelief * relativeError);
}

// ----------------------------------------------------------------------

void
TrustBamberger2010::updatePredictability( const IntReport* r,
         ComputationParameters& ps ) throw ()
{
   // Make the prediction
   vector<pair<float, IntOpinion> > opinionList(3,
            make_pair(trustToTrustValue(ps.trust), r->opinion));
   pair<bool, IntOpinion> prediction = makeDecision(opinionList);

   // Compute the prediction error
   const IntInformationKnowledge* i = r->associatedInformation();
   /*
    * Simplification: For our decision maker, the predicted value is always the
    * same as the value in the report. With this simplification, I can handle
    * the case of a predicted opinion with several equal maximal elements
    * (e.g. 0.0, 0.0, 0.0, 0.0, 0.0).
    */
   int predictedValue = r->value;
   bool equalValues = r->value == i->value;
   double predictedBelief = static_cast<double> (prediction.second.belief(
            predictedValue));
   double ownBelief = static_cast<double> (i->opinion.belief(predictedValue));
   double error = ownBelief - predictedBelief;
   if ( (prediction.first && (!equalValues) && error < 0)
            || ((!prediction.first) && equalValues && error > 0) )
   {
      // Prediction error, i.e., wrong decision.
      ps.errorSum += error;
      double KP = prediction.first ? 0.2 : 0.1;
      double y = KP * error + 0.1 * ps.errorSum;
      if ( y >= 0 )
      {
         ps.c *= 1.0 + y;
      }
      else
      {
         ps.c /= (1.0 - y);
      }

      if ( logStream_ )
      {
         *logStream_ << "    – accepted:" << prediction.first << ", vM:"
                  << i->value << ", vA:" << predictedValue << ", error:"
                  << error << ", c:" << ps.c << ", errorSum:" << ps.errorSum
                  << ", y:" << y << "\n";
         *logStream_ << "      oM:" << i->opinion << ", oA:"
                  << prediction.second << "\n";
      }
   }
   else
   {
      // No prediction error. c remains unchanged.
      if ( logStream_ )
         *logStream_ << "    + accepted:" << prediction.first << ", vM:"
                  << i->value << ", vA:" << predictedValue << ", c:" << ps.c
                  << "\n";
   }

   /* Compute the new predictability opinion from c.
    *
    * I do this in evidence notation. From c = b+ / b- follows r+ / r- = c
    * as well. Things are quite simple then. Just obtain the new amount of
    * evidence and split it on r+ and r- according to c. Simple and fast.
    *
    * The amount of evidence should always be the same as that of the
    * competence. This is important for the averaging fusion. */
   double newEvidenceSum = static_cast<double> (accumulate(
            ps.competence.beginEvidence(), ps.competence.endEvidence(), 0.0f));
   double rminus = newEvidenceSum / (1.0 + ps.c);
   ps.predictability.setEvidence(1, static_cast<float> (rminus));
   ps.predictability.setEvidence(0, static_cast<float> (rminus * ps.c));
}

// ----------------------------------------------------------------------

void
TrustBamberger2010::updateTrustInType( const IntReport* r,
         ComputationParameters& ps ) throw ()
{
   updateCompetence(r, ps);
   if ( withPredictability_ )
   {
      float f = .0;
      updatePredictability(r, ps);
      ps.trust = ps.competence.averagingFusion(ps.predictability);
   }
   else
   {
      ps.trust = ps.competence;
   }

   if ( logStream_ )
   {
      *logStream_ << "      type:" << r->type << ", competence:"
               << ps.competence << " / " << ps.competence.evidence(0)
               / ps.competence.evidence(1) << " / " << trustToTrustValue(
               ps.competence);
      if ( withPredictability_ )
         *logStream_ << ", predictability:" << ps.predictability << " / "
                  << ps.predictability.evidence(0)
                           / ps.predictability.evidence(1) << " / "
                  << trustToTrustValue(ps.predictability);
      *logStream_ << ", trust:" << ps.trust << " / " << ps.trust.evidence(0)
               / ps.trust.evidence(1) << " / " << trustToTrustValue(ps.trust)
               << "\n";
   }
}

// ----------------------------------------------------------------------

float
TrustBamberger2010::trustToTrustValue( const BinaryOpinion& trust ) throw ()
{
   static bool withChannelCapacity =
            simulationController_.environment().optional_bool_param(
                     "trust_value_with_channel_capacity", false);
   if ( withChannelCapacity )
   {
      // In evidence representation with W = 2
      const double W = 2.0; // Weight of the base rate
      double rplus = static_cast<double> (trust.evidence(0));
      double rminus = static_cast<double> (trust.evidence(1));

      double S = rplus + rminus;
      if ( S <= 0.0 )
      {
         return 0.5;
      }
      else
      {
         double x = rplus / S;

         double Hb = 0.0;
         if ( x > 0.0 && x < 1.0 )
            Hb = -x * log2(x) - (1.0 - x) * log2(1.0 - x);
         // else: For x=0 and x=1, Hb=0. x cannot be <0 or >1.

         if ( rplus > rminus )
            return static_cast<float> ((S / (S + W) * (1.0 - Hb) + 1.0) / 2.0);
         else
            return static_cast<float> ((-S / (S + W) * (1.0 - Hb) + 1.0) / 2.0);
      }
   }
   else
   {
      return trust.probabilityExpectation(0);
   }
}

}

#endif /* ENABLE_VANET */
