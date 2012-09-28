/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_edge_estimated.h"

namespace vis
{

   const std::string DrawableEdgeEstimated::PREFIX("estimated");
   // ----------------------------------------------------------------------
   DrawableEdgeEstimated::
   DrawableEdgeEstimated( const shawn::Node& v1,
                        const DrawableNode& dv1,
                        const std::string& p )
      : DrawableEdge( std::string("edge.") + p, v1, v1 ),
        props_ ( NULL ),
        src_drawable_ ( &dv1 )
   {}
   // ----------------------------------------------------------------------
   DrawableEdgeEstimated::
   ~DrawableEdgeEstimated()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableEdgeEstimated::
   init( void )
      throw()
   {
      props_ = new EdgePropertySet;
      props_->init(*this);
      DrawableEdge::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableEdgeEstimated::
   draw( cairo_t* cr, double t, const Context& C )
      const throw(std::runtime_error)
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            shawn::Vec pos1 = src_drawable_->position(t);
            const shawn::Vec pos2 = src_drawable_->node().est_position();
            double lw       = edge_properties().line_width(t);
            shawn::Vec col  = edge_properties().color(t);
            double blend   = edge_properties().blend(t);

            cairo_save(cr);
            cairo_set_line_width( cr, lw );

            cairo_set_source_rgba(cr,col.x(),col.y(),col.z(),1.0-blend);

            //blend_set_color(cr,col);
            cairo_move_to(cr,pos1.x(),pos1.y());
            cairo_line_to(cr,pos2.x(),pos2.y());
            cairo_stroke(cr);
            cairo_restore(cr);
         }
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   DrawableEdgeEstimated::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   DrawableEdgeEstimated::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_edge_default.cpp,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2006/02/19 21:34:29 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_edge_default.cpp,v $
 * Revision 1.4  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/05 20:22:35  ali
 * more vis
 *
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
