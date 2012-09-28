/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable.h"
#include "apps/vis/base/visualization.h"
#include "apps/vis/properties/vis_property_names.h"

namespace vis
{

   Drawable::
   Drawable( const std::string& s )
      : Element(s)
   {}
   // ----------------------------------------------------------------------
   Drawable::
   ~Drawable()
   {}
   // ----------------------------------------------------------------------
   void
   Drawable::
   init( void )
      throw()
   {
      Element::init();
      ConstPropertyStackHandle cps = properties().property_stack( PROP_DBL_PRIORITY );
      assert( cps.is_not_null() ); // if this assert fails:
      // (1) your Drawable-derived class calls Drawable::init() at the beginning
      //     of its init() -- it should do that IN THE END!
      // (2) your Drawable-derived class does not provide a priority property.
      //     this is infeasible. provide one.
      stack_prio_=dynamic_cast<const vis::PropertyStack<double>*>(cps.get());
      assert( stack_prio_ != NULL );

      cps = properties().property_stack( "blend" );
      assert( cps.is_not_null() ); // if this assert fails:
      // (1) your Drawable-derived class does not provide a blend property.
      //     this is infeasible. provide one.
      stack_blend_=dynamic_cast<const vis::PropertyStack<double>*>(cps.get());
      assert( stack_blend_ != NULL );
   }
   // ----------------------------------------------------------------------
   double
   Drawable::
   priority( double t )
      const throw()
   {
      assert( stack_prio_ != NULL );
      return stack_prio_->value(t,*this);
   }
   // ----------------------------------------------------------------------
   double
   Drawable::
   blend( double t )
      const throw()
   {
      assert( stack_blend_ != NULL );
      return stack_blend_->value(t,*this);
   }
   // ----------------------------------------------------------------------
   void
   Drawable::
   draw( cairo_t* cr,
         double t,
         const Context& )
      const throw(std::runtime_error)
   {
      current_blend_=blend(t);
      current_cam_background_ = visualization().camera().background(t);
   }
   // ----------------------------------------------------------------------
   void
   Drawable::
   blend_set_color( cairo_t* cr,
                    const shawn::Vec& col )
      const throw()
   {
      if( current_blend_ < EPSILON )
         cairo_set_source_rgb(cr,col.x(),col.y(),col.z());
      else {
         cairo_set_source_rgb(cr,
                              col.x()*(1.0-current_blend_) + 
                              current_cam_background_.x()*current_blend_,
                              col.y()*(1.0-current_blend_) + 
                              current_cam_background_.y()*current_blend_,
                              col.z()*(1.0-current_blend_) + 
                              current_cam_background_.z()*current_blend_ );
      }
   }
   // ----------------------------------------------------------------------
   void
   Drawable::
   blend_set_color_alpha( cairo_t* cr,
                          const shawn::Vec& col,
                          double alpha )
      const throw()
   {
      if( current_blend_ < EPSILON )
         cairo_set_source_rgba(cr,col.x(),col.y(),col.z(),alpha);
      else {
         cairo_set_source_rgba(cr,
                               col.x()*(1.0-current_blend_) + 
                               current_cam_background_.x()*current_blend_,
                               col.y()*(1.0-current_blend_) + 
                               current_cam_background_.y()*current_blend_,
                               col.z()*(1.0-current_blend_) + 
                               current_cam_background_.z()*current_blend_,
                               alpha );
      }
   }



}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable.cpp,v $
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
