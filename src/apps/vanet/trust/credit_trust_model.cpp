/**
 * @file credit_trust_model.cpp
 * Implements the trust model class vanet::CREDITModel.
 *
 * @author Lu Zeng
 *
 * $Revision: 606 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/credit_trust_model.cpp $
 * $Id: credit_trust_model.cpp 606 2011-05-31 22:15:39Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Lu Zeng<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "credit_trust_model.h"
#ifdef ENABLE_VANET

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace shawn;

namespace vanet
{

CREDITModel::CREDITModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb ) :
logger_(logger), knowledgeBase_(kb), reputationComputer_(0)
{
}

CREDITModel::CREDITModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb,TrustComputer* reputationComputer ) :
logger_(logger), knowledgeBase_(kb), reputationComputer_(reputationComputer)
{
}

CREDITModel::~CREDITModel()
{
}
/*
   Exponential utility
   power: the power of x value in function
 */
float
CREDITModel::utilityFunction( int x, InformationDescription::Type type )
{
   const float a = 0.5;
   return exp(-a * x);
}

float
CREDITModel::membershipFunction( float rho, FuzzySetLabel label, MembershipFunctionType mode )
{
   if (mode == Triangular)
   {
      switch ( label )
      {
         case Bad:
         if ( rho > 0 )
          return 0;
         else
         return -rho;

         case Average:
         if ( rho < 0 )
         return 1 + rho;
         else
         return 1 - rho;

         case Good:
         if ( rho < 0 )
         return 0;
         else
         return rho;

         default:
         return 0;
      }
   }
   else
   {
      float miu_map[] = {-1, 0, 1};
       return exp(-M_PI* pow(rho-miu_map[label], 2));
   }
}

pair<float, float>
CREDITModel::inversMembershipFunction( float value,
         FuzzySetLabel label, MembershipFunctionType mode )
{
   if (mode == Triangular)
   {
      switch ( label )
         {
            case Bad:
            return make_pair(-1.0f, -value);

            case Average:
            if ( value == 1 )
               return make_pair(0.0f, 0.0f);

            else
               return make_pair(value - 1.0f, 1.0f - value);

            case Good:
            return make_pair(value, 1.0f);

            default:
            return make_pair(-1.0f, 1.0f);
         }
   }
   else
   {
      if (value <= 0.0)
      {
         // @todo throw bad argument
      }
      float root = sqrt(log(value)/(-M_PI));
         float miuBad(-1),miuAverage(0),miuGood(1);

         switch ( label )
            {
               case Bad:
               return make_pair(-1.0f, root + miuBad);

               case Average:
               return make_pair(-root, root);

               case Good:
               return make_pair(- root + miuGood, 1.0f);

               default:
               return make_pair(-1.0f, 1.0f);
            }
   }
}

std::list<float>
CREDITModel::getVariationsForCurrentReport( const Report *re )
{
   const int agreed_difference = 0;
   std::list<float> utilityVariations;
   ReportList reportList = knowledgeBase_.findAllReports(re->sender, re->type);
   contractsSize = reportList.size();
   for ( ReportList::iterator it = reportList.begin(); it != reportList.end(); ++it )
   {
      IntReport* int_report = dynamic_cast<IntReport*> (*it);
      if (int_report == 0)
      {
         // @todo throw something
      }

      IntInformationKnowledge* ik = dynamic_cast<IntInformationKnowledge*> (int_report->associatedInformation());
      if (ik == 0)
      {
         // @todo throw something
      }
      float infoValue = ik->value;

      int real_difference = abs(infoValue - (dynamic_cast<IntReport*> (*it))->value);
      float t = utilityFunction(agreed_difference, re->type) - utilityFunction(
               real_difference, re->type);
      utilityVariations.push_back(t);
   }
   return utilityVariations;
}

float CREDITModel::getReputationWeight( float reliabilityOfConfidence, float reliabilityOfReputation, int reportListSize )
{
   if(reliabilityOfReputation <= 0.0)
      return 0.0;

   const float a = 0.2;
   float weightOfReliabilityOfConfidence = 1 - exp( -a * reportListSize );

   return (1 - weightOfReliabilityOfConfidence) * reliabilityOfReputation /
            ( weightOfReliabilityOfConfidence * reliabilityOfConfidence
                     + (1 - weightOfReliabilityOfConfidence) * reliabilityOfReputation );
}

