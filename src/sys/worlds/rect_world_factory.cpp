/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <assert.h>

#include "sys/worlds/rect_world_factory.h"
#include "sys/vec.h"
#include "sys/util/defutils.h"
#include "sys/misc/random/basic_random.h"

namespace shawn
{

   RectWorldFactory::
   RectWorldFactory()
      : width_  ( -1.0 ),
        height_ ( -1.0 )
   {}
   // ----------------------------------------------------------------------
   RectWorldFactory::
   ~RectWorldFactory()
   {}
   // ----------------------------------------------------------------------
   void
   RectWorldFactory::
   set_size( double w, double h ) 
      throw()
   {
      assert( w>0.0 );
      assert( h>0.0 );

      width_ = w;
      height_ = h;
   }
   // ----------------------------------------------------------------------
   double
   RectWorldFactory::
   width( void )
      const throw()
   {
      return width_;
   }
   // ----------------------------------------------------------------------
   double
   RectWorldFactory::
   height( void )
      const throw()
   {
      return height_;
   }
   // ----------------------------------------------------------------------
   Vec
   RectWorldFactory::
   new_position( void )
      const throw()
   {
      assert( width() > 0.0 );
      assert( height() > 0.0 );
      return Vec( uniform_random_0i_1i()*width(),
                  uniform_random_0i_1i()*height() );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/rect_world_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: rect_world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
