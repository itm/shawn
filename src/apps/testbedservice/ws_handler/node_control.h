/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_NODE_CONTROL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_NODE_CONTROL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "apps/testbedservice/util/types.h"
#include "apps/testbedservice/processor/testbedservice_processor.h"
#include "apps/testbedservice/ws_handler/virtual_link_control.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/event_scheduler.h"
#include "sys/node.h"
#include "sys/processor.h"
#include <string>
#include <vector>
#include <list>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace testbedservice
{

   class NodeControl
   {
   public:
      NodeControl();
      virtual ~NodeControl();
      // --------------------------------------------------------------------
      void init( shawn::SimulationController& sc, TestbedServiceClient& controller, VirtualLinkControl& vlink_control ) throw();
      // --------------------------------------------------------------------
      void are_nodes_alive( std::string id, NodeIdVector nodes );
      void reset_nodes( std::string id, NodeIdVector nodes );
      void set_node_state( std::string id, std::string urn, shawn::Processor::ProcessorState state );
      void pass_message( std::string id, NodeIdVector nodes, TestbedserviceMessage message );
      NodeIdVector get_neighborhood( std::string urn );

      void dbg_nodevector( std::string id, NodeIdVector nodes );
      // --------------------------------------------------------------------
      inline shawn::SimulationController& simulation_controller_w( void ) throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline const shawn::SimulationController& simulation_controller( void ) const throw()
      { assert(sc_); return *sc_; }
      // --------------------------------------------------------------------
      inline TestbedServiceClient& controller( void ) throw()
      { assert(controller_); return *controller_; }
      // --------------------------------------------------------------------
      inline VirtualLinkControl& virtual_link_control( void ) throw()
      { assert(vlink_control_); return *vlink_control_; }

   private:

      TestbedServiceClient *controller_;
      VirtualLinkControl *vlink_control_;
      shawn::SimulationController *sc_;
   };

}

#endif
#endif
