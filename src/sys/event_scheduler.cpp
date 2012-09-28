/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/constants.h"
#include "sys/event_scheduler.h"
#include "sys/util/defutils.h"
#include <stdlib.h>

#ifdef MULTITHREADED_EVENT_SCHEDULER
#include "boost/date_time/posix_time/posix_time.hpp"
#endif



namespace shawn
{
   const EventScheduler::EventHandle EventScheduler::NO_EVENT = NULL;

	// ----------------------------------------------------------------------
	EventScheduler::EventInfo::
		EventInfo( EventHandler& h, double t, const EventTagHandle& tag )
		: handler_ ( &h ),
		  time_    ( t ),
          dead_    ( false ),
          tag_     ( tag )
	{}

	// ----------------------------------------------------------------------
	EventScheduler::EventInfo::
		~EventInfo()
	{}

	// ----------------------------------------------------------------------
	bool
	EventScheduler::EventInfo::
	operator < ( const EventScheduler::EventInfo& ei )
	{
		if (EQDOUBLE(time_, ei.time_))
			return (void*)this > (void*)(&ei);
		else
			return time_ < ei.time_;
	}

	// ----------------------------------------------------------------------
	double
		EventScheduler::EventInfo::
		time()
		const throw()
	{
		return time_;
	}






	EventScheduler::EventHandler::
		EventHandler()
	{}

	// ----------------------------------------------------------------------
	EventScheduler::EventHandler::
		~EventHandler()
	{}





	// ----------------------------------------------------------------------
	EventScheduler::
		EventScheduler()
		: time_ ( 0.0 )
#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
		, last_touch_time_(0.0)
		, rate_adaptation_enabled_(true)
#endif
#ifdef MULTITHREADED_EVENT_SCHEDULER
         , last_event_( boost::posix_time::microsec_clock::local_time() ),
            last_round_( boost::posix_time::microsec_clock::local_time() ),
            waiting_     ( false )
#endif
	{}

   // ----------------------------------------------------------------------
	EventScheduler::
		~EventScheduler()
	{}

	// ----------------------------------------------------------------------
	EventScheduler::EventHandle
		EventScheduler::
		new_event( EventScheduler::EventHandler& h, double t, const EventScheduler::EventTagHandle& eth )
		throw()
	{
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
      if ( t < current_time() )
         t = current_time();
#else
      assert( t >= current_time() );
#endif
      EventHandle eh = new EventInfo( h, t, eth );

		std::pair<EventSet::iterator,bool> worked = events_.insert(eh);
		if( !worked.second )
		{
			std::cout << "ASSERT OCCURED: " << std::endl;
			std::cout << "this ->time_ " << eh->time_ << ", ptr=" << (void*)eh << std::endl;
			std::cout << "(*it)->time_ " << (*(worked.first))->time_ << ", ptr=" << (void*)(*(worked.first)) << std::endl;
		}
		assert( worked.second );
		eh->pos_ = worked.first;
#ifdef MULTITHREADED_EVENT_SCHEDULER
      external_event_cond_.notify_all();
#endif
		return eh;
	}

   // ----------------------------------------------------------------------
   void
   EventScheduler::
   delete_event( EventScheduler::EventHandle eh )
      throw()
   {
		assert( !eh->dead_ ); // must not delete_event in it's timeout
		assert( eh != NULL );
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		bool event_found = false;
		if (events_.find(eh) == events_.end())
		{
			for (EventSet::iterator it = events_.begin(); it != events_.end(); ++it)
			{
				if ( eh->pos_ == it )
				{
					event_found = true;
					break;
				}
			}
		}
		else
		{
			event_found = true;
		}

		assert( event_found );
		events_.erase( eh->pos_ );
		delete eh;
	}

