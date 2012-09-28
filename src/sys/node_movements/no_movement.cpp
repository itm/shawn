/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/node_movements/no_movement.h"


namespace shawn
{

   NoMovement::
   NoMovement()
      : initialized_ ( false )
   {}
   // ----------------------------------------------------------------------
   NoMovement::
   ~NoMovement()
   {}
   // ----------------------------------------------------------------------
   void
   NoMovement::
   init( Node& v )
      throw()
   {
      assert( !initialized_ );
      initialized_ = true;
      set_node(v);
      movement_initial_boxes();
   }
   // ----------------------------------------------------------------------
   void
   NoMovement::
   init_from( Node& v,
              NodeMovement& nm )
      throw()
   {
      assert( !initialized_ );
      initialized_ = true;
      set_node( v );

      //movement_initial_boxes(); //Replaced by following
	  movement_boxes_from(nm);

   }
   // ----------------------------------------------------------------------
   bool
   NoMovement::
   set_position( const Vec& p )
      throw()
   {
      if( initialized_ )
         return false;

      pos_ = p;
      return true;
   }
   // ----------------------------------------------------------------------
   Vec
   NoMovement::
   position( void )
      const throw()
   {
      return pos_;
   }
   // ----------------------------------------------------------------------
   Vec
   NoMovement::
   velocity( void )
      const throw()
   {
      return Vec(0,0,0);
   }
   // ----------------------------------------------------------------------
   std::string
   NoMovement::
   name( void )
      const throw()
   {
      return std::string("NoMovement");
   }

   // ----------------------------------------------------------------------
   void
	   NoMovement::
	   boxes_changed() 
	   throw()
   {}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/no_movement.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: no_movement.cpp,v $
 *-----------------------------------------------------------------------*/
