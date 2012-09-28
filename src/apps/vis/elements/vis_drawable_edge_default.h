/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_EDGE_DEFAULT_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_EDGE_DEFAULT_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_edge.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "apps/vis/properties/vis_edge_property_set.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
	/**
	 * This is the default implementation of a DrawableEdge. See also EdgePropertySet 
	 * on properties of edges. See CreateEdgesTask on how to create edges.
	 *
	 * @sa EdgePropertySet
	 * @sa CreateEdgesTask
	 */
   class DrawableEdgeDefault
      : public DrawableEdge
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      DrawableEdgeDefault( const shawn::Node&,
                           const shawn::Node&,
                           const DrawableNode&,
                           const DrawableNode&,
                           const std::string& p = PREFIX);
      virtual ~DrawableEdgeDefault();
      ///@}

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

	   /**
       * Draws the edge onto the given cairo surface.
       */
      virtual void draw( cairo_t*, double, const Context& ) const throw(std::runtime_error);

      ///@name Getter
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
       * Returns the property set as EdgePropertySet (typed).
       */
      inline EdgePropertySet& edge_properties_w( void ) throw()
      { assert( props_.is_not_null() ); return *props_; }
      /**
       * Returns the property set as EdgePropertySet (typed and constant).
       */
      inline const EdgePropertySet& edge_properties( void ) const throw()
      { assert( props_.is_not_null() ); return *props_; }
      ///@}

      /// Prefix used for the name of every default edge.
      static const std::string PREFIX;

   private:
      /// RefPointer to Property set.
      shawn::RefcntPointer<EdgePropertySet> props_;
      /// Source Node.
      const DrawableNode* src_drawable_;
      /// Target Node.
      const DrawableNode* tgt_drawable_;
   };
}

#endif
#endif
