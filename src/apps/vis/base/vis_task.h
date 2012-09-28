/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_TASK_H
#define __SHAWN_TUBSAPPS_VIS_TASK_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/vis/base/visualization.h"

namespace vis
{
   /** \brief Base class for visualization tasks.
    * 
    * This is the base class for all visualization tasks of Vis. To create new
    * visualization tasks, derive them from this class.
    */
   class VisualizationTask
      : public shawn::SimulationTask
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      VisualizationTask();
      virtual ~VisualizationTask();
      ///@}

      /**
       * Runs the task.
       */
      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:
      ///@name Getter
      ///@{
      /**
       * Gets the visualization object.
       */
      inline Visualization& visualization_w( void ) throw()
      { assert( vis_.is_not_null() ); return *vis_; }
      /**
       * Gets the visualization object (constant).
       */
      inline const Visualization& visualization( void ) const throw()
      { assert( vis_.is_not_null() ); return *vis_; }
      ///@}

   private:
      /// Handle of the visualization instance.
      VisualizationHandle vis_;
   };

}

#endif
#endif
