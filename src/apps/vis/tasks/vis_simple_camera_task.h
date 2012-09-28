/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_SIMPLE_CAMERA_TASK_H
#define __SHAWN_TUBSAPPS_VIS_SIMPLE_CAMERA_TASK_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"

namespace vis
{
   /** \brief Task to simplify camera configuration
    * 
    * This task configures the camera with default values, making it more simple
    * to setup Vis. You may set camera resolution before calling this task
    * to automatically set the other values like scale accordingly.
    */
   class SimpleCameraTask
      : public VisualizationTask
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      SimpleCameraTask();
      virtual ~SimpleCameraTask();
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
