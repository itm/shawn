/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "apps/vis/elements/vis_drawable_node_default.h"
#include "sys/taggings/basic_tags.h"

namespace vis
{
   const std::string DrawableNodeDefault::PREFIX("node.default");
   // ----------------------------------------------------------------------
   DrawableNodeDefault::
   DrawableNodeDefault( const shawn::Node& v )
      : DrawableNode( PREFIX, v ),
        props_ ( NULL )
   {
   }
   // ----------------------------------------------------------------------
   DrawableNodeDefault::
   ~DrawableNodeDefault()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableNodeDefault::
   init( void )
      throw()
   {
      props_ = new NodePropertySet;
      props_->init(*this);

      DrawableNode::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableNodeDefault::
   draw( cairo_t* cr, double t, const Context& C )
      const throw(std::runtime_error)
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            shawn::Vec pos = position(t);
            double size    = node_properties().size(t);
            
            shawn::Vec bg;

            shawn::ConstTagHandle rtag = node().find_tag( "red" );
            shawn::ConstTagHandle gtag = node().find_tag( "green" );
            shawn::ConstTagHandle btag = node().find_tag( "blue" );
            //if(rtag!=NULL && gtag!=NULL && btag!=NULL)
            //{
            //   double r = dynamic_cast<const shawn::DoubleTag*>( rtag.get() )->value();
            //   double g = dynamic_cast<const shawn::DoubleTag*>( gtag.get() )->value();
            //   double b = dynamic_cast<const shawn::DoubleTag*>( btag.get() )->value();
            //   
            //   bg = shawn::Vec(r,g,b);
            //}
            //else
            //{
               bg = node_properties().background(t);
            //}



			   int shape = node_properties().shape(t);

            cairo_save(cr);
            cairo_translate(cr,pos.x(),pos.y());
            cairo_set_line_width( cr, 0 );

			   switch(shape)
			   {
			   case 2:
				   cairo_rectangle(cr,-size,-size,size*2,size*2);
				   break;
			   default:
				   cairo_arc(cr,0.0,0.0,size,0,2.0*M_PI);
				   break;
			   }
            blend_set_color(cr,bg);
            cairo_fill(cr);

            if( C.draft_level()<2 ) {
               double lw      = node_properties().line_width(t);
               shawn::Vec fg  = node_properties().foreground(t);

               cairo_set_line_width( cr, lw );
               
               switch(shape)
			      {
				   case 2:
					   cairo_rectangle(cr,-size,-size,size*2,size*2);
				   break;
				   default:
					   cairo_arc(cr,0.0,0.0,size,0,2.0*M_PI);
				   break;
			      }
                  blend_set_color(cr,fg);
                  cairo_stroke(cr);
            }

            cairo_restore(cr);
         }
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   DrawableNodeDefault::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   DrawableNodeDefault::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   shawn::Vec
   DrawableNodeDefault::
   position( double t )
      const throw()
   { return node_properties().position(t); }

}

#endif
