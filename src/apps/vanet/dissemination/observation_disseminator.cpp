/**
 * @file observation_disseminator.cpp
 * Implements the disseminator vanet::ObservationDisseminator.
 *
 * @author Walter Bamberger
 *
 * $Revision: 539 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/dissemination/observation_disseminator.cpp $
 * $Id: observation_disseminator.cpp 539 2011-04-13 01:26:31Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "observation_disseminator.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"

using namespace shawn;

namespace vanet
{

ObservationDisseminator::ObservationDisseminator( Node& sendingNode,
         const KnowledgeBase& kb ) :
   Disseminator(sendingNode), knowledgeBase_(kb)
{
}

// ----------------------------------------------------------------------

ObservationDisseminator::~ObservationDisseminator()
{
}

// ----------------------------------------------------------------------

void
ObservationDisseminator::disseminate() throw ()
{
   /* Notes for writing a new disseminator that uses the allNewInformation list:
    *  - Do not send an InformationKnowledge that has a report with a
    *    receptionTime close to the current time (i.e., that has been
    *    received recently).
    *  - Prefer for m->time the newest own observation. If there is no own
    *    observation, use the newest reported observation.
    */
   const ObservationList& newObservations =
            knowledgeBase_.findAllNewObservations();
   for ( ObservationList::const_reverse_iterator it = newObservations.rbegin(); it
            != newObservations.rend(); ++it )
   {
      const IntObservation* intObservation =
               dynamic_cast<const IntObservation*> (*it);
      if ( intObservation != 0 )
      {
         VanetIntMessage* m = new VanetIntMessage();
         m->location = intObservation->location;
         m->type = intObservation->type;
         m->opinion = intObservation->opinion;
         m->time = intObservation->time;
         m->value = intObservation->value;
         send(m);
      }
   }
}

}

#endif /* ENABLE_VANET */
