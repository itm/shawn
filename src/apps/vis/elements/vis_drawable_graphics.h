/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_GRAPHICS_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_GRAPHICS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable.h"
#include "apps/vis/properties/vis_graphics_property_set.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
	/**
	 * This class represents a drawable graphics object, an image that can be
	 * loaded from a PNG image file and rendered to the visualization output.
	 * 
	 * Use a CreateGraphicsTask to create a drawable graphics object by calling
	 * vis_create_graphics name=object_name file=filename.png.
	 *
	 * See also GraphicsPropertySet for the properties of a drawable graphics
	 * object.
	 *
	 * @sa GraphicsPropertySet
	 * @sa CreateGraphicsTask
	 */
   class DrawableGraphics
      : public Drawable
   {
   public:
	   ///@name construction/deconstruction
	   ///@{
      DrawableGraphics( const std::string& elemname,
                        const std::string& filename );
      virtual ~DrawableGraphics();
	   ///@}

	   /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

	   /**
       * Draws the image onto the given cairo surface.
       */
      virtual void draw( cairo_t*, double, const Context& ) 
         const throw(std::runtime_error);

	   ///@name Getters
      ///@{
      /**
       * Returns the property set (constant).
       */
      virtual const PropertySet& properties( void ) const throw();
      /**
       * Returns the property set.
       */
      virtual PropertySet& properties_w( void ) throw();
      /**
       * Returns the property set as GraphicsPropertySet (typed).
       */
      inline GraphicsPropertySet& graphics_properties_w( void ) throw()
      { assert( props_.is_not_null() ); return *props_; }
      /**
       * Returns the property set as GraphicsPropertySet (typed and constant).
       */
      inline const GraphicsPropertySet& graphics_properties( void ) const throw()
      { assert( props_.is_not_null() ); return *props_; }
	   ///@}
   private:
      /// RefPointer to Property set.
      shawn::RefcntPointer<GraphicsPropertySet> props_;
      /// Filename of image file.
      std::string file_;
      /// Cairo surface to draw on.
      cairo_surface_t* image_;
      /// Width of the image.
      double width_;
      /// Height of the image.
      double height_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_graphics.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_graphics.h,v $
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

