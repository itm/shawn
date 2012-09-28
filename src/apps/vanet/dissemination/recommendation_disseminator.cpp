/*
 * Recommendation_disseminator.cpp
 *
 *  Created on: Apr 13, 2011
 *      Author: andy
 */

#include "recommendation_disseminator.h"

#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"

using namespace shawn;


namespace vanet
{

RecommendationDisseminator::RecommendationDisseminator( Node& sendingNode,
         const KnowledgeBase& kb ) :
   Disseminator(sendingNode), knowledgeBase_(kb)
{
}

// ----------------------------------------------------------------------

RecommendationDisseminator::~RecommendationDisseminator()
{
}

// ----------------------------------------------------------------------

void RecommendationDisseminator::disseminate() throw ()
{
   // TODO May throw runtime_error
   const TrustAttitudeList& newTrusts =
            knowledgeBase_.findRecentTrustAttitudes();
   for ( TrustAttitudeList::const_reverse_iterator it = newTrusts.rbegin(); it
            != newTrusts.rend(); ++it )
   {
      if ( (*it) != 0 )
      {
         VanetTrustMessage* m = new VanetTrustMessage();

         m->name = (*it)->trustedNode;
         m->type = (*it)->type;
         m->time = (*it)->lastInteractionTime;
         m->trustValue = (*it)->trust.first;
         send(m);
      }
   }
}


}

#endif /* ENABLE_VANET */