	// ----------------------------------------------------------------------
	void
		EventScheduler::
		move_event( EventHandle eh, double newt )
		throw()
	{
		assert( newt >= current_time() );
		assert( eh != NULL );
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		bool event_found = false;
		if (events_.find(eh) == events_.end())
		{
			for (EventSet::iterator it = events_.begin(); it != events_.end(); ++it)
			{
				if ( eh->pos_ == it )
				{
					event_found = true;
					break;
				}
			}
		}
		else
		{
			event_found = true;
		}

		assert( event_found );
		events_.erase( eh->pos_ );
		eh->time_ = newt;
		std::pair<EventSet::iterator,bool> worked = events_.insert(eh);
		assert( worked.second );
		eh->pos_ = worked.first;
		eh->dead_ = false;
#ifdef MULTITHREADED_EVENT_SCHEDULER
      external_event_cond_.notify_all();
#endif
	}

/*
    // ----------------------------------------------------------------------
   const EventScheduler::EventHandle
   EventScheduler::
   find_event( EventHandler& eh )
   const
   {
      for (EventSet::const_iterator it = events_.begin(); it != events_.end(); ++it){
         if ( &eh == (*it)->handler_ ){
            return *it;
         }
      }
      return NULL;
   }
*/

	// ----------------------------------------------------------------------
	double
		EventScheduler::
		current_time( void )
		 const throw()
	{
#ifdef MULTITHREADED_EVENT_SCHEDULER
      if ( waiting_ )
         update_time();
#endif
		return time_;
	}
#ifdef MULTITHREADED_EVENT_SCHEDULER
   // ----------------------------------------------------------------------
   double
   EventScheduler::
   update_time( void )
      const throw()
   {
      boost::posix_time::ptime end_cond_wait =
            boost::posix_time::microsec_clock::local_time();
      boost::posix_time::time_duration td = end_cond_wait - begin_cond_wait_;
      time_ += td.total_milliseconds() / 1000.0;
      if ( time_ > max_stop_time_ )
         time_ = max_stop_time_;
      begin_cond_wait_ = boost::posix_time::microsec_clock::local_time();
   }
   // ----------------------------------------------------------------------
   bool
   EventScheduler::
   timed_wait( int millis )
      throw()
   {
      boost::unique_lock<boost::mutex> lk( external_event_mutex_ );
      boost::posix_time::time_duration td = boost::posix_time::milliseconds( millis );

      double wakeup_time = (millis / 1000.0) + current_time();

      //std::cout << "At " << current_time() << std::endl;
      //std::cout << "  wait for " << td.total_milliseconds() << std::endl;
      //std::cout << "  wakeup: " << wakeup_time << "; ne: " << next_event_time() << std::endl;

      if ( !empty() && next_event_time() < wakeup_time )
         return false;

      // FIXME: If an event is "injected" exactly at this moment (and thus
      //        before external_event_cond_.timed_wait is called), the
      //        EventScheduler would not recognize this, and the potential
      //        event would be executed delayed!

      begin_cond_wait_ = boost::posix_time::microsec_clock::local_time();
      waiting_ = true;
      if ( external_event_cond_.timed_wait( lk, td ) )
      {
         update_time();
         waiting_ = false;
         return false;
      }

      waiting_ = false;
      return true;
   }
   // ----------------------------------------------------------------------
   void
   EventScheduler::
   playback( double stop_time )
      throw()
   {
      assert( stop_time >= current_time() );

      max_stop_time_ = stop_time;
      while( 1 )
      {
         if( empty() )
         {

            boost::posix_time::ptime now =
               boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration td = now - last_event_;
            int real_duration = td.total_milliseconds();
            int virt_duration = 1000.0 * (stop_time - time_);

            //std::cout << "ev::empty: real=" << real_duration << "; virt=" << virt_duration << std::endl;

            if( real_duration < virt_duration )
            {
               double millis = virt_duration - real_duration;
               if ( !timed_wait( millis ) )
               {
//                   std::cout << "EventScheduler: Net < Stop: Interrupted at "
//                      << time_ << std::endl;
                  continue;
               }
            }

//             int millis = 1000.0 * (stop_time - current_time());
//             if ( !timed_wait( millis ) )
//             {
// //                std::cout << "EventScheduler: Net < Stop: Interrupted at "
// //                   << time_ << std::endl;
//                continue;
//             }

            last_event_ = boost::posix_time::microsec_clock::local_time();
            time_ = stop_time;
            break;
         }

         double net = next_event_time();
//          std::cout << "Next event at " << net << "; "
//             << "we have " << current_time() << std::endl;
         if( net < stop_time )
         {
            boost::posix_time::ptime now =
               boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration td = now - last_event_;
            int real_duration = td.total_milliseconds();
            int virt_duration = 1000.0 * (net - time_);

//             std::cout << "ev::nec<stop: real=" << real_duration << "; virt=" << virt_duration << std::endl;

            if( real_duration < virt_duration )
            {
               double millis = virt_duration - real_duration;
               if ( !timed_wait( millis ) )
               {
//                   std::cout << "EventScheduler: Net < Stop: Interrupted at "
//                      << time_ << std::endl;
                  continue;
               }
               last_event_ = boost::posix_time::microsec_clock::local_time();
            }

            time_ = net;
            EventScheduler::EventHandle eh = front_w();

            assert( eh );
            assert( eh->pos_ == events_.begin() );
            assert( !eh->dead_ );

            eh->dead_ = true;
            eh->handler_->timeout( *this, eh, net, eh->tag_ );

            if( eh->dead_ )
            {
               boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
               events_.erase( eh->pos_ );
               delete eh;
            }
         }
         else
         {

            boost::posix_time::ptime now =
               boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration td = now - last_event_;
            int real_duration = td.total_milliseconds();
            int virt_duration = 1000.0 * (stop_time - time_);

            //std::cout << "ev::nec>stop: real=" << real_duration << "; virt=" << virt_duration << std::endl;

            if( real_duration < virt_duration )
            {
               double millis = virt_duration - real_duration;
               if ( !timed_wait( millis ) )
               {
//                   std::cout << "EventScheduler: Net < Stop: Interrupted at "
//                      << time_ << std::endl;
                  continue;
               }
            }

//             std::cout << "ev::nec>stop: stop=" << stop_time << "; cur=" << current_time() << std::endl;
// 
//             int millis = 1000.0 * (stop_time - current_time());
//             if ( !timed_wait( millis ) )
//             {
// //                std::cout << "EventScheduler: Net >= Stop: Interrupted at "
// //                   << time_ << std::endl;
//                continue;
//             }

            last_event_ = boost::posix_time::microsec_clock::local_time();
            time_ = stop_time;
            break;
         }
      }

      boost::posix_time::ptime pb = boost::posix_time::microsec_clock::local_time();
      boost::posix_time::time_duration td = pb - last_round_;
      //std::cout << "PB fin at " << pb << " with duration of " << td.total_milliseconds() << std::endl;
      last_round_ = boost::posix_time::microsec_clock::local_time();
   }
#else
   // ----------------------------------------------------------------------
   void
   EventScheduler::
   playback( double stop_time )
      throw()
   {
      assert( stop_time >= current_time() );

      while( 1 )
      {

         if( empty() )
         {
            #ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
			   /*sys_time_.sleep( int(1000.0 * (stop_time - current_time())) );
               sys_time_.touch();
               last_touch_time_ = stop_time;
               */
        	 if (rate_adaptation_enabled_)
        		 adapt_rate(stop_time);
            #endif
            time_ = stop_time;
            return;
         }

         //double temp = (1000.0 * stop_time - 1000.0*last_touch_time_); // ms
         //double real = double(sys_time_.us_since_last_touch()); // us
         double net = next_event_time();
         //std::cout << "cur " << current_time() << " net " << net << " stop time " << stop_time << " virt " << temp << " real " << real << std::endl << std::flush;

         if( net < stop_time)
         {
        	#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
        	 if (rate_adaptation_enabled_)
				 adapt_rate(net);
            #endif
            time_ = net;
            EventScheduler::EventHandle eh = front_w();

            assert( eh );
            assert( eh->pos_ == events_.begin() );
            assert( net == eh->time_ );
            assert( !eh->dead_ );

            eh->dead_ = true;
            eh->handler_->timeout( *this, eh, net, eh->tag_ );

            if( eh->dead_ )
            {
               events_.erase( eh->pos_ );
               delete eh;
            }
         }
         else
         {
            #ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
        	 /*
               std::cout << "cur " << current_time() << " net " << net << " stop time " << stop_time << " virt " << temp << " real " << real << std::endl << std::flush;
               //std::cout << "2 sleep " << int(1000.0 * (stop_time - current_time())) << " stop " << stop_time << std::endl << std::flush;
               sys_time_.sleep( int(1000.0 * (stop_time - current_time())) );
               sys_time_.touch();
               last_touch_time_ = stop_time;
               */
        	 if (rate_adaptation_enabled_)
			   adapt_rate(stop_time);
            #endif
            time_ = stop_time;
            return;
         }
      }
   }

