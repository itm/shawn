/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_TRACE_TASK_H
#define __SHAWN_APPS_WISEML_TRACE_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <iostream>
using namespace shawn;
using namespace std;
namespace wiseml
{
   /** \brief Creates a Trace
    */
   class WisemlTraceTask
      : public SimulationTask
   {
   public:
      /// Constructor/Destructor
      WisemlTraceTask();
      virtual ~WisemlTraceTask();

      /// KeeperManaged implementations:
      virtual string name() const throw();
      virtual string description() const throw();

      /// SimulationTest implementations:
      virtual void run( SimulationController &) throw();
   };
}
#endif
#endif
