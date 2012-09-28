/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_TASK_CREATE_H
#define __SHAWN_TUBSAPPS_VIS_TASK_CREATE_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace vis
{
   /** \brief Visualization creation task
    * 
    * Creates a new visualization object. Use the 'vis' parameter to set the
    * name of the visualization. Use the optional 'drawable_node' parameter
    * to define which type of nodes to create.
    */
   class VisualizationTaskCreate
      : public shawn::SimulationTask
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      VisualizationTaskCreate();
      virtual ~VisualizationTaskCreate();
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
