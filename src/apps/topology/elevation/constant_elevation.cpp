/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/constant_elevation.h"
#include <sstream>

using namespace shawn;

namespace topology
{

   ConstantElevation::
   ConstantElevation( double x,
                      const std::string& name )
      : value_ ( x ),
        name_  ( name )
   {
      if( name_.empty() )
         {
            std::ostringstream oss;
            oss << "constant_elevation_" << x;
            name_=oss.str();
         }
      
      std::ostringstream oss;
      oss << "constant elevation at level " << x;
      description_=oss.str();
   }
   // ----------------------------------------------------------------------
   ConstantElevation::
   ~ConstantElevation()
   {}
   // ----------------------------------------------------------------------
   std::string
   ConstantElevation::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   ConstantElevation::
   description( void )
      const throw()
   { return description_; }
   // ----------------------------------------------------------------------
   double
   ConstantElevation::
   value( const Vec& v )
      const throw()
   { return value_; }
   // ----------------------------------------------------------------------
   Box
   ConstantElevation::
   domain( void )
      const throw()
   { return Box::INFINITE_3D_SPACE; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/constant_elevation.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: constant_elevation.cpp,v $
 *-----------------------------------------------------------------------*/
