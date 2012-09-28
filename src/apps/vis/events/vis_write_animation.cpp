/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#include "apps/vis/events/vis_write_animation.h"
#include "apps/vis/writer/vis_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include <cairo.h>
#include <time.h>
#include <sstream>

using namespace shawn;

namespace vis
{
   WriteAnimationFrameEvent::
   WriteAnimationFrameEvent(double refresh_interval, 
      Visualization &vis, std::string writer_type, std::string file_name, 
      shawn::SimulationController& sc)
   :vis_(&vis),
    refresh_interval_(refresh_interval),
    file_name_(file_name)
   {
      WriterFactoryHandle wfh = sc.keeper_by_name_w<WriterKeeper>("WriterKeeper")
		   ->find_w(writer_type);
	   writer_ = wfh->create();
      writer_->pre_write( visualization(), file_name_, true );
      writer_->set_draft(0);
   }
   // ----------------------------------------------------------------------
   WriteAnimationFrameEvent::
   ~WriteAnimationFrameEvent()
   {
      std::cout << "~WriteAnimationEvent" << std::endl;
      writer_->post_write();
      delete writer_;
   }
   // ----------------------------------------------------------------------
   void WriteAnimationFrameEvent::timeout(shawn::EventScheduler &es, 
         shawn::EventScheduler::EventHandle eh, double t, 
         shawn::EventScheduler::EventTagHandle &)
      throw()
   {
	  // Writes current network state to file.
      write_frame(t);
      
	  // Re-schedules the event for next execution.
      es.move_event(eh, t + refresh_interval_);
   }
   // ----------------------------------------------------------------------
   void
   WriteAnimationFrameEvent::
   write_frame(double t)
      throw( std::runtime_error )
   {
      writer_->write_frame( t );
   }
}
#endif
