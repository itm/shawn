/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_external_animation.h"
#include "apps/vis/writer/vis_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include "apps/vis/events/vis_write_animation.h"
#include "sys/event_scheduler.h"
#include "sys/world.h"
#include <iomanip>
#include <sstream>
#include <math.h>

#define VIS_HAVE_ETA

#ifdef VIS_HAVE_ETA
#include <ctime>
#endif


namespace vis
{

   ExternalAnimationTask::
   ExternalAnimationTask()
   {}
   // ----------------------------------------------------------------------
   ExternalAnimationTask::
   ~ExternalAnimationTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   ExternalAnimationTask::
   name( void )
      const throw()
   { return "vis_external_animation"; }
   // ----------------------------------------------------------------------
   std::string
   ExternalAnimationTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   ExternalAnimationTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      std::string file_name = sc.environment().optional_string_param(
         "filename", "animation");
      double refresh_interval = sc.environment().optional_double_param(
		  "refresh_interval", 1.0);
      std::string writer_type = sc.environment().optional_string_param("writer", "png");
      double event_time = sc.world().scheduler().current_time();

      sc.world_w().scheduler_w().new_event(*new WriteAnimationFrameEvent(
		  refresh_interval, visualization_w(), writer_type, file_name, sc ), 
		  event_time, NULL);
   }


}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/tasks/vis_external_animation.cpp,v $
 * Version $Revision: 1.8 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_external_animation.cpp,v $
 * Revision 1.8  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.7  2006/02/26 12:00:59  ali
 * *** empty log message ***
 *
 * Revision 1.6  2006/02/25 15:21:41  ali
 * *** empty log message ***
 *
 * Revision 1.5  2006/02/22 22:27:24  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/22 09:18:57  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
