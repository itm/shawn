/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_ELEVATION_H
#define __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_ELEVATION_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/reading/readings/simple_reading.h"
#include "sys/misc/box.h"

#include <string>


namespace topology
{
   class XYZFile;

   class XYZFileElevation
      : public reading::DoubleReading
   {
   public:
      XYZFileElevation( XYZFile*,
                        const shawn::Vec& base_point,
                        double xscale, double yscale, double zscale,
                        const std::string& name );
      virtual ~XYZFileElevation();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual double value( const shawn::Vec& ) const throw();
      virtual shawn::Box domain( void ) const throw();

   private:
      shawn::Box  box_;
      std::string name_;
      shawn::Vec  base_point_;
      double      x_scale_;
      double      y_scale_;
      double      z_scale_;
      XYZFile*    xyz_data_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file_elevation.h,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file_elevation.h,v $
 *-----------------------------------------------------------------------*/
