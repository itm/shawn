/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/base/vis_task.h"
#include "apps/vis/base/vis_keepers.h"
#include "apps/vis/base/visualization_keeper.h"

using namespace shawn;

namespace vis
{

   VisualizationTask::
   VisualizationTask()
   {}
   // ----------------------------------------------------------------------
   VisualizationTask::
   ~VisualizationTask()
   {}
   // ----------------------------------------------------------------------
   void
   VisualizationTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      vis_ = visualization_keeper_w(sc).
         find_w( sc.environment().optional_string_param("vis", "visualization") );
   }

}
#endif
