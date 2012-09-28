/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transmission_model.h"
#include "sys/communication_model.h"
#include "sys/world.h"
#include "sys/node.h"

namespace shawn
{
  

   TransmissionModel::
   TransmissionModel()
      : world_      ( NULL )
   {}
   // ----------------------------------------------------------------------
   TransmissionModel::
   ~TransmissionModel()
   {}
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   set_world( World& w ) 
      throw()
   {
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   World&
   TransmissionModel::
   world_w( void ) 
      throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   const World&
   TransmissionModel::
   world( void )
      const throw()
   {
      assert( world_ != NULL );
      return *world_;
   }
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   init( void )
      throw()
   {
      assert( world_ != NULL );
   }
   // ----------------------------------------------------------------------
   void
   TransmissionModel::
   reset( void ) 
      throw()
   {

   }
   // ----------------------------------------------------------------------
   bool
   TransmissionModel::
   transmission_in_range(Node *src, Node *dst, MessageInfo *mi)
   {
      if(world().communication_model().exists_communication_upper_bound())
      {
         double euc_distance = euclidean_distance(src->real_position(),
            dst->real_position());
         double g_range = 
            world().communication_model().communication_upper_bound();

         if(euc_distance <= g_range * mi->range_)
         {
            return true;
         }

         return false;
      }
      else
      {
         return true;
      }

   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transmission_model.cpp,v $
 * Version $Revision: 370 $
 * Date    $Date: 2009-11-12 14:12:56 +0100 (Thu, 12 Nov 2009) $
 *-----------------------------------------------------------------------
 * $Log: transmission_model.cpp,v $
 *-----------------------------------------------------------------------*/
