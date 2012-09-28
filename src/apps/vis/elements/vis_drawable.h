/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include "apps/vis/elements/vis_element.h"
#include "apps/vis/properties/vis_property_stack.h"
#include "apps/vis/base/vis_context.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
   DECLARE_HANDLES( Drawable );

   /** \brief Base class for drawable elements.
    *
	 * This is the base class for all drawable Elements. To define all-new 
    * drawables, derive it from this class. To create new types of existing
    * drawables, like nodes or edges, derive it from the corrensponding base
    * class (like DrawableNode) instead.
	 */
   class Drawable
      : public Element
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      Drawable( const std::string& );
      virtual ~Drawable();
      ///@}

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

      /**
       * Draws the element onto the given cairo surface.
       *
       * Note: Call before derived draw to have blend_set_color() working!
       */
      virtual void draw( cairo_t*, double, const Context& ) 
         const throw(std::runtime_error);

      ///@name Getter/Setter
      ///@{
      /**
       * Sets the blend color, the element is blendet to this color, e.g. to 
       * simulate a blend-out or -in.
       */
      virtual void blend_set_color( cairo_t*, const shawn::Vec& ) 
         const throw();
      /**
       * Sets the blend color, the element is blendet to this color, e.g. to 
       * simulate a blend-out or -in. Uses alpha value to define, how much to
       * blend.
       */
      virtual void blend_set_color_alpha( cairo_t*, const shawn::Vec&, 
         double alpha ) const throw();

      /**
       * Gets current priority value.
       */
      double priority( double t ) const throw();
      /**
       * Get current blend amount.
       */
      double blend( double t ) const throw();
      ///@}

      /**
       * Returns, if the element is visible in respect to the blend setting.
       * Note: Valid after call to draw().
       */
      inline bool visible(void) const throw()
      { return current_blend_<=1.0-EPSILON; }

   private:
      /// Priority stack.
      const PropertyStack<double>* stack_prio_;
      /// Blend stack.
      const PropertyStack<double>* stack_blend_;
      
      /// Current camera background color.
      mutable shawn::Vec current_cam_background_;
      /// Current blend amount.
      mutable double current_blend_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable.h,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable.h,v $
 * Revision 1.5  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/05 20:22:35  ali
 * more vis
 *
 * Revision 1.3  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/01/31 12:44:41  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/

