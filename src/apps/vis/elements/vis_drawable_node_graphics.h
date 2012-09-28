/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_NODE_GRAPHICS_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_NODE_GRAPHICS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_node.h"
#include "apps/vis/properties/vis_node_property_set.h"
#include "apps/vis/properties/vis_graphics_property_set.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
   /** \brief Implementation of drawable nodes creating images.
    *
    * This class represents an implementation of the graphical
    * representation of a shawn::Node as images. For DrawableNodeGraphics instances
    * being created
    * automatically, when the vis_create_task is called.
    *
    * @sa vis::VisualizationTaskCreate
    * @sa vis::DrawableNode
    */
   class DrawableNodeGraphics
      : public DrawableNode
   {
   public:
      /// Name prefix for elements of this type.
      static const std::string PREFIX;

      ///@name Contructor/Destructor
      ///@{
      DrawableNodeGraphics( const shawn::Node&, const std::string& );
      virtual ~DrawableNodeGraphics();
      ///@}

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

      /**
       * Draws the node onto a given Cairo surface.
       */
      virtual void draw( cairo_t*, double, const Context& ) const throw(std::runtime_error);

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
       * Returns the property set as NodePropertySet (typed).
       */
      inline NodePropertySet& node_properties_w( void ) throw()
      { assert( props_.is_not_null() ); return *props_; }
      /**
       * Returns the property set as NodePropertySet (typed and constant).
       */
      inline const NodePropertySet& node_properties( void ) const throw()
      { assert( props_.is_not_null() ); return *props_; }
      /**
       * Returns the node's (real) position.
       */
      virtual shawn::Vec position( double ) const throw();
      ///@}

   private:
      /// RefPointer to the property set.
      shawn::RefcntPointer<NodePropertySet> props_;
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
