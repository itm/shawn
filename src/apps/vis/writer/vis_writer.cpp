/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_writer.h"
#include "apps/vis/base/visualization.h"
#include "sys/util/defutils.h"
#include <sstream>
#include <cmath>
#include <iomanip>
#include "cairo.h"

namespace vis
{

   Writer::
   Writer()
      : draft_( 0 ),
	    surface_        ( NULL ),
        surface_width_  ( -1 ),
        surface_height_ ( -1 ),
        next_frame_     ( -1 ),
        multi_frame_    ( false ),
        visualization_  ( NULL ),
        last_freezed_   ( false ),
		file_base_      ( "" )

   {}
   // ----------------------------------------------------------------------
   Writer::
   ~Writer()
   {}
   // ----------------------------------------------------------------------
   int
   Writer::
   draft_level( void )
      const throw()
   { return draft_; }
   // ----------------------------------------------------------------------
   void
   Writer::
   set_draft( int d )
      throw( std::runtime_error )
   { draft_ = d; }
   // ----------------------------------------------------------------------
   int
   Writer::
   next_frame_number(void)
      const throw()
   {
	   return next_frame_;
   }
   // ----------------------------------------------------------------------
   void
   Writer::
   drop_surface( void )
      throw()
   {
      cairo_surface_destroy( surface_ );
      surface_ = NULL;
   }
   // ----------------------------------------------------------------------
   void
   Writer::
   post_write( void )
      throw( std::runtime_error )
   {
      visualization_=NULL;
   }
   // ----------------------------------------------------------------------
   void
   Writer::
   pre_write( const Visualization& vis,
              const std::string& filename_base,
              bool multi_frame )
      throw( std::runtime_error )
   {
      assert( visualization_ == NULL );
      visualization_ = &vis;
      multi_frame_ = multi_frame;
	   file_base_ = filename_base;
      next_frame_=0;
   }
      // ----------------------------------------------------------------------
   void
   Writer::
   write_frame( double t )
      throw( std::runtime_error )
   {
      // GET PARAMS FROM CAM ELEMENT
      const Camera& cam = visualization_->camera();
      bool freezed = cam.cam_properties().freeze(t)>.5;

      if( (!freezed) || (!last_freezed_) )
         {
            int w = int(ceil(cam.width(t)));
            int h = int(ceil(cam.height(t)));
      
            if( (w<5) || (h<5) )
               {
                  std::ostringstream oss;
                  oss << "Picture dimension too small: "
                      << w << "x" << h;
                  throw std::runtime_error(oss.str());
               }
            
            double scale = cam.scale(t);
            if( scale < EPSILON )
               {
                  std::ostringstream oss;
                  oss << "Scale too small: " << scale;
                  throw std::runtime_error(oss.str());
               }
            
            shawn::Vec cam_pos = cam.position(t);
            shawn::Vec cam_pos_shift = cam.position_shift(t);
            
            // ALLOC SURFACE
            make_surface(w,h);
            
            
            // CLEAR
            shawn::Vec cam_bg = cam.background(t);
            cairo_t* cr = cairo_create(surface_);
            cairo_rectangle (cr, 0, 0, w, h); 
            cairo_set_source_rgb (cr,cam_bg.x(),cam_bg.y(),cam_bg.z()); 
            cairo_fill (cr);
            
            if( draft_level()>0 )
               cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE );
            
            shawn::Vec trans( (-cam_pos.x()*scale) + (double(w)/2.0) + cam_pos_shift.x(),
                              (-cam_pos.y()*scale) - (double(h)/2.0) - cam_pos_shift.y() );
            
            Context C(trans/scale,scale,w,h,draft_level());

            cairo_save(cr);
            // TRANSFORMATION
            cairo_translate( cr, trans.x(), -trans.y() );
            cairo_scale(cr,scale,-scale);
            
            //
            visualization_->draw(cr,t,C);
			   write_frame_to_file(cr);
            
            cairo_restore(cr);
      
            cairo_destroy (cr);
         }

      
      ++next_frame_;

      last_freezed_=freezed;
   }
}
#endif
