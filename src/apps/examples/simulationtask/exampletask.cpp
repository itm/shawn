/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/examples/simulationtask/exampletask.h"
#ifdef ENABLE_EXAMPLES

#include "sys/world.h"

namespace exampletask
{

   ExampleTask::
   ExampleTask()
   {}
   // ----------------------------------------------------------------------
   ExampleTask::
   ~ExampleTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   ExampleTask::
   name( void )
      const throw()
   {
      return "exampletask";
   }
   // ----------------------------------------------------------------------
   std::string
   ExampleTask::
   description( void )
      const throw()
   {
      return "example task for seeing how a task has to be implemented";
   }
   // ----------------------------------------------------------------------
   void
   ExampleTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      double count = 0;

      for( shawn::World::const_node_iterator
               it = sc.world().begin_nodes();
               it != sc.world().end_nodes();
               ++it )
      {
         count++;
      }
      
      INFO( logger(), "visited " << count << " nodes" );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask.cpp,v $
 * Version $Revision: 126 $
 * Date    $Date: 2008-02-21 16:33:02 +0100 (Thu, 21 Feb 2008) $
 *-----------------------------------------------------------------------
 * $Log: exampletask.cpp,v $
 *-----------------------------------------------------------------------*/
