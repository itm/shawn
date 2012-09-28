/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/math/localization_general_math.h"
#include <cmath>
#include <limits>


namespace localization
{

   double
   collinear_measure( double a, double b, double c, double error )
      throw()
   {
      if ( a <= c && b <= c )
      {
         return a + b - ( 1 + error ) * c;
      }
      else if ( a <= b && c <= b )
      {
         return a + c - ( 1 + error ) * b;
      }
      else if ( b <= a && c <= a )
      {
         return b + c - ( 1 + error ) * a;
      }

      return std::numeric_limits<double>::min();
   }
   // ----------------------------------------------------------------------
   bool
   is_collinear( double a, double b, double c, double error )
      throw()
   {
      return collinear_measure( a, b, c, error ) <= 0;
   }
   // ----------------------------------------------------------------------
   void
   rotate_2D( double angle, shawn::Vec& coord, const shawn::Vec& origin )
      throw()
   {
      double cos_angle = cos( angle );
      double sin_angle = sin( angle );

      coord = shawn::Vec(
         ((coord.x() - origin.x()) * cos_angle) - ((coord.y() - origin.y()) * sin_angle) + origin.x(),
         ((coord.y() - origin.y()) * cos_angle) + ((coord.x() - origin.x()) * sin_angle) + origin.y(),
         coord.z() );
   }
   // ----------------------------------------------------------------------
   double
   angle_vec( const shawn::Vec& vec )
      throw()
   {
      if ( vec.x() == 0 && vec.y() >= 0 )
         return 0.5 * M_PI;
      else if ( vec.x() == 0 && vec.y() < 0 )
         return 1.5 * M_PI;

      if ( vec.y() == 0 && vec.x() >= 0 )
         return 0;
      else if ( vec.y() == 0 && vec.x() < 0 )
         return M_PI;

      if ( vec.y() >= 0 )
         return acos( vec.x() / vec.euclidean_norm() );
      else
         return 2.0*M_PI - acos( vec.x() / vec.euclidean_norm() );
   }
   // ----------------------------------------------------------------------
   double
   normalize_angle( double angle )
      throw()
   {
      while ( angle < 0 ) angle += 2 * M_PI;
      while ( angle >= (2 * M_PI) ) angle -= (2 * M_PI);

      return angle;
   }
}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_general_math.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_general_math.cpp,v $
 *-----------------------------------------------------------------------*/
