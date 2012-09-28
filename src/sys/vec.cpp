/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include "sys/vec.h"


namespace shawn
{

   // ----------------------------------------------------------------------
   Vec::~Vec()
   {}
   // ----------------------------------------------------------------------
   double 
   Vec::
   euclidean_norm( void )
      const throw()
   {
      return sqrt( (x_*x_) + (y_*y_) + (z_*z_) ); 
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator << ( std::ostream& os, const Vec& v )
      throw()
   {
      return os << "(" << v.x() << " " << v.y() << " " << v.z() << ")";
   }
   // ----------------------------------------------------------------------
   double Vec::xy_direction( void ) const throw()
   {
      if( euclidean_norm() < EPSILON )
         return 0.0;
      if( x_ >= fabs(y_) ) {
         if( y_ < 0.0 )
            return atan(y_/x_) + 2.0*M_PI;
         else
            return atan(y_/x_);
      }
      if( x_ <= -fabs(y_) )
         return atan(y_/x_)+M_PI;
      if( y_ >= fabs(x_) )
         return atan(-x_/y_)+.5*M_PI;
      // y_ <= -fabs(x_)
      return atan(-x_/y_)+1.5*M_PI;
   }

}
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/vec.cpp,v $
 * Version $Revision: 300 $
 * Date    $Date: 2008-12-11 07:14:16 +0100 (Thu, 11 Dec 2008) $
 *-----------------------------------------------------------------------
 * $Log: vec.cpp,v $
 *-----------------------------------------------------------------------*/
