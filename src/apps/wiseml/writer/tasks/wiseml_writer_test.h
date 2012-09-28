/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_WRITER_TEST_H
#define __SHAWN_APPS_WISEML_WRITER_TEST_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>
using namespace shawn;
namespace wiseml
{
   /** \brief WiseML file writer task
    * Creates or extends WiseML files with the current network
    * topology.
    */
   class WisemlWriterTest
      : public SimulationTask
   {
   public:
      /// Constructor/Destructor
      WisemlWriterTest();
      virtual ~WisemlWriterTest();

      /// KeeperManaged implementations:
      virtual std::string name() const throw();
      virtual std::string description() const throw();

      /// SimulationTest implementations:
      virtual void run( SimulationController &) throw();

   protected:
      /// File handling
      //virtual void pre_write();
      //virtual void post_write();
      
   };
}
#endif
#endif
