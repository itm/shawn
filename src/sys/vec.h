/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_VEC_H
#define __SHAWN_SYS_VEC_H

#include "sys/util/defutils.h"
#include "shawn_config.h"
#include <iostream>


namespace shawn
{

   class Vec
   {
   public:
      inline Vec( const Vec& p )
         : x_(p.x()), y_(p.y()), z_(p.z())
      {}
      inline Vec( double x=0.0, double y=0.0, double z=0.0 )
         : x_(x), y_(y), z_(z)
      {}

      virtual ~Vec();

      inline double x( void ) 
         const throw()
      { return x_; }

      inline double y( void )
         const throw()
      { return y_; }

      inline double z( void )
         const throw()
      { return z_; }

      inline Vec operator - ( const Vec& p ) const throw()
      { return Vec( x()-p.x(), y()-p.y(), z()-p.z() ); }
      inline Vec operator + ( const Vec& p ) const throw()
      { return Vec( x()+p.x(), y()+p.y(), z()+p.z() ); }

      inline Vec& operator -= ( const Vec& p ) throw()
      { return *this = (*this-p); }
      inline Vec& operator += ( const Vec& p ) throw()
      { return *this = (*this+p); }

      inline Vec operator * ( double f ) const throw()
      { return Vec( f*x(), f*y(), f*z() ); }
      inline Vec operator / ( double f ) const throw()
      { return Vec( x()/f, y()/f, z()/f ); }

      inline Vec& operator *= ( double f ) throw()
      { return *this = (*this*f); }
      inline Vec& operator /= ( double f ) throw()
      { return *this = (*this/f); }

      inline bool operator == ( const Vec& p ) const throw()
      { return (*this - p).euclidean_norm()<=EPSILON; }

      inline bool operator != ( const Vec& p ) const throw()
      { return !(*this == p); }

      inline Vec& operator= ( const Vec& p ) throw()
      { x_=p.x(); y_=p.y(); z_=p.z(); return *this; }

	  inline double operator * ( const Vec& p ) const throw()
      { return x()*p.x() + y()*p.y() + z()*p.z() ; }
	  
      double euclidean_norm( void )
         const throw();

      double xy_direction( void ) const throw();

      friend std::ostream& operator << ( std::ostream&, const Vec& )
         throw();

   private:
      double x_, y_, z_;
   };

   inline double
   euclidean_distance( const Vec& p1,
                       const Vec& p2 )
      throw()
   { return (p1-p2).euclidean_norm(); }

   inline Vec
   cross_product( const Vec& p1,
                       const Vec& p2 )
      throw()
   { return Vec( p1.y()*p2.z() - p1.z()*p2.y(), p1.z()*p2.x() - p1.x()*p2.z(), p1.x()*p2.y() - p1.y()*p2.x() ); }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/vec.h,v $
 * Version $Revision: 359 $
 * Date    $Date: 2009-08-05 14:06:18 +0200 (Wed, 05 Aug 2009) $
 *-----------------------------------------------------------------------
 * $Log: vec.h,v $
 *-----------------------------------------------------------------------*/
