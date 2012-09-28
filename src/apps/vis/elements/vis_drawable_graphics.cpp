/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_graphics.h"
#include <string>

namespace vis
{

   DrawableGraphics::
   DrawableGraphics( const std::string& ename,
                     const std::string& fname )
      : Drawable( ename ),
        props_ ( NULL ),
        file_ ( fname ),
        image_ ( NULL )
   {
      image_ = cairo_image_surface_create_from_png (fname.c_str());
      if( cairo_surface_status(image_) != CAIRO_STATUS_SUCCESS )
         throw std::runtime_error(fname + " problem");

      width_ = cairo_image_surface_get_width (image_);
      height_ = cairo_image_surface_get_height (image_);
   }
   // ----------------------------------------------------------------------
   DrawableGraphics::
   ~DrawableGraphics()
   {
      if( image_!=NULL )
         cairo_surface_destroy (image_);
   }
   // ----------------------------------------------------------------------
   void
   DrawableGraphics::
   init( void )
      throw()
   {
      props_ = new GraphicsPropertySet;
      props_->init(*this);
      Drawable::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableGraphics::
   draw( cairo_t* cr, double t, const Context& C )
      const throw( std::runtime_error )
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            shawn::Vec pos =    graphics_properties().position(t);
            shawn::Vec anchor = graphics_properties().anchor(t);
            double scale =      graphics_properties().scale(t);
            double blend =      graphics_properties().blend(t);
            cairo_save(cr);
            cairo_translate(cr,
                            -C.translation().x()
                            +(pos.x()-(scale*width_*anchor.x()))/C.scale(),

                            -C.translation().y()
                            +(pos.y()/*-C.image_height()*/+(scale*height_*(1.0-anchor.y())))/C.scale());
            cairo_scale(cr,scale/C.scale(),-scale/C.scale());
            cairo_set_source_surface (cr, image_,0,0);
            if( (C.draft_level()>=3) || ( blend < EPSILON ) )
               cairo_paint(cr);
            else
               cairo_paint_with_alpha(cr,1.0-blend);
            cairo_restore(cr);

//             cairo_save(cr);
//             cairo_matrix_t   matrix;
//             cairo_pattern_t *pattern = cairo_pattern_create_for_surface (image_);
//             cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);
//             if( cairo_pattern_status(pattern) != CAIRO_STATUS_SUCCESS )
//                abort();
// //             cairo_translate (cr, 0.5, 0.5);
// //             cairo_scale (cr, 1 / sqrt (2), 1 / sqrt (2));
// //             cairo_translate (cr, - 0.5, - 0.5);
            
//             cairo_pattern_set_filter(pattern,CAIRO_FILTER_BEST);
//             cairo_translate(cr,

//                             -C.translation().x()
//                             +(pos.x()-(scale*width_*anchor.x()))/C.scale(),

//                             +C.translation().y()
//                             +(pos.y()-C.image_height()+(scale*height_*(1.0-anchor.y())))/C.scale());

//             cairo_scale(cr,scale/C.scale(),-scale/C.scale());
//             cairo_matrix_init_scale (&matrix, width_,height_);
//             //cairo_pattern_set_matrix (pattern, &matrix);

//             cairo_set_source (cr, pattern);
//             //cairo_set_source_rgb(cr,1.0,0.0,1.0);
//             cairo_rectangle (cr, 0, 0, width_,height_);//1.0, 1.0);
//             cairo_fill (cr);

//             cairo_pattern_destroy (pattern);
// // cairo_surface_destroy (image);
// //            cairo_set_source_surface (cr, image_,0,0);
// //            cairo_paint (cr);
//             cairo_restore(cr);

         }
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   DrawableGraphics::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   DrawableGraphics::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_graphics.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_graphics.cpp,v $
 * Revision 1.5  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/22 09:18:57  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/16 07:50:00  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
