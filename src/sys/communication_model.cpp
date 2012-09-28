/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/communication_model.h"
#include "sys/node.h"
#include "sys/world.h"

#include <cassert>

namespace shawn
{

   CommunicationModel::
   CommunicationModel()
      : world_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   CommunicationModel::
   ~CommunicationModel()
   {}
   // ----------------------------------------------------------------------
   void
   CommunicationModel::
   set_world( World& w )
      throw()
   {
      assert( world_ == NULL );
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   CommunicationModel::
   init( void )
      throw()
   {
      assert( world_ != NULL );
   }
   // ----------------------------------------------------------------------
   World&
   CommunicationModel::
   world_w( void )
      throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   const World&
   CommunicationModel::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   void 
       CommunicationModel::
       set_size_hint(double hint)
       throw()
   {
   }

   // ----------------------------------------------------------------------
   bool  
	   CommunicationModel::
	   in_domain(const Node&, const Node&) 
	   const throw()
   {
	   return true;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/communication_model.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: communication_model.cpp,v $
 *-----------------------------------------------------------------------*/
