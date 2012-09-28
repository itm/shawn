/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_label.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/properties/vec/vis_property_real_node_position.h"
#include "apps/vis/properties/vis_node_property_set.h"
#include "sys/taggings/basic_tags.h"

#include <string>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vis
{

   DrawableLabel::
   DrawableLabel( const std::string& lname,
                     const DrawableNode *node )
      : Drawable( lname ),
        node_ (node),
        tagname_("")
   {
   }
   // ----------------------------------------------------------------------
   DrawableLabel::
   ~DrawableLabel()
   {
   }
   // ----------------------------------------------------------------------
   void
   DrawableLabel::
   init( void )
      throw()
   {
      props_ = new ComradiusPropertySet;
      props_->init(*this);
      props_->stack_position_w().add_t(
         auto_init_property(
         new PropertyRealNodePositionTask
         ::PropertyRealNodePosition(&node_->node())));
      Drawable::init();
   }
   // ----------------------------------------------------------------------
   void
   DrawableLabel::
   draw( cairo_t* cr, double t, const Context& C )
      const throw( std::runtime_error )
   {
      Drawable::draw(cr,t,C);
      if( visible() )
         {
            Drawable::draw(cr,t,C);
            shawn::Vec position = props_->position(t);
            const NodePropertySet *nps = dynamic_cast<const NodePropertySet*>(&node_->properties());
            double size = nps->size(t);
            cairo_save(cr);

            std::string label_text = node_->name();;
            if(tagname_!="")
            {
				  label_text = "";

               const shawn::StringTag *stag = NULL;

               shawn::ConstTagHandle taghandle = node_->node().find_tag(tagname_);
               if(taghandle != NULL)
               {
                  stag = dynamic_cast<const shawn::StringTag*>(taghandle.get());
               }

               if(stag!=NULL)
               {
                  label_text = stag->value();
                  //std::cout << label_text << std::endl;
               }
            }

            cairo_text_extents_t te;

            cairo_select_font_face (cr, "serif",
                CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size (cr, 0.25);
            
            cairo_text_extents (cr, label_text.c_str(), &te);
            cairo_rectangle(cr, position.x() - te.width/2.0, position.y() - te.height*2,
               te.width, te.height);
            cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
            cairo_fill (cr);

            cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
            cairo_move_to (cr, position.x() - te.width/2.0, position.y() - te.height*2);
            cairo_scale(cr,1.0,-1.0);
            cairo_show_text (cr, label_text.c_str());

            cairo_restore(cr);
         }
   }

   const PropertySet&
   DrawableLabel::
   properties( void )
      const throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet&
   DrawableLabel::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }

   // ----------------------------------------------------------------------
   void DrawableLabel::use_tag(std::string tagname)
   {
      tagname_.assign(tagname);
   }
}

#endif
