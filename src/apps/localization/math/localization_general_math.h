/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MATH_GENERAL_MATH_H
#define __SHAWN_APPS_LOCALIZATION_MATH_GENERAL_MATH_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/util/localization_defutils.h"
#include "sys/vec.h"

namespace localization
{

   /** To decide, whether a triangle is collinear or not, the sum of the two
    *  smallest sides minus the largest side is formed. To take account
    *  ranging errors, additionally the largest side is multiplied by
    *  1 plus a threshold, which is given as the fourth parameter.
    *
    *  Resulting formula looks like following:
    *  \f[ a + b - ( 1 + threshold ) * c \f]
    *
    *  The greater the resulting measure is, the fewer collinear the
    *  triangle is.
    *
    * \param double length of side of a triangle
    * \param double length of side of a triangle
    * \param double length of side of a triangle
    * \param double above mentioned threshold
    * \return Measure of collinearity of the three given distances
    *  with respect to the given error
    * \sa is_collinear()
    */
   double collinear_measure( double, double, double, double = 0 ) throw();
   /** This methode forms the collinear_measure() and decides, whether
    *  the triangle is collinear or not.
    *
    * \param double length of side of a triangle
    * \param double length of side of a triangle
    * \param double length of side of a triangle
    * \param double above mentioned threshold
    * \return \c true, if collinear_measure() is less or equal 0.
    * \c false otherwise.
    * \sa collinear_measure()
    */
   bool is_collinear ( double, double, double, double = 0 ) throw ();
   /** This method rotates a given coordinate by a given radius.
    *
    *  \param double angle
    *  \param shawn::Vec& coordinate to be rotated.
    *  \param shawn::Vec& coordinate to rotate around (std: (0,0,0)).
    */
   void rotate_2D( double, shawn::Vec&, const shawn::Vec& = shawn::Vec( 0.0, 0.0, 0.0 ) ) throw();
   /** This method computes the angle of a vector in respect to the x-axis.
    *
    *  \param shawn::Vec& Vector
    *  \result angle
    */
   double angle_vec( const shawn::Vec& ) throw();
   /** This method sets the angle between 0 and 2*PI.
    *
    *  \param doublke angle
    *  \result angle
    */
   double normalize_angle( double ) throw();
 
}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_general_math.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_general_math.h,v $
 *-----------------------------------------------------------------------*/
