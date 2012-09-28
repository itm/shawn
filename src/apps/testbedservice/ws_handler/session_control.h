/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_SESSION_CONTROL_H_
#define __SHAWN_APPS_TESTBEDSERVICE_SESSION_CONTROL_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/core/testbedservice_client.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include <list>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace testbedservice
{

   class SessionControl
   {
   public:
      SessionControl();
      virtual ~SessionControl();

      void init( shawn::SimulationController& sc, TestbedServiceClient& controller ) throw();
      // --------------------------------------------------------------------
      std::string wsn_api_uri( void ) throw();
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
      TestbedServiceClient *controller_;
      shawn::SimulationController *sc_;

      std::string wsn_api_uri_;
   };

}

#endif
#endif
