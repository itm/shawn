/**
 * @file disseminator.h
 * Defines the interface vanet::Disseminator for algorithms that spread out
 * messages in the vanet.
 *
 * @author Walter Bamberger
 *
 * $Revision: 528 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/dissemination/disseminator.h $
 * $Id: disseminator.h 528 2011-04-11 18:30:44Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_DISSEMINATION_DISSEMINATOR_H
#define __SHAWN_APPS_VANET_DISSEMINATION_DISSEMINATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/node.h"

namespace vanet
{

/**
 * Interface for algorithms that distribute information in the VANET.
 */
class Disseminator
{
public:
   Disseminator( shawn::Node& sendingNode ) :
      node_(&sendingNode)
   {
   }

   virtual
   ~Disseminator()
   {
   }

   virtual void
   setNode( shawn::Node& sendingNode )
   {
      node_ = &sendingNode;
   }

   /**
    * Determines what information to send and sends it then.
    */
   virtual void
   disseminate() throw () = 0;

protected:
   shawn::Node* node_;

   virtual void
   send( const shawn::MessageHandle& mh )
   {
      node_->send(mh);
   }
};

}
#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_DISSEMINATION_DISSEMINATOR_H */
