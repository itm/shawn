/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_group_add_task.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/elements/vis_auto_elements.h"


using namespace shawn;

namespace vis
{

   GroupAddTask::
   GroupAddTask()
   {}
   // ----------------------------------------------------------------------
   GroupAddTask::
   ~GroupAddTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   GroupAddTask::
   name( void )
      const throw()
   { return "vis_group_add"; }
   // ----------------------------------------------------------------------
   std::string
   GroupAddTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   GroupAddTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      std::string n = sc.environment().required_string_param("group");
      ElementHandle eh = 
         visualization_w().
         element_w( n );
      if( eh.is_null() )
         throw std::runtime_error(std::string("no such group: ")+n);
      GroupElement* ge = dynamic_cast<GroupElement*>(eh.get());
      if( ge == NULL )
         throw std::runtime_error(std::string("element is no group: ")+n);


      std::string ename = sc.environment().required_string_param("elem");
      eh = visualization_w().element_w( ename );
      if( eh.is_null() )
          throw std::runtime_error(std::string("no such element: ")+ename);
      AutoElements ae( sc, visualization_w() );
      
      while( (eh=ae.top()).is_not_null() )
      {
         ge->add_element( *eh );
         ae.pop();
      }
   }


}
#endif
