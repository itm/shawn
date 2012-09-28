/**
 * @file modifying_disseminator.cpp
 * Implements the disseminator vanet::ModifyingDisseminator.
 *
 * @author Lu Zeng
 * @author Walter Bamberger
 *
 * $Revision: 586 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/dissemination/modifying_disseminator.cpp $
 * $Id: modifying_disseminator.cpp 586 2011-05-10 16:42:30Z andylz $
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

#include "modifying_disseminator.h"
#ifdef ENABLE_VANET

#include "../knowledge_base/knowledge_base.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/basic_random.h"
#include <iostream>
#include <cmath>

using namespace std;

namespace vanet
{

ModifyingDisseminator::ModifyingDisseminator( shawn::Node& sendingNode,
         const KnowledgeBase& kb ) :
   Disseminator(sendingNode), knowledgeBase_(kb)
{
}

// ----------------------------------------------------------------------

ModifyingDisseminator::~ModifyingDisseminator()
{
}

// ----------------------------------------------------------------------

void
ModifyingDisseminator::disseminate() throw ()
{
   /// @todo May throw runtime_error
   static int
            beginModifyingRound =
                     node_->world().simulation_controller().environment().required_int_param(
                              "begin_modifying_round");
   static double
            proportionOfModifiedMessages =
                     node_->world().simulation_controller().environment().optional_double_param(
                              "proportion_of_modified_messages", 100.0);
   int count = 0;
   const ObservationList& newObservations =
            knowledgeBase_.findAllNewObservations();
   const int maxNumberOfModifiedMessages = static_cast<int> (round(
            proportionOfModifiedMessages
                     * static_cast<double> (newObservations.size())));

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
         m->time = intObservation->time;
         if ( node_->simulation_round() > beginModifyingRound && count
                  < maxNumberOfModifiedMessages )
         {
            count++;
            // Choose the value with the highest distance from the actual value.

            int maxValue = InformationDescription::maxIntValues[m->type];
             if ( intObservation->value >= maxValue / 2 ) // Integer division
             	m->value = 0;
             else
             	m->value = maxValue;
/*
            m->value = (int) (shawn::uniform_discrete_random(maxValue));
            if (m->value < 0)
               m->value = 0;
            else if (m->value > maxValue)
               m->value = maxValue;
*/
            m->opinion.addEvidence(m->value, 1.0);
         }
         else
         {
            m->opinion = intObservation->opinion;
            m->value = intObservation->value;
         }
         send(m);
      }
   }
}

}

#endif /* ENABLE_VANET */
