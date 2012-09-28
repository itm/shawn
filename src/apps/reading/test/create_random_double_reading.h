/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_READING_DOUBLE_TEST_CREATE_H
#define __SHAWN_APPS_READING_DOUBLE_TEST_CREATE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include "sys/simulation/simulation_task.h"
#include "sys/processor.h"
#include "sys/node.h"

namespace reading
{

   class SimulationTaskReadingDoubleTestCreate
      : public shawn::SimulationTask
   {

   public:

      SimulationTaskReadingDoubleTestCreate();
      virtual ~SimulationTaskReadingDoubleTestCreate();

      virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   };

}// namespace reading
#endif
#endif

