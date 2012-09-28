/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_CREATE_DYNAMIC_EDGES_TX_RANGE_H
#define __SHAWN_TUBSAPPS_VIS_CREATE_DYNAMIC_EDGES_TX_RANGE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/elements/vis_drawable_node.h"

namespace vis
{
	/**
	 * This simulation task creates drawable edges, representing relationships between nodes by
	 * connecting these nodes graphically.
	 *
	 * Use regex to define, which node relations should be
	 * used to create edges, by calling "vis_create_edges source=regex target=regex", where
	 * source is a regex about all node names that should act as an edge's source, and target is
	 * a regex about all node names that should act as (possible) targets.
	 *
	 * To draw all edges, call for example "vis_create_edges source_regex=.* target_regex=.*".
	 *
	 * @sa DrawableEdge
	 */
   class CreateDynamicEdgesTransmissionRangeTask
      : public VisualizationTask
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      CreateDynamicEdgesTransmissionRangeTask();
      virtual ~CreateDynamicEdgesTransmissionRangeTask();
      ///@}

      ///@name Getter
      ///@{
      /**
       * The name of the task.
       */
      virtual std::string name( void ) const throw();
      /**
       * A short description of the task.
       */
      virtual std::string description( void ) const throw();
      ///@}

      /**
       * Runs the task. This ist the task main method.
       */
      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:
      /**
       * Returns the group, this task ist attached to. Throws an error,
       * if it is in no group.
       */
      virtual GroupElement* group( shawn::SimulationController& sc )
         throw( std::runtime_error );
      /**
       * Helper method to get a DrawableNode object by a shawn::Node and a
       * prefix.
       */
      virtual const DrawableNode* drawable_node( const shawn::Node&,
                                           const std::string& nprefix )
                  throw( std::runtime_error );

   };

}

#endif
#endif
