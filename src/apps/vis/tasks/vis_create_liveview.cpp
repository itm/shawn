/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#ifdef HAVE_GLUT
#include "apps/vis/tasks/vis_create_liveview.h"
#include "apps/vis/events/vis_refresh_liveview.h"
#include "apps/vis/base/vis_liveview.h"
#include "sys/event_scheduler.h"
#include "sys/world.h"
#include <cairo.h>
#include <time.h>
#include <iostream>

using namespace shawn;

namespace vis
{
   CreateLiveviewTask::
   CreateLiveviewTask()
   {}
   // ----------------------------------------------------------------------
   CreateLiveviewTask::
   ~CreateLiveviewTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateLiveviewTask::
   name( void )
      const throw()
   { return "vis_create_liveview"; }
   // ----------------------------------------------------------------------
   std::string
   CreateLiveviewTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateLiveviewTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

	  // Uses the resolution of the vis camera as texture resolution:
      int texwidth = (int)visualization().camera().width(0.0);
      int texheight = (int)visualization().camera().height(0.0);

	  // Fit the window size to the configured camera resolution:
      int width = sc.environment().optional_int_param("winwidth", 
		  texwidth + 10);
      int height = sc.environment().optional_int_param("winheight", 
		  texwidth + 10);

	  // Refresh interval in Shaun time (once every simulation round by 
	  // default):
      double refresh_interval = sc.environment().optional_double_param(
		  "refresh_interval", 1.0);
	  // Minimum interval delay in milliseconds (to make a fast simulations 
	  // visible; 0 by default):
      int refresh_delay = sc.environment().optional_int_param("refresh_delay",
		  0);

#ifdef HAVE_BOOST
	  // Creates the external window:
      createWindow(width, height, texwidth, texheight);

	  // Adds the Liveview refresh event to Shauns event scheduler:
      double event_time = sc.world().scheduler().current_time();
      sc.world_w().scheduler_w().new_event(*new RefreshLiveviewEvent(
		  refresh_interval, refresh_delay, visualization_w()), 
		  event_time, NULL);
#endif
   }
}
#endif
#endif
