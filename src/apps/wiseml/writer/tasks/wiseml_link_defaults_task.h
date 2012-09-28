/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_LINKDEFAULTS_TASK_H
#define __SHAWN_APPS_WISEML_LINKDEFAULTS_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include <iostream>
#include <time.h>
using namespace shawn;
namespace wiseml
{
   /** \brief WiseML file writer task
    * Creates or extends WiseML files with the current network
    * topology.
    */
   class WisemlLinkDefaultsTask
      : public SimulationTask
   {
   public:
      /// Constructor/Destructor
      WisemlLinkDefaultsTask();
      virtual ~WisemlLinkDefaultsTask();

      /// KeeperManaged implementations:
      virtual std::string name() const throw();
      virtual std::string description() const throw();

      /// SimulationTest implementations:
      virtual void run( SimulationController &) throw();
   protected:
      std::string replace_underline(std::string);
   };
}
#endif
#endif
