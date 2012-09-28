/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/uniform_3d_point_gen.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/box.h"

using namespace shawn;
using namespace std;
using namespace reading;

namespace topology
{

   Uniform3DPointGenerator::
   Uniform3DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Uniform3DPointGenerator::
   ~Uniform3DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Box
   Uniform3DPointGenerator::
   extract_box( ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      //Box b = brh->domain();
      //cout << "BB " << b.lower() << " || " << b.upper() << endl;

      return brh->domain();
   }
   // ----------------------------------------------------------------------
   std::string
   Uniform3DPointGenerator::
   name( void )
      const throw()
   { return "uniform_3d"; }
   // ----------------------------------------------------------------------
   std::string
   Uniform3DPointGenerator::
   description( void )
      const throw()
   { return "uniform random distribution for a 3D topology, based on simple sampling"; }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_3d_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_3d_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
