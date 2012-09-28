/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_NODE_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_NODE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/node.h"
#include "apps/vis/elements/vis_drawable.h"

namespace vis
{
	/**
	 * This is the base class for the graphical representation of nodes, 
	 * To create new node types, derive it from this class.
	 */
   class DrawableNode
      : public Drawable
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      DrawableNode( const std::string& prefix,
                    const shawn::Node& );
      virtual ~DrawableNode();
      ///@}

      /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

      ///@name Getters
      ///@{
      /**
       * Returns the original network node this drawable node is representing.
       */
      inline const shawn::Node& node( void ) const throw()
      { assert(node_!=NULL); return *node_; }
      /**
       * Returns the node's (real) position vector.
       */
      virtual shawn::Vec position( double ) const throw() = 0;
      ///@}

   private:
      /// Shawn node to visualize.
      const shawn::Node* node_;
   };
}

#endif
#endif
