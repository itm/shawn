/**
 * @file regret_reputation_model.cpp
 * Defines the trust model class vanet::RegretReputationModel.
 *
 * @author Lu Zeng
 *
 * $Revision: 522 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/sumo/sumo_net.cpp $
 * $Id: andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Lu Zeng<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */


#include "regret_reputation_model.h"
#ifdef ENABLE_VANET

using namespace std;
using namespace shawn;

namespace vanet
{

RegretReputationModel::RegretReputationModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb ) :
         logger_(logger), knowledgeBase_(kb)
{
   // TODO Auto-generated constructor stub

}

RegretReputationModel::~RegretReputationModel()
{
   // TODO Auto-generated destructor stub
}

pair<float, float>
RegretReputationModel::computeTrustValue( const Report* report )
{
   return computeTrustValueAndCsv(report, 0);
}

ostream& RegretReputationModel::computeCsvRecord( std::ostream& os, const Report* report )
{
   computeTrustValueAndCsv(report, &os);
}

std::string RegretReputationModel::getCsvHeader() const
{
   return ",\"number of reputation\",\"reputation value\",\"reliability value of reputation\"";
}

pair<float, float>
RegretReputationModel::computeTrustValueAndCsv( const Report* report, ostream* csvStream )
{
   ReportList reportList = knowledgeBase_.findAllTrustReports(report->sender, report->type);
   recommendationSize = reportList.size();
   if ( recommendationSize == 0 )
      return std::make_pair( 0, 0 );

   float reputationValue = computeReputationValue(report);
   float intimateValue = computeIntimateLevel(recommendationSize);
   float deviationValue = computeReputationDeviation(report, reputationValue);
   float reliabilityValue = computeReputationReliability(intimateValue, deviationValue, recommendationSize);

   if (csvStream != 0) {
      *csvStream << "," << recommendationSize << "," << reputationValue << "," << reliabilityValue;
   }

   return  std::make_pair(reputationValue, reliabilityValue );
}

float RegretReputationModel::getWeightBasedTime(int time, InformationDescription::Type type )
{
   float a = 0;
   switch (type)
      {
      case InformationDescription::OwnMovement: a = 5.0; break;
      case InformationDescription::IntSituational: a = 5.0; break;
      case InformationDescription::IntShort: a = 2.0; break;
      case InformationDescription::IntMedium: a = 1.5; break;
      case InformationDescription::IntLong: a = 1.0; break;
      case InformationDescription::IntNone: a = 0; break;
      default: a = 1.0; break;
      }
   float week = float(time) / (3600 * 24 * 7);
   return exp(- a * week);
}

float RegretReputationModel::getWeightOfDeviation(int number)
{
   float a = 0.1;
   return 1 - exp( -a * number );
}

float RegretReputationModel::computeReputationValue(const Report* report)
{
   ReportList reportList = knowledgeBase_.findAllTrustReports(report->sender, report->type);

   float sumTrustsMalWeights = 0;
   float sumWeights = 0;

   for ( ReportList::iterator it = reportList.begin(); it != reportList.end(); ++it )
      {
         TrustReport* trustReport = dynamic_cast<TrustReport*> (*it);
         float weight = getWeightBasedTime(trustReport->time, report->type);
         sumWeights += weight;
         sumTrustsMalWeights += weight * trustReport->trustValue;
      }

   return sumTrustsMalWeights / sumWeights;
}

float RegretReputationModel::computeIntimateLevel(int number)
{
   if ( number > intimateOfReputation )
      return 1;
   else
      return sin( PI * number / (2 * intimateOfReputation));
}

float RegretReputationModel::computeReputationDeviation(const Report* report, float reputationValue)
{
   ReportList reportList = knowledgeBase_.findAllTrustReports(report->sender, report->type);

   float deviation = 0;
   float sumWeights = 0;

   for ( ReportList::iterator it = reportList.begin(); it != reportList.end(); ++it )
      {
         TrustReport* trustReport = dynamic_cast<TrustReport*> (*it);
         float weight = getWeightBasedTime(trustReport->time, report->type);
         sumWeights += weight;
         deviation += weight * fabs(trustReport->trustValue - reputationValue);
      }
   if( sumWeights == 0)
      return 1;
   else
      deviation /= sumWeights;

   return 1 - deviation;
}

float RegretReputationModel::computeReputationReliability(float intimateValue, float deviationValue, int number)
{
   float weightOfDeviation = getWeightOfDeviation(number);
   return (1 - weightOfDeviation) * intimateValue + weightOfDeviation * deviationValue;
}

}

#endif /* ENABLE_VANET */

