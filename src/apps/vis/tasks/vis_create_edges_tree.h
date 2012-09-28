/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_CREATE_EDGES_TREE_H
#define __VIS_CREATE_EDGES_TREE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

namespace vis
{
	/**
	 * This task creates tree-like connections between nodes. Each node must 
	 * have an integer tag named "predecessor_id". If there is one greater or
	 * equal than zero, a connection is drawn between the node and its 
	 * predecessor.
    *
    * @sa CreateEdgesTask
	 */
   class CreateEdgesTreeTask
      : public VisualizationTask
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      CreateEdgesTreeTask();
      virtual ~CreateEdgesTreeTask();
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
      /**
       * Helper method to read out the given id of the predecessor.
       */
      std::string read_predecessor( const shawn::Node& node, const std::string& taglabel ) const throw();

   };

}

#endif
#endif
