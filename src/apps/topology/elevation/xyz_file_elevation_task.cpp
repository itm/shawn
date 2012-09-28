/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/elevation/xyz_file_elevation_task.h"
#include "apps/topology/elevation/xyz_file_elevation.h"
#include "apps/topology/elevation/xyz_file.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"
#include "apps/topology/topology_keepers.h"

#include <sstream>

using namespace shawn;
using namespace std;
using namespace reading;

namespace topology
{
   const std::string XYZFileElevationTask::KEY_SCALE ( "scale" );
   const std::string XYZFileElevationTask::KEY_BOX ( "box" );
   // ----------------------------------------------------------------------
   XYZFileElevationTask::
   XYZFileElevationTask()
   {}
   // ----------------------------------------------------------------------
   XYZFileElevationTask::
   ~XYZFileElevationTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   XYZFileElevationTask::
   name( void )
      const throw()
   { return "xyz_file_elevation"; }
   // ----------------------------------------------------------------------
   std::string
   XYZFileElevationTask::
   description( void )
      const throw()
   {
      return
         string("Creates an elevation with name $name, based on .xyz file $file. If $xy_placement=") +
         KEY_SCALE +
         string(" (default), it will be scaled to a grid size of $x_scale x $y_scale (or $xy_scale x $xy_scale) with origin at ($x1,$y1). If $xy_placement=") +
         KEY_BOX +
         string(", it will be scaled to fit into the rectangle [$x1,$x2] x [$y1,$y2]. If $z_placement=") +
         KEY_SCALE +
         string(" (default), Z is scaled by $z_scale and shifted $z_base. If $z_placement=") +
         KEY_BOX +
         string(", Z is scaled such that the smallest value becomes $z1 and the largest becomes $z2.");
   }
   // ----------------------------------------------------------------------
   void
   XYZFileElevationTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string fn       = sc.environment().required_string_param("file");
      std::string outn     = sc.environment().required_string_param("name");
      std::string xyplacen = sc.environment().optional_string_param("xy_placement",KEY_SCALE);
      std::string zplacen  = sc.environment().optional_string_param("z_placement",KEY_SCALE);
      bool xyplace_box;
      bool zplace_box;

      if( xyplacen == KEY_BOX )      xyplace_box = true;
      else if( xyplacen==KEY_SCALE ) xyplace_box = false;
      else throw runtime_error(string("$xy_placement must be ")+KEY_SCALE+string(" or ")+KEY_BOX);

      if( zplacen == KEY_BOX )      zplace_box = true;
      else if( zplacen==KEY_SCALE ) zplace_box = false;
      else throw runtime_error(string("$z_placement must be ")+KEY_SCALE+string(" or ")+KEY_BOX);

      XYZFile* data = new XYZFile;
      Vec base_point;
      Vec scale_vec;
      XYZFileElevation* elev;
      try {
         data->read(fn);

         if( (data->x_dimension()<2) || (data->y_dimension()<2) )
            {
               ostringstream oss;
               oss << "XYZ data must have at least 2x2 grid points, but "
                   << fn << " just contains "
                   << data->x_dimension() << "x" << data->y_dimension();
               throw runtime_error(oss.str());
            }

         if( xyplace_box ) xy_place_box(sc,*data,base_point,scale_vec);
         else xy_place_scale(sc,*data,base_point,scale_vec);

         if( zplace_box ) z_place_box(sc,*data,base_point,scale_vec);
         else z_place_scale(sc,*data,base_point,scale_vec);

         elev = new XYZFileElevation(data,
                                     base_point,
                                     scale_vec.x(),scale_vec.y(),scale_vec.z(),
                                     outn);
      }
      catch( runtime_error& )
	  {
         delete data;
         throw;
      }

      elevation_keeper_w(sc).add( elev );
   }
   // ----------------------------------------------------------------------
   void
   XYZFileElevationTask::
   xy_place_box( shawn::SimulationController& sc,
                 const XYZFile& f,
                 shawn::Vec& bp, shawn::Vec& s )
      throw( std::runtime_error )
   {
      abort();
   }
   // ----------------------------------------------------------------------
   void
   XYZFileElevationTask::
   xy_place_scale( shawn::SimulationController& sc,
                 const XYZFile& f,
                 shawn::Vec& bp, shawn::Vec& s )
      throw( std::runtime_error )
   {
      double scale = sc.environment().optional_double_param("xy_scale",1.0);

      bp = Vec( sc.environment().optional_double_param("x1",0.0),
                sc.environment().optional_double_param("y1",0.0),
                bp.z() );
      s = Vec( sc.environment().optional_double_param("x_scale",scale),
               sc.environment().optional_double_param("y_scale",scale),
               s.z() );
   }
   // ----------------------------------------------------------------------
   void
   XYZFileElevationTask::
   z_place_box( shawn::SimulationController& sc,
                 const XYZFile& f,
                 shawn::Vec& bp, shawn::Vec& s )
      throw( std::runtime_error )
   {
      abort();
   }
   // ----------------------------------------------------------------------
   void
   XYZFileElevationTask::
   z_place_scale( shawn::SimulationController& sc,
                 const XYZFile& f,
                 shawn::Vec& bp, shawn::Vec& s )
      throw( std::runtime_error )
   {
      bp = Vec( bp.x(), bp.y(),
                sc.environment().optional_double_param("z_base",0.0) );
      s = Vec( s.x(), s.y(),
               sc.environment().optional_double_param("z_scale",1.0) );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file_elevation_task.cpp,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file_elevation_task.cpp,v $
 *-----------------------------------------------------------------------*/
