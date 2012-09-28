/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   SimulationTask::~SimulationTask()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTask::
   require_world( const SimulationController& sc )
      const throw( std::runtime_error )
   {
      if( !sc.has_world() )
         throw std::runtime_error( std::string("Task '") +
                                   name() +
                                   std::string("' needs a world.") );
   }
   // ----------------------------------------------------------------------
   SimulationTask::ResultSetIt
	   SimulationTask::
	   begin_results()
   {
	   return results_.begin();
   }
   // ----------------------------------------------------------------------
   SimulationTask::ResultSetIt
	   SimulationTask::
	   end_results()
   {
	   return results_.end();
   }
   // ----------------------------------------------------------------------
   void
	   SimulationTask::
	   add_result(SimulationTaskResult* res)
   {
	   results_.insert(res);
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_task.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: simulation_task.cpp,v $
 *-----------------------------------------------------------------------*/
