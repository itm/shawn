/**
 * @file observation_disseminator.h
 * Defines the disseminator vanet::RecommendationDisseminator.
 *
 * @author Lu Zeng
 *
 * $Revision: 534 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/dissemination/recommendation_disseminator.h $
 * $Id: recommendation_disseminator.h 534 2011-04-13 23:32:54Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Lu Zeng<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_DISSEMINATION_RECOMMENDATION_DISSEMINATOR_H_
#define __SHAWN_APPS_VANET_DISSEMINATION_RECOMMENDATION_DISSEMINATOR_H_
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "disseminator.h"

namespace vanet
{

class KnowledgeBase;

/**
 * Dissemination module that sends only pieces of information that have been
 * perceived by the vehicle directly.
 *
 * This dissemination algorithm goes through the list of own trust attitudes and
 * spreads out the most recent ones.
 *
 * Sending only own trust attitudes avoids statistical dependencies in the
 * information integration at the receiver. It also avoids a circular
 * information flow.
 */
class RecommendationDisseminator : public vanet::Disseminator
{
public:
   RecommendationDisseminator( shawn::Node& sendingNode, const KnowledgeBase& kb );

   virtual
   ~RecommendationDisseminator();

   virtual void
   disseminate() throw ();

private:
   const KnowledgeBase& knowledgeBase_;
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_DISSEMINATION_RECOMMENDATION_DISSEMINATOR_H_ */
