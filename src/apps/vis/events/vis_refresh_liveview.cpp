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
#include "apps/vis/events/vis_refresh_liveview.h"
#include "apps/vis/base/vis_liveview.h"
#include <cairo.h>
#include <time.h>
#include <sstream>

using namespace shawn;

namespace vis
{
   RefreshLiveviewEvent::
   RefreshLiveviewEvent(double refresh_interval, int min_delay, Visualization &vis)
   :vis_(&vis),
    refresh_interval_(refresh_interval)
   {
#ifdef HAVE_BOOST
      boost::xtime_get(&last_refresh_, boost::TIME_UTC);
      last_refresh_.sec -= 1;
      boost::mutex::scoped_lock lock(*getUpdateMutex());
#endif
      min_delay_time_ = min_delay;
	  // Convert to Nanoseconds as needed by boost::thread:
      min_delay_time_ *= 1000000;

      // Writes current network state to texture.
      write_frame();

	  // Informs the external window that texture data has changed.
      updateTexture(getTexture());
   }
   // ----------------------------------------------------------------------
   RefreshLiveviewEvent::
   ~RefreshLiveviewEvent()
   {}
   // ----------------------------------------------------------------------
   void RefreshLiveviewEvent::timeout(shawn::EventScheduler &es,
         shawn::EventScheduler::EventHandle eh, double t,
         shawn::EventScheduler::EventTagHandle &)
      throw()
   {
#ifdef HAVE_BOOST
      // Compute timestamp of next refresh.
      last_refresh_.nsec += min_delay_time_;
	  // Sleeps until next refresh (if needed, maybe already reached).
      boost::thread::sleep(last_refresh_);
	  // Start refresh and mark current time as time of last refresh.
      boost::xtime_get(&last_refresh_, boost::TIME_UTC);
	  // Lock the texture update mutex!
      boost::mutex::scoped_lock lock(*getUpdateMutex());
#endif
	  // Writes current network state to texture.
      write_frame();

	  // Informs the external window that texture data has changed.
      updateTexture(getTexture());

	  // Re-schedules the event for next execution.
      es.move_event(eh, t + refresh_interval_);
   }
   // ----------------------------------------------------------------------
   void
   RefreshLiveviewEvent::
   write_frame()
      throw( std::runtime_error )
   {
      double t = 0.0;
      unsigned char* texture_ = getTexture();
      const Camera& cam = visualization().camera();
      bool freezed = cam.cam_properties().freeze(t)>.5;
      cairo_surface_t* surface_;

      if( (!freezed) )
         {
            int w = int(ceil(cam.width(t)));
            int h = int(ceil(cam.height(t)));

            if( (w<5) || (h<5) )
               {
                  std::ostringstream oss;
                  oss << "Picture dimension too small: "
                      << w << "x" << h;
                  throw std::runtime_error(oss.str());
               }

            double scale = cam.scale(t);
            if( scale < EPSILON )
               {
                  std::ostringstream oss;
                  oss << "Scale too small: " << scale;
                  throw std::runtime_error(oss.str());
               }

            shawn::Vec cam_pos = cam.position(t);
            shawn::Vec cam_pos_shift = cam.position_shift(t);

            // ALLOC SURFACE
            // make_surface(w,h);
            surface_ =
               cairo_image_surface_create_for_data ( texture_,
                                                     CAIRO_FORMAT_ARGB32,
                                                     w,h,
                                                      w*4);


            // CLEAR
            shawn::Vec cam_bg = cam.background(t);
            cairo_t* cr = cairo_create(surface_);
            cairo_rectangle (cr, 0, 0, w, h);
            cairo_set_source_rgb (cr,cam_bg.x(),cam_bg.y(),cam_bg.z());
            cairo_fill (cr);

            shawn::Vec trans( (-cam_pos.x()*scale) + (double(w)/2.0) + cam_pos_shift.x(),
                              (-cam_pos.y()*scale) - (double(h)/2.0) - cam_pos_shift.y() );

            Context C(trans/scale,scale,w,h, 0);

            cairo_save(cr);
            // TRANSFORMATION
            cairo_translate( cr, trans.x(), -trans.y() );
            cairo_scale(cr,scale,-scale);

            //
            visualization().draw(cr,t,C);

            cairo_restore(cr);

            cairo_destroy (cr);
      }
   }
}
#endif
#endif
