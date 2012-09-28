#ifndef __SHAWN_APPS_WISELIB_EVENT_TAG_H_
#define __SHAWN_APPS_WISELIB_EVENT_TAG_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "sys/event_scheduler.h"

namespace wiselib
{

   template<typename TimerDelegateType>
   class WiselibEventTag
      : public shawn::EventScheduler::EventTag
   {
   public:
      typedef TimerDelegateType shawn_timer_delegate_t;
      // --------------------------------------------------------------------
      WiselibEventTag( shawn_timer_delegate_t del, void *data );
      virtual ~WiselibEventTag() {};  
      // --------------------------------------------------------------------
      inline void* userdata( void ) const throw()
      { return userdata_; }
      // --------------------------------------------------------------------
      inline shawn_timer_delegate_t delegate( void ) const throw()
      { return delegate_; }
      
   private:
      void *userdata_;
      shawn_timer_delegate_t delegate_;
   };
   // -----------------------------------------------------------------------
   template<typename TimerDelegateType>
   WiselibEventTag<TimerDelegateType>::
   WiselibEventTag( shawn_timer_delegate_t del, void *data )
      : userdata_ ( data ),
         delegate_( del )
   {}

}

#endif
#endif
