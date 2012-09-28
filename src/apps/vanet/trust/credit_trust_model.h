/**
 * @file credit_trust_model.h
 * Defines the trust model class vanet::CREDITModel.
 *
 * @author Lu Zeng
 *
 * $Revision: 605 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/trust/credit_trust_model.h $
 * $Id: credit_trust_model.h 605 2011-05-31 09:29:20Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Lu Zeng<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_TRUST_CREDITMODEL_H
#define __SHAWN_APPS_VANET_TRUST_CREDITMODEL_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "trust_computer.h"
#include "regret_reputation_model.h"
#include "apps/vanet/knowledge_base/report.h"
#include "apps/vanet/knowledge_base/trust_attitude.h"
#include "apps/vanet/knowledge_base/knowledge_base.h"
#include "sys/logging/logging.h"

namespace vanet
{

/**
 * Trust model of Ramchurn (2004) adopted to VANETs.
 */
class CREDITModel : public TrustComputer
{
public:
   CREDITModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb);

   CREDITModel( const __LOGGER_OBJECT& logger, KnowledgeBase& kb,TrustComputer*reputationComputer );

   virtual
   ~CREDITModel();

   virtual std::pair<float, float>
   computeTrustValue( const Report* report );

   virtual std::ostream&
   computeCsvRecord( std::ostream& os, const Report* report );

   virtual std::string
   getCsvHeader() const;

private:

   enum FuzzySetLabel
   {
      Bad, Average, Good
   };

   enum MembershipFunctionType
   {
      Triangular, Gaussian
   };

   struct Interval
   {
      Interval()
      :inf(0.0), sup(0.0)
      {}

      Interval(std::pair<float, float> range)
      : inf(range.first), sup(range.second)
      {}

      Interval
      intersect_with(const Interval& i2)
      {
         Interval result;

         result.inf = std::max(inf, i2.inf);
         result.sup = std::min(sup, i2.sup);
         if (result.inf - result.sup > 0.00001)
         {
            result.sup = 0.0;
            result.inf = 0.0;
         }

         return result;
      }

      float inf;
      float sup;
   };

   KnowledgeBase& knowledgeBase_;
   TrustComputer* reputationComputer_;
   __LOGGER_OBJECT logger_;
   int contractsSize;
   static const int intimateOfConfidence = 10;

   /**
    * Assigns a utility to a difference between the reported value and the
    * assumed right value. This difference describes the quality of the
    * provided value. So it can be mapped to a utility.
    *
    * @param x the difference between the reported value and the assumed
    *          right value
    * @param type the information type of the reported value
    * @return the utility of the given value quality
    * @throw None
    */
   float
   utilityFunction( int x, InformationDescription::Type type );

   /**
    * Computes the degree of membership of an element @c rho to the the set with
    * the given label. This method implements several membership function. You
    * can select the desired one with the argument @c mode.
    *
    * The triangular membership function has the factor k = 1. The Gaussian
    * membership function has the form exp(-M_PI* pow(rho-miu, 2)), with
    * mio depending on the set label: for "bad" it is -1, for average it is 0
    * and for good it is +1.
    *
    * @param rho the element for which the membership should be computed
    * @param label the name of the set for which the membership should be computed
    * @param mode the type of the membership function to use
    * @return the degree of membership
    * @throw None
    */
   float
   membershipFunction( float rho, FuzzySetLabel label, MembershipFunctionType mode );

   /**
    * Provides the range of values that have a degree of membership at least
    * as high as the given degree.
    *
    * @param degree the minimal allowed degree of membership
    * @param label the name of the fuzzy set
    * @param mode the type of the membership function
    * @return the minimal and maximal value of the computed range
    * @throw invalid_argument if value <= 0 and mode = Gaussian
    */
   std::pair<float, float>
   inversMembershipFunction( float degree,
            FuzzySetLabel label, MembershipFunctionType mode );

   /**
    * Obtains all relevant reports from the knowledge base and computes
    * the utility variations for them.
    *
    * @param re the report for which trust should be computed
    * @return the list of utility variations
    */
   std::list<float>
   getVariationsForCurrentReport( const Report *re );

   /**
    * Provides the weight of the reputation value for the combination of
    * trust and reputation.
    *
    * @param reliabilityOfConfidence
    *            a degree of certainty about the computed confidence value
    * @param reliabilityOfReputation
    *            a degree of certainty about the computed reputation value
    * @param reportedListSize
    *            the number of reports on which the trust computation is based
    * @return the weight of the reputation value in the trust computation
    */
   float getReputationWeight( float reliabilityOfConfidence, float reliabilityOfReputation, int reportListSize );

   /**
    * Computes the degree of certainty about the computed confidence value.
    * The vehicle can never be sure whether the computed confidence is right.
    * The provided value indicates how certain the vehicle is about the
    * confidence value. For example, a high number of reports in the trust
    * computation leads to a high certainty about the confidence value; so
    * the reliability is high in this case.
    *
    * @param reportListSize
    *            the number of reports used in the trust computation
    * @return the reliability of the confidence value
    */
   float computeConfidenceReliability( int reportListSize );

   /**
    * Computes the degree of certainty about the computed trust value.
    *
    * @param reliabilityOfConfidence
    *            a degree of certainty about the used confidence value
    * @param reliabilityOfReputation
    *            a degree of certainty about the used reliability value
    * @param weightOfReputation
    *            the weight of the reputation in the trust computation
    * @return a degree of certainty about the computed trust value
    */
   float computeTrustReliability( float reliabilityOfConfidence, float reliabilityOfReputation, float weightOfReputation );

   /**
    * Performs the actual trust computation. If an output stream is given
    * as the second argument, statistics data is put out in the CSV file.
    *
    * @param reportPtr
    *            the report for which the trust value should be computed
    * @param csvStream
    *            a stream to the CSV file or @c null
    * @return a pair of the trust value and the reliability of the trust value
    */
   std::pair<float, float>
   computeTrustValueAndCsv( const Report* reportPtr, std::ostream* csvStream );
};

}

#endif /* VANET_ENABLE */
#endif /* __SHAWN_APPS_VANET_TRUST_CREDITMODEL_H */
