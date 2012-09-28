/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/uniform_2d_point_gen.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/box.h"

using namespace shawn;
using namespace std;
using namespace reading;

namespace topology
{

   Uniform2DPointGenerator::
   Uniform2DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Uniform2DPointGenerator::
   ~Uniform2DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Box
   Uniform2DPointGenerator::
   extract_box( ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      Box b=brh->domain();
      if( (b.lower().z() > 0.0) ||
          (b.upper().z() < 0.0) )
         throw std::runtime_error(string("topology '") +
                                  brh->name() +
                                  string("' does not include the z=0 plane"));

      return Box( Vec(b.lower().x(),b.lower().y(),0.0),
                  Vec(b.upper().x(),b.upper().y(),0.0) );
   }
   // ----------------------------------------------------------------------
   std::string
   Uniform2DPointGenerator::
   name( void )
      const throw()
   { return "uniform_2d"; }
   // ----------------------------------------------------------------------
   std::string
   Uniform2DPointGenerator::
   description( void )
      const throw()
   { return "uniform random distribution for a 2D topology, based on simple sampling"; }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_2d_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_2d_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
