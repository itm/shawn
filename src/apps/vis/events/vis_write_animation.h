/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_VIS_WRITE_ANIMATION_EVENT_H
#define __SHAWN_VIS_WRITE_ANIMATION_EVENT_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_VIS
#include "apps/vis/base/vis_task.h"
#include "sys/event_scheduler.h"
#include "apps/vis/writer/vis_writer.h"

#ifdef HAVE_BOOST
#include <boost/thread.hpp>
#endif

namespace vis
{
   /** \brief Writes a frame of an animation.
    * This event is added to shauns event scheduler automatically by calling 
	 * ExternalAnimationTask.
    */
   class WriteAnimationFrameEvent
      : public shawn::EventScheduler::EventHandler
   {
   public:
      ///@name Constructor/Destructor
	   ///@{
      /**
	   * Creates the event.
	   */
      WriteAnimationFrameEvent(double refresh_interval, 
         Visualization &vis, std::string writer_type, std::string file_name,
         shawn::SimulationController& sc);
      virtual ~WriteAnimationFrameEvent();
      ///@}

      void timeout(shawn::EventScheduler &es, 
         shawn::EventScheduler::EventHandle eh, double t, 
         shawn::EventScheduler::EventTagHandle &) throw();

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

      virtual void write_frame(double t)
         throw( std::runtime_error );

   private:
      /// Handle of the visualization instance.
      VisualizationHandle vis_;

      /// Refresh interval (shawn time)
      double refresh_interval_;

      /// Vis Writer
      Writer *writer_;

      /// File name
      std::string file_name_;

#ifdef HAVE_BOOST
      /// Time of last refresh
      boost::xtime last_refresh_;
#endif
   };

}

#endif
#endif
