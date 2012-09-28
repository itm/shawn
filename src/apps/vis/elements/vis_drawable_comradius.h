/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_DRAWABLE_COMRADIUS_H
#define __SHAWN_VIS_DRAWABLE_COMRADIUS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable.h"
#include "apps/vis/properties/vis_comradius_property_set.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
	
   class DrawableComradius
      : public Drawable
   {
   public:
	   ///@name construction/deconstruction
	   ///@{
      DrawableComradius( const std::string& elemname,
                        const double radius);
      virtual ~DrawableComradius();
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

      /**
       * Returns the property set (constant).
       */
      virtual const PropertySet& properties( void ) const throw();
      /**
       * Returns the property set.
       */
      virtual PropertySet& properties_w( void ) throw();

   private:
      /// Cairo surface to draw on.
      cairo_surface_t* image_;
      /// Comradius
      double radius_;
      shawn::RefcntPointer<ComradiusPropertySet> props_;
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

