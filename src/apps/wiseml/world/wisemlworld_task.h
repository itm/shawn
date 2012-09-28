/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_WORLD_TASK_H
#define __SHAWN_APPS_WISEML_WORLD_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/simulation/simulation_task.h"
#include "sys/worlds/world_tasks.h"
using namespace shawn;

namespace wiseml
{
   /**
    * A task which creates the WiseMlProcessorWorldFactory.
    */
   class SimulationTaskWiseMLWorldFactory
      : public shawn::SimulationTaskProcessorWorldFactory
   {
   public:
      SimulationTaskWiseMLWorldFactory();
      virtual ~SimulationTaskWiseMLWorldFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual shawn::ProcessorWorldFactory* create_factory( shawn::SimulationController& )
         throw( std::runtime_error );
      virtual void set_node_count( SimulationController&,
                                   ProcessorWorldFactory& ) throw( std::runtime_error );
   };
}

#endif
#endif
