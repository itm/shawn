/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_EDGE_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_EDGE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/node.h"
#include "apps/vis/elements/vis_drawable.h"

namespace vis
{

	/**
	 * This is the base class for the graphical representation of edges, 
	 * representing relations between two nodes. To create new edge types,
    * derive it from this class.
	 *
	 * Use the CreateEdgesTask to create visual edges out of the simulation
	 * environment.
	 *
	 * @sa CreateEdgesTask
	 * @sa DrawableEdgeDefault
	 * @sa EdgePropertySet
	 */
   class DrawableEdge
      : public Drawable
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      DrawableEdge( const std::string& prefix,
                    const shawn::Node&,
                    const shawn::Node& );
      virtual ~DrawableEdge();
      ///@}

       /**
       * Base initialization of this element.
       */
      virtual void init( void ) throw();

      ///@name Getters
      ///@{
      /**
       * Returns the source node in which this edge starts.
       */
      inline const shawn::Node& source_node( void ) const throw()
      { assert(src_!=NULL); return *src_; }
      /**
       * Returns the target node in which this edge ends.
       */
      inline const shawn::Node& target_node( void ) const throw()
      { assert(tgt_!=NULL); return *tgt_; }
      ///@}


   private:
      /// Source node.
      const shawn::Node* src_;
      /// Target node.
      const shawn::Node* tgt_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_edge.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_edge.h,v $
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

