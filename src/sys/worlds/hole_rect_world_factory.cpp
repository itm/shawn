/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <math.h>

#include "sys/worlds/hole_rect_world_factory.h"
#include "sys/vec.h"


namespace shawn
{

   HoleRectWorldFactory::
   HoleRectWorldFactory()
      : hole_width_  ( -1.0 ),
        hole_height_ ( -1.0 )
   {}
   // ----------------------------------------------------------------------
   HoleRectWorldFactory::
   ~HoleRectWorldFactory()
   {}
   // ----------------------------------------------------------------------
   void
   HoleRectWorldFactory::
   set_hole_size( double w, double h )
      throw()
   {
      assert( w > 0.0 );
      assert( h > 0.0 );
      hole_width_ = w;
      hole_height_ = h;
   }
   // ----------------------------------------------------------------------
   double
   HoleRectWorldFactory::
   hole_width( void )
      const throw()
   {
      return hole_width_;
   }
   // ----------------------------------------------------------------------
   double
   HoleRectWorldFactory::
   hole_height( void )
      const throw()
   {
      return hole_height_;
   }
   // ----------------------------------------------------------------------
   Vec
   HoleRectWorldFactory::
   new_position( void )
      const throw()
   {
      assert( width() > hole_width() );
      assert( height() > hole_height() );

      Vec v;

      while( 1 )
         {
            v = RectWorldFactory::new_position();
            
            if( fabs( v.x()-(width()/2.0) ) > (hole_width()/2.0) )
               return v;
            
            if( fabs( v.y()-(height()/2.0) ) > (hole_height()/2.0) )
               return v;
         }

      // just against warnings
      return v;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/hole_rect_world_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: hole_rect_world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
