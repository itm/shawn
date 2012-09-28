/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_group_task.h"
#include "apps/vis/elements/vis_group_element.h"


using namespace shawn;

namespace vis
{

   CreateGroupTask::
   CreateGroupTask()
   {}
   // ----------------------------------------------------------------------
   CreateGroupTask::
   ~CreateGroupTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateGroupTask::
   name( void )
      const throw()
   { return "vis_create_group"; }
   // ----------------------------------------------------------------------
   std::string
   CreateGroupTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateGroupTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      GroupElement* ge =
         new GroupElement( sc.environment().required_string_param("group") );
      ge->init();
      visualization_w().add_element( ge );
   }


}
#endif
