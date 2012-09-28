/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_comradius.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vis
{

   DrawableComradius::
   DrawableComradius( const std::string& ename,
                     const double radius)
      : Drawable( ename ),
        radius_(radius)
   {
   }
   // ----------------------------------------------------------------------
   DrawableComradius::
   ~DrawableComradius()
   {
   }
   // ----------------------------------------------------------------------
   void
   DrawableComradius::
   init( void )
      throw()
   {
      props_ = new ComradiusPropertySet;
      props_->init(*this);
      props_->stack_position_w().add_t(
         auto_init_property(
         new PropertyConstantVecTask
         ::PropertyConstantVec(shawn::Vec(radius_,radius_))));
      Drawable::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableComradius::
   draw( cairo_t* cr, double t, const Context& C )
      const throw( std::runtime_error )
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            Drawable::draw(cr,t,C);
            double rangeline = sqrt(radius_*radius_*0.5);
            shawn::Vec position = props_->position(t);
       
            cairo_save(cr);

            cairo_set_line_width( cr, 0.07 );
            blend_set_color(cr,shawn::Vec(0.4,0.4,0.4));

            cairo_save(cr);
            cairo_translate(cr, position.x(), position.y());
				cairo_arc(cr,0.0,0.0,radius_,0,2.0*M_PI);
            cairo_stroke(cr);
            cairo_restore(cr);

            cairo_move_to (cr, position.x(), position.y());
            cairo_line_to(cr, position.x()+rangeline, position.y()+rangeline);
            cairo_stroke(cr);

            cairo_restore(cr);
         }
   }

   const PropertySet&
   DrawableComradius::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   DrawableComradius::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
}

#endif
