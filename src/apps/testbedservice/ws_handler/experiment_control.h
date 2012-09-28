/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SIMULATION_CONTROL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SIMULATION_CONTROL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/util/types.h"
#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/event_scheduler.h"
#include <list>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace testbedservice
{

   class ExperimentControl
   {
   public:
      struct FlashProgram
      {
         uint8_t *data;
         int data_len;
         std::string version;
         std::string name;
         std::string platform;
         std::string other;
      };

      typedef std::vector<int> IndicesVector;
      typedef std::vector<FlashProgram> FlashProgramVector;

      ExperimentControl();
      virtual ~ExperimentControl();

      void init( shawn::SimulationController& sc, TestbedServiceClient& controller ) throw();

      void flash_programs( std::string id, NodeIdVector nodes,
                           IndicesVector indices, FlashProgramVector programs )
         throw();
      // --------------------------------------------------------------------
      inline shawn::SimulationController& simulation_controller_w( void ) throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline const shawn::SimulationController& simulation_controller( void ) const throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline TestbedServiceClient& controller( void ) throw()
      { assert(controller_); return *controller_; }


   private:
      typedef std::list<shawn::EventScheduler::EventHandler*> EventHandlerList;
      typedef EventHandlerList::iterator EventHandlerListIterator;
      typedef EventHandlerList::const_iterator ConstEventHandlerListIterator;

      TestbedServiceClient *controller_;
      shawn::SimulationController *sc_;
      boost::mutex sc_mutex_;

      boost::mutex handler_list_mutex_;
      EventHandlerList handler_list_;
   };

}

#endif
#endif
