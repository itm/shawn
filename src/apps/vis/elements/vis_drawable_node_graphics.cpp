/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_node_graphics.h"
#include "sys/taggings/basic_tags.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace vis
{
   const std::string DrawableNodeGraphics::PREFIX("node.graphics");
   // ----------------------------------------------------------------------
   DrawableNodeGraphics::
   DrawableNodeGraphics( const shawn::Node& v,
                         const std::string& fname = "" )
      : DrawableNode( PREFIX, v ),
        props_ ( NULL )
   {
      if ( fname != "" )
      {
         image_ = cairo_image_surface_create_from_png (fname.c_str());
         if( cairo_surface_status(image_) != CAIRO_STATUS_SUCCESS )
            throw std::runtime_error(fname + " problem");

         width_ = cairo_image_surface_get_width (image_);
         height_ = cairo_image_surface_get_height (image_);
      }
   }
   // ----------------------------------------------------------------------
   DrawableNodeGraphics::
   ~DrawableNodeGraphics()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableNodeGraphics::
   init( void )
      throw()
   {
      props_ = new NodePropertySet;
      props_->init(*this);

      DrawableNode::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableNodeGraphics::
   draw( cairo_t* cr, double t, const Context& C )
      const throw(std::runtime_error)
   {
      Drawable::draw(cr,t,C);
      if( visible() )
      {
         cairo_surface_t* image;
         double width;
         double height;

         std::string taglabel = node().world().simulation_controller().
            environment().optional_string_param("node_graphics_fname", "");
         if ( taglabel == "" )
         {
            image = image_;
            width = width_;
            height = height_;
         }
         else
         {
            const shawn::StringTag *tag = 0;
            shawn::ConstTagHandle htag = node().find_tag(taglabel);
            if ( htag != 0 )
            {
               tag = dynamic_cast<const shawn::StringTag*>(htag.get());
            }
            else
            {
               std::cerr << "DrawableNodeGraphics: label '" << taglabel << "' not found." << std::endl;
               return;
//                abort();
            }

            if ( tag )
            {
               std::string fname = tag->value();
               image = cairo_image_surface_create_from_png(fname.c_str());
               if( cairo_surface_status(image) != CAIRO_STATUS_SUCCESS )
                  throw std::runtime_error(fname + " problem");

               width = cairo_image_surface_get_width (image);
               height = cairo_image_surface_get_height (image);
            }
            else
            {
               std::cerr << "DrawableNodeGraphics: label '" << taglabel << "' not a StringTag" << std::endl;
               abort();
            }
         }

         shawn::Vec pos =    node_properties().position(t);
         shawn::Vec anchor = shawn::Vec(0,0);
         double scale =      node_properties().size(t);
         double blend =      node_properties().blend(t);
         cairo_save(cr);

         cairo_translate(cr,pos.x(),pos.y());
         cairo_translate(cr,
                           -C.translation().x()
                           +(pos.x()-(scale*width*anchor.x()))/C.scale(),

                           -C.translation().y()
                           +(pos.y()/*-C.image_height()*/+(scale*height*(1.0-anchor.y())))/C.scale());
         cairo_scale(cr,scale/C.scale(),-scale/C.scale());
         cairo_set_source_surface (cr, image,0,0);
         if( (C.draft_level()>=3) || ( blend < EPSILON ) )
            cairo_paint(cr);
         else
            cairo_paint_with_alpha(cr,1.0-blend);
         cairo_restore(cr);
      }
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   DrawableNodeGraphics::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   DrawableNodeGraphics::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   shawn::Vec
   DrawableNodeGraphics::
   position( double t )
      const throw()
   { return node_properties().position(t); }

}

#endif
