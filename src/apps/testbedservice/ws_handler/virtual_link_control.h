/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_CONTROL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_CONTROL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "apps/testbedservice/util/types.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/event_scheduler.h"
#include <list>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace testbedservice
{

   class VirtualLinkControl
      : public shawn::EventScheduler::EventHandler
   {
   public:
      VirtualLinkControl();
      virtual ~VirtualLinkControl();

      void timeout( shawn::EventScheduler&,
                     shawn::EventScheduler::EventHandle,
                     double,
                     shawn::EventScheduler::EventTagHandle& ) throw();

      void init( shawn::SimulationController& sc,
                 TestbedServiceClient& controller,
                 VirtualLinkTransmissionModel *vltm ) throw();
      // --------------------------------------------------------------------
      void set_virtual_link( std::string id,
                             std::string source, std::string destination,
                             std::string uri ) throw();
      void destroy_virtual_link( std::string id,
                     std::string source, std::string destination ) throw();
      void enable_physical_link( std::string id, std::string source, std::string destination );
      void disable_physical_link( std::string id, std::string source, std::string destination );
      // --------------------------------------------------------------------
      void add_virtual_message( std::string dest, TestbedserviceMessage message ) throw();
      // --------------------------------------------------------------------
      inline shawn::SimulationController& simulation_controller_w( void ) throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline const shawn::SimulationController& simulation_controller( void ) const throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline VirtualLinkTransmissionModel& virtual_link_transmission_model_w( void ) throw()
      { assert(vltm_); return *vltm_; }
      // --------------------------------------------------------------------
      inline const VirtualLinkTransmissionModel& virtual_link_transmission_model( void ) const throw()
      { assert(vltm_); return *vltm_; }
      // --------------------------------------------------------------------
      inline TestbedServiceClient& controller_w( void ) throw()
      { assert(controller_); return *controller_; }
      // --------------------------------------------------------------------
      inline const TestbedServiceClient& controller( void ) const throw()
      { assert(controller_); return *controller_; }

   private:
      typedef std::list<shawn::EventScheduler::EventHandler*> EventHandlerList;
      typedef EventHandlerList::iterator EventHandlerListIterator;
      typedef EventHandlerList::const_iterator ConstEventHandlerListIterator;

      VirtualLinkTransmissionModel *vltm_;
      TestbedServiceClient *controller_;
      shawn::SimulationController *sc_;

      boost::mutex sc_mutex_;
   };

}

#endif
#endif
