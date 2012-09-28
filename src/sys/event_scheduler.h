/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_EVENT_SCHEDULER_H
#define __SHAWN_SYS_EVENT_SCHEDULER_H

#include "sys/util/refcnt_pointable.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "shawn_config.h"
#include <set>

// Note that this define does enable the rate adaptation automatically!
// The simulation task must be called with the (optional) boolean parameter "enable_realtime_rate=true" to enable the rate adaptation.
#define SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
	#define SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION_RESOLUTION_MICROSECONDS
	#include "sys/misc/os/system_time.h"
#endif

//#define MULTITHREADED_EVENT_SCHEDULER
#ifdef MULTITHREADED_EVENT_SCHEDULER
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#endif

namespace shawn
{

	class EventScheduler
	{
	public:

	   ///@name Local Types
	   ///@{
	   class EventHandler;
	   class EventInfo;

	   ///
	   typedef EventInfo* EventHandle;

	   ///
	   static const EventHandle NO_EVENT;

	   ///
	   struct EventSorter
	   {
		  bool operator() ( const EventHandle& e1, const EventHandle& e2 ) const throw()
		  { return (*e1) < (*e2); }
	   };

	   ///
	   typedef std::set<EventHandle,EventSorter> EventSet;

	   ///
	   DECLARE_HANDLES(EventTag);

	   /// Stuff to derive from, thereby adding tagged info to events
	   class EventTag
		  : public RefcntPointable
	   {
	   public:
		   EventTag() {};
		   virtual ~EventTag() {};
	   };

	   ///
	   class EventInfo
	   {
		  friend class EventScheduler;
	   public:
		  bool operator < ( const EventInfo& ei );
		  double time() const throw();
	   private:
		  EventInfo( EventHandler&, double, const EventTagHandle& );
		  ~EventInfo();
		  EventHandler*      handler_;
		  double             time_;
		  EventSet::iterator pos_;
		  bool               dead_;
		  EventTagHandle     tag_;
	   };

	   /// Interface for event-handling classes
	   class EventHandler
	   {
	   public:
		  ///
		  EventHandler();
		  ///
		  virtual ~EventHandler();
		  /// Main interface method. Called when a scheduled event happens,
		  /// that is, timeouts.
		  /// EventHandle gets automagically deleted after handler returns,
		  /// unless move_event() is used.
		  /// Deleting the event during it's handling is a no-no.
		  virtual void timeout( EventScheduler&, EventHandle, double, EventTagHandle& ) throw() = 0;
	   };
	   ///@}

	   ///@name Construction / Destruction
	   ///@{
	   ///
	   EventScheduler();
	   ///
	   virtual ~EventScheduler();
	   ///@}


	   ///@name Event Management
	   ///@{
	   /** Creates a new event that timeouts at time \a t, at which time
		* \a eh ->timeout() will be called.
		* \a tag may be NULL or anything the handler might find useful
		* \returns a handle (read: ID) by which the new event is identified
		*/
	   EventScheduler::EventHandle new_event( EventScheduler::EventHandler& eh,
	            double t, const EventScheduler::EventTagHandle& eth ) throw();
	   /** Removes the specified event. Must not be called from within
		* timeout() (at least for the timeouting event)
		*/
	   void delete_event( EventHandle ) throw();
	   /** Re-schedules the event for a different time, abandoning the current one
		*/
	   void move_event( EventHandle, double ) throw();
	   ///@}

//       const EventScheduler::EventHandle find_event( EventHandler& eh ) const;

	   ///@name Info Access
	   ///@{
	   ///
	   double current_time( void ) const throw();
	   ///
	   double next_event_time( void ) const throw();
	   ///
	   bool empty( void ) const throw();
	   ///
	   EventHandle front_w( void ) throw();
	   ///
	   const EventHandle front( void ) const throw();
	   ///@}

	   ///@name Running
	   ///@{
#ifdef MULTITHREADED_EVENT_SCHEDULER
      ///
      double update_time( void ) const throw();
      ///
      bool timed_wait( int millis ) throw();
#endif
	   ///
	   void playback( double stop_time ) throw();
	   ///
	   void clear( double new_time = 0.0 ) throw();
	   ///@}
	   void set_rate_adaptation(bool enable) throw();

	private:
	   EventSet events_;
	   mutable double time_;

		#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
		    void adapt_rate(double net) throw();
			SystemTime sys_time_;
			mutable double last_touch_time_;
			bool rate_adaptation_enabled_;
		#endif

#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::posix_time::ptime last_round_;

      boost::posix_time::ptime last_event_;
      /// Time of last update of time_ - required in update_time() to define
      /// the passed time.
      mutable boost::posix_time::ptime begin_cond_wait_;
      /// Set when EventScheduler is going to sleep - if so, current_time()
      /// updates time_ when called (because if called at this moment, it
      /// *must* have happened externally.
      bool waiting_;
      /// Global storage of maximal time that time_ can be set to. The value
      /// is given by the parameter stop_time in playback().
      double max_stop_time_;
      ///< Protect EventSet events_ from multiple access
      mutable boost::mutex events_mutex_;
      /// Used by condition variable external_event_cond_
      boost::mutex external_event_mutex_;
      /// Sleep when nothing to do, but get notified when new event added
      boost::condition_variable external_event_cond_;
#endif

	};



}

#endif
