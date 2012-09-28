/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_TASK_OPEN_LIVEVIEW_H
#define __SHAWN_VIS_TASK_OPEN_LIVEVIEW_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#ifdef HAVE_GLUT
#include "apps/vis/base/vis_task.h"

namespace vis
{
   /** \brief Creates external Liveview window.
    * Creates the external Liveview output window. This window is used to
	* visualize the network state directly, while the simulation is
	* executed. It runs in an external thread in parallel to the shaun
	* simulation thread.
    */
   class CreateLiveviewTask
      : public VisualizationTask
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      CreateLiveviewTask();
      virtual ~CreateLiveviewTask();
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
#endif
