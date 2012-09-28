/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_TAG_SAMPLETASK_H
#define __VIS_TAG_SAMPLETASK_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"

namespace vis
{
   /** \brief Helper task to create sample tags on nodes.
    * 
    * Creates some tags that may be used to test Vis tagging functionality.
    */
   class TagSampleTask
      : public VisualizationTask
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      TagSampleTask();
      virtual ~TagSampleTask();
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
   };
}

#endif
#endif
