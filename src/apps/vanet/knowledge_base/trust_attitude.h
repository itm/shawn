/**
 * @file trust_attitude.h
 * Defines the data containers vanet::Trust
 * with its list vanet::TrustList
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

#ifndef __SHAWN_APPS_VANET_KNOWLEDGE_BASE_TRUST_H
#define __SHAWN_APPS_VANET_KNOWLEDGE_BASE_TRUST_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include <set>

namespace vanet
{

/**
 * Record of a piece of trust information after computation from trust model
 * for a report.
 * This data will later be packed as trust report
 * then send to other nodes as recommendations for reputation.
 */
struct TrustAttitude
{
   std::string trustedNode;
   /** @todo Should not be in here. Could be extracted from the report list of the trusted node */
   int lastInteractionTime;
   std::pair<float, float> trust;
   InformationDescription::Type type;

   /**
    * Virtual destructor for polymorphism.
    */
   virtual
   ~TrustAttitude()
   {
   }
};

struct TrustAttitudeTemporalLess : public std::binary_function<TrustAttitude*,
         TrustAttitude*, bool>
{
   bool
   operator()( const TrustAttitude* const x, const TrustAttitude* const y ) const
   {
      return x->lastInteractionTime < y->lastInteractionTime;
   }
};

/**
 * List of trust attitudes ordered by the last interaction time.
 */
typedef std::multiset<TrustAttitude*, TrustAttitudeTemporalLess> TrustAttitudeList;

}

#endif /* VANET_ENABLE */
#endif /* __SHAWN_APPS_VANET_KNOWLEDGE_BASE_TRUST_H */
