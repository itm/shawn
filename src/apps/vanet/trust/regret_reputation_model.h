/**
 * @file regret_reputation_model.h
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

#ifndef __SHAWN_APPS_VANET_TRUST_REPUTATION_MODEL_FOR_CREDIT_H_
#define __SHAWN_APPS_VANET_TRUST_REPUTATION_MODEL_FOR_CREDIT_H_
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "trust_computer.h"
#include "apps/vanet/knowledge_base/report.h"
#include "apps/vanet/knowledge_base/knowledge_base.h"
#include "sys/vec.h"
#include "sys/logging/logging.h"
#include <math.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#define PI 3.14159265

namespace vanet
{

class RegretReputationModel: public TrustComputer
{
public:
   RegretReputationModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb);
   
   virtual
   ~RegretReputationModel();
   
   virtual std::pair<float, float>
      computeTrustValue( const Report* report );
	  
   virtual std::ostream&
      computeCsvRecord( std::ostream& os, const Report* report );

   virtual std::string
   getCsvHeader() const;

private:
   KnowledgeBase& knowledgeBase_;
   __LOGGER_OBJECT logger_;
   int recommendationSize;
   static const int intimateOfReputation = 20;

   float getWeightBasedTime(int time, InformationDescription::Type type );
   float getWeightOfDeviation(int number);
   float computeReputationValue(const Report* report);
   float computeIntimateLevel(int number);
   float computeReputationDeviation(const Report* report, float reputationValue);
   float computeReputationReliability(float intimateValue, float deviationValue, int number);
   std::pair<float, float>
   computeTrustValueAndCsv( const Report* report, std::ostream* csvStream = 0 );
};

}

#endif /* VANET_ENABLE */
#endif /* __SHAWN_APPS_VANET_TRUST_REPUTATION_MODEL_FOR_CREDIT_H_ */
