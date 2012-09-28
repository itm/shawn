/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <assert.h>

#include "sys/worlds/cuboid_world_factory.h"
#include "sys/vec.h"
#include "sys/util/defutils.h"
#include "sys/misc/random/basic_random.h"

namespace shawn
{

   CuboidWorldFactory::
   CuboidWorldFactory()
      : length_ ( -1.0 ),
        width_  ( -1.0 ),
        height_ ( -1.0 )
   {}
   // ----------------------------------------------------------------------
   CuboidWorldFactory::
   ~CuboidWorldFactory()
   {}
   // ----------------------------------------------------------------------
   void
   CuboidWorldFactory::
   set_size( double w, double h, double l )
      throw()
   {
      assert( w>0.0 );
      assert( h>0.0 );
      assert( l>0.0 );

      width_ = w;
      height_ = h;
      length_ = l;
   }
   // ----------------------------------------------------------------------
   double
   CuboidWorldFactory::
   width( void )
      const throw()
   {
      return width_;
   }
   // ----------------------------------------------------------------------
   double
   CuboidWorldFactory::
   height( void )
      const throw()
   {
      return height_;
   }
   // ----------------------------------------------------------------------
   double
   CuboidWorldFactory::
   length( void )
      const throw()
   {
      return length_;
   }
   // ----------------------------------------------------------------------
   Vec
   CuboidWorldFactory::
   new_position( void )
      const throw()
   {
      assert( length() > 0.0 );
      assert( width() > 0.0 );
      assert( height() > 0.0 );

      return Vec( uniform_random_0i_1i()*width(),
                  uniform_random_0i_1i()*height(),
                  uniform_random_0i_1i()*length() );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/cuboid_world_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: cuboid_world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