float CREDITModel::computeConfidenceReliability( int reportListSize )
{
   if ( reportListSize > intimateOfConfidence )
         return 1;
      else
         return sin( M_PI * reportListSize / (2 * intimateOfConfidence));
}

float CREDITModel::computeTrustReliability( float reliabilityOfConfidence, float reliabilityOfReputation, float weightOfreputation )
{
   return ( 1 - weightOfreputation ) * reliabilityOfConfidence
            + weightOfreputation * reliabilityOfReputation;
}

ostream&
CREDITModel::computeCsvRecord( ostream& os, const Report* report )
{
   computeTrustValueAndCsv(report, &os);
   return os;
}

string
CREDITModel::getCsvHeader() const
{
	string st = "\"type of information\",\"final value\",\"number of contracts\",\"confidence value\"";
	
	if ( reputationComputer_ != 0 )
		st = st + reputationComputer_->getCsvHeader();

   return st;
}

std::pair<float, float>
CREDITModel::computeTrustValue( const Report* report )
{
   return computeTrustValueAndCsv(report, 0);
}

std::pair<float, float>
CREDITModel::computeTrustValueAndCsv(  const Report* report, std::ostream* csvStream  )
{
   float trustValue = 1;
   float trustReliability = 0;
   try
   {
      MembershipFunctionType mode = Gaussian;
      std::list<float> utilityVariations = getVariationsForCurrentReport(report);

      /*
       * Obtain the Gaussian model parameters (mean and variance)
       * for the utility variations.
       */
      float expectedValue = mean(utilityVariations);
      float standardDeviation = sqrt(variance(utilityVariations, expectedValue));

      /*
       * Compute the confidence interval and the confidence levels.
       */
      const float l = 1.96;
      float rho1 = expectedValue - standardDeviation * l;
      float rho2 = expectedValue + standardDeviation * l;
      float CBad = min(membershipFunction(rho1, Bad, mode),
               membershipFunction(rho2, Bad, mode));
      float CAverage = min(membershipFunction(rho1, Average, mode), membershipFunction(
               rho2, Average, mode));
      float CGood = min(membershipFunction(rho1, Good, mode), membershipFunction(rho2,
               Good, mode));

      /*
       * Compute the range of expected values and the utility loss.
       */
      Interval EdeltaUBad(inversMembershipFunction(CBad, Bad, mode));
      Interval EdeltaUAverage(inversMembershipFunction(CAverage, Average, mode));
      Interval EdeltaUGood(inversMembershipFunction(CGood, Good, mode));
      Interval EdeltaInt = EdeltaUBad.intersect_with(EdeltaUAverage).intersect_with(EdeltaUGood);
      float utilityLoss = max(EdeltaInt.inf, EdeltaInt.sup);

      /*
       * Obtain the confidence value, the reputation value and the trust value.
       */
      float CValue = min(1.0f, 1.0f - utilityLoss);
      float reliabilityOfConfidence = computeConfidenceReliability( contractsSize );

      float reliabilityOfReputation = 0.0;
      float reputationValue = 0.0;
      if ( reputationComputer_ != 0 )
      {
         pair<float, float> reputation = reputationComputer_->computeTrustValue(report);
         reputationValue = reputation.first;
         reliabilityOfReputation = reputation.second;
      }
      float reputationWeight = getReputationWeight(reliabilityOfConfidence, reliabilityOfReputation, contractsSize);

      trustValue = ( 1 - reputationWeight) * CValue + reputationWeight * reputationValue;
      trustReliability = computeTrustReliability( reliabilityOfConfidence, reliabilityOfReputation, reputationWeight );

      if ( csvStream )
      {
         *csvStream << report->type << "," << trustValue << "," << contractsSize << "," << CValue;
         if (reputationComputer_ != 0) {
            reputationComputer_->computeCsvRecord(*csvStream, report);
         }
      }
   }
   catch ( const out_of_range& oe )
   {
   }

   return make_pair(trustValue, trustReliability);
}

}

#endif /* ENABLE_VANET */
