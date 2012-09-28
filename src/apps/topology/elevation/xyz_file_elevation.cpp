/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/xyz_file_elevation.h"
#include "apps/topology/elevation/xyz_file.h"
#include <sstream>
#include <math.h>
#include <limits>
#include <math.h>

using namespace shawn;

namespace topology
{

   XYZFileElevation::
   XYZFileElevation( XYZFile* f,
                     const shawn::Vec& base_point,
                     double xscale, double yscale, double zscale,
                     const std::string& name )
      : box_        ( Vec( base_point.x(), 
                           base_point.y(),
                           -std::numeric_limits<double>::max()),
                      Vec( base_point.x() + xscale*double(f->x_dimension()),
                           base_point.y() + yscale*double(f->y_dimension()),
                           std::numeric_limits<double>::max()) ),
        name_       ( name ),
        base_point_ ( base_point ),
        x_scale_    ( xscale ),
        y_scale_    ( yscale ),
        z_scale_    ( zscale ),
        xyz_data_   ( f )
   {}
   // ----------------------------------------------------------------------
   XYZFileElevation::
   ~XYZFileElevation()
   { 
      assert( xyz_data_ != NULL );
      delete xyz_data_;
   }
   // ----------------------------------------------------------------------
   std::string
   XYZFileElevation::
   name( void ) 
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   XYZFileElevation::
   description( void )
      const throw()
   { return std::string("elevation based on a .XYZ file"); }
   // ----------------------------------------------------------------------
   double 
   XYZFileElevation::
   value( const shawn::Vec& p )
      const throw()
   {
      double xlocal = (p.x()-base_point_.x())/x_scale_;
      double ylocal = (p.y()-base_point_.y())/y_scale_;

      int xcell = int(floor(xlocal));
      int ycell = int(floor(ylocal));

      double xfrac = xlocal-double(xcell);
      double yfrac = ylocal-double(ycell);

      if( EQDOUBLE(xfrac,0.0) && (xcell == xyz_data_->x_dimension()-1) )
         { --xcell; xfrac=1.0; }
      if( EQDOUBLE(yfrac,0.0) && (ycell == xyz_data_->y_dimension()-1) )
         { --ycell; yfrac=1.0; }

      if( (xcell<0) || (ycell<0) ||
          (xcell>=xyz_data_->x_dimension()-1) ||
          (ycell>=xyz_data_->y_dimension()-1) )
         return 0.0;

      if( xfrac > yfrac )
         {
            // lower triangle
            double a = xyz_data_->value(xcell,ycell);
            double b = xyz_data_->value(xcell+1,ycell);
            double c = xyz_data_->value(xcell+1,ycell+1);
            return  (a + xfrac*(b-a) + yfrac*(c-b))*z_scale_ + base_point_.z();
         }
      else
         {
            // upper triangle
            double a = xyz_data_->value(xcell,ycell);
            double b = xyz_data_->value(xcell,ycell+1);
            double c = xyz_data_->value(xcell+1,ycell+1);
            return  (a + yfrac*(b-a) + xfrac*(c-b))*z_scale_ + base_point_.z();
         }
   }
   // ----------------------------------------------------------------------
   shawn::Box 
   XYZFileElevation::
   domain( void )
      const throw()
   { return box_; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file_elevation.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file_elevation.cpp,v $
 *-----------------------------------------------------------------------*/
