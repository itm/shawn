/**
 * @file modifying_disseminator.h
 * Defines the disseminator vanet::ModifyingDisseminator.
 *
 * @author Lu Zeng
 * @author Walter Bamberger
 *
 * $Revision: 539 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/dissemination/modifying_disseminator.h $
 * $Id: modifying_disseminator.h 539 2011-04-13 01:26:31Z andylz $
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

#ifndef __SHAWN_APPS_VANET_DISSEMINATION_MODIFYING_DISSEMINATOR_H
#define __SHAWN_APPS_VANET_DISSEMINATION_MODIFYING_DISSEMINATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "disseminator.h"

namespace vanet
{

class KnowledgeBase;

/**
 * Dissemination module that models a certain kind of attacker.
 *
 * The algorithm sends good information up to a certain point in time and from
 * then on a mixture of good and bad information. The bad information is derived
 * from an observation in the knowledge base: This observation is taken and its
 * value is modified. All other properties of the observation are kept. So no
 * new piece of information is invented.
 */
class ModifyingDisseminator : public Disseminator
{
public:
   ModifyingDisseminator( shawn::Node& sendingNode, const KnowledgeBase& kb );

   virtual
   ~ModifyingDisseminator();

   virtual void
   disseminate() throw ();

private:
   const KnowledgeBase& knowledgeBase_;
};
}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_DISSEMINATION_MODIFYING_DISSEMINATOR_H */
