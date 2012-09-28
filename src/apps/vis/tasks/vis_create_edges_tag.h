/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_CREATE_EDGES_TAG_H
#define __VIS_CREATE_EDGES_TAG_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/taggings/basic_tags.h"

namespace vis
{
	/**
	 * This task created edges based on a string tag. The tag can be chosen by the
    * 'tag' parameter, the regular expressions 'source_regex' and 'target_regex'
    * are used to verify the tag's value.
    *
    * @sa CreateEdgesTask
	 */
   class CreateEdgesTagTask
      : public VisualizationTask
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      CreateEdgesTagTask();
      virtual ~CreateEdgesTagTask();
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
       * Helper method to read out the given tag's value.
       */
      virtual const shawn::StringTag* get_stringtag(const shawn::Node *node,
                                                      std::string taglabel);

   };

}

#endif
#endif
