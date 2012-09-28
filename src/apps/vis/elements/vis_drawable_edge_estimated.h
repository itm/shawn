/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_DRAWABLE_EDGE_ESTIMATED_H
#define __VIS_DRAWABLE_EDGE_ESTIMATED_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_edge.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "apps/vis/properties/vis_edge_property_set.h"
#include "apps/vis/base/vis_needs_cairo.h"

namespace vis
{
	/**
	 * This is the 'estimated' implementation of a DrawableEdge. See also 
    * EdgePropertySet on properties of edges. 
    *
    * This version of edges draws the edge from a real node position to
    * it's estimated position.
	 *
	 * @sa EdgePropertySet
	 * @sa CreateEstimatedEdgesTask
	 */
   class DrawableEdgeEstimated
      : public DrawableEdge
   {
   public:
      /// Prefix for the name of all edges of this type.
      static const std::string PREFIX;
      ///@name Contructor/Destructor
      ///@{
      DrawableEdgeEstimated( const shawn::Node&,
                             const DrawableNode&,
                             const std::string& p = PREFIX);
      virtual ~DrawableEdgeEstimated();
      ///@}

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

	   /**
       * Draws the element onto the given cairo surface.
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

   private:
      /// RefPointer to Property set.
      shawn::RefcntPointer<EdgePropertySet> props_;
      /// Source (real position) and target (estimated position) node.
      const DrawableNode* src_drawable_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_edge_default.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_edge_default.h,v $
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