   // ----------------------------------------------------------------------
   void
   EventScheduler::
   adapt_rate(double net)
   throw()
   {
	   #ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION_RESOLUTION_MICROSECONDS
		   double real_duration = double(sys_time_.us_since_last_touch()); // us
		   double virt_duration = (1000000.0 * net - 1000000.0*last_touch_time_); // us
		   if( (real_duration + 1000.0) <= virt_duration )
		   {
			   sys_time_.sleep_us( int((virt_duration - real_duration)) - 100 );
			   sys_time_.touch();
			   last_touch_time_ = net;
		   }
		   else if( real_duration > virt_duration+1000000 )
		   {
			   sys_time_.touch();
			   last_touch_time_ = net;
		   }
		#else
		   double real_duration = double(sys_time_.ms_since_last_touch()); // ms
		   double virt_duration = (1000.0 * net - 1000.0*last_touch_time_); // ms
		   if( real_duration + 10 <= virt_duration ) // 10ms is minimum resolution for windows sleep function
		   {
			   sys_time_.sleep( int((virt_duration - real_duration)) );
			   last_touch_time_ = net;
			   sys_time_.touch();
		   }
		   else if( real_duration > virt_duration+1000 )
		   {
			   last_touch_time_ = net;
			   sys_time_.touch();
		   }
		#endif
   }
#endif
	// ----------------------------------------------------------------------
	double
		EventScheduler::
		next_event_time( void )
		const throw()
	{
		assert( !empty() );
		return front()->time_;
	}

