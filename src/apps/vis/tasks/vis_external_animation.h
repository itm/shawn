/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_TASK_EXTERNAL_ANIMATION_H
#define __SHAWN_TUBSAPPS_VIS_TASK_EXTERNAL_ANIMATION_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"

namespace vis
{
   /**
    * Creates a new animation. Use 'start', 'end' and 'fps' parameters to define
    * how many frames to draw. Use 'file_base' to set the filename of the output
    * file and the 'writer' parameter to define which writer to use (which type
    * of document to create).
    */
   class ExternalAnimationTask
      : public VisualizationTask
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      ExternalAnimationTask();
      virtual ~ExternalAnimationTask();
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