	// ----------------------------------------------------------------------
	bool
		EventScheduler::
		empty( void )
		const throw()
	{
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		return events_.empty();
	}

	// ----------------------------------------------------------------------
	EventScheduler::EventHandle
		EventScheduler::
		front_w( void )
		throw()
	{
		assert( !empty() );
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		return *events_.begin();
	}

	// ----------------------------------------------------------------------
	const EventScheduler::EventHandle
		EventScheduler::
		front( void )
		const throw()
	{
		assert( !empty() );
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		return *events_.begin();
	}

	// ----------------------------------------------------------------------
	void
		EventScheduler::
		clear( double new_time )
		throw()
	{
#ifdef MULTITHREADED_EVENT_SCHEDULER
      boost::unique_lock<boost::mutex> event_queue_lock( events_mutex_ );
#endif
		for( EventSet::iterator it = events_.begin(); it != events_.end(); ++it )
			if(*it)
				delete *it;

		events_.clear();
		time_ = new_time;
	}
	// ----------------------------------------------------------------------
	void
		EventScheduler::
		set_rate_adaptation( bool enable )
		throw()
	{
		#ifdef SHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION
			rate_adaptation_enabled_ = enable;
		#else
			if (enable)
				std::cout << "Rate adaptation support not available." << std::endl;
		#endif
	}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/event_scheduler.cpp,v $
 * Version $Revision: 418 $
 * Date    $Date: 2010-05-25 23:15:10 +0200 (Tue, 25 May 2010) $
 *-----------------------------------------------------------------------
 * $Log: event_scheduler.cpp,v $
 *-----------------------------------------------------------------------*/
