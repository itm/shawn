/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_group_add_by_tag_task.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/elements/vis_auto_elements.h"

#include <stdlib.h>
using namespace shawn;

namespace vis
{

   GroupAddByTagTask::
   GroupAddByTagTask()
   {}
   // ----------------------------------------------------------------------
   GroupAddByTagTask::
   ~GroupAddByTagTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   GroupAddByTagTask::
   name( void )
      const throw()
   { return "vis_group_add_by_tag"; }
   // ----------------------------------------------------------------------
   std::string
   GroupAddByTagTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   GroupAddByTagTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
#ifdef HAVE_BOOST_REGEX
      std::string n = sc.environment().required_string_param("group");
      std::string tag = sc.environment().required_string_param("tag");
      std::string regex = sc.environment().required_string_param("tag_regex");

      ElementHandle eh = 
         visualization_w().
         element_w( n );
      if( eh.is_null() )
         throw std::runtime_error(std::string("no such group: ")+n);
      GroupElement* ge = dynamic_cast<GroupElement*>(eh.get());
      if( ge == NULL )
         throw std::runtime_error(std::string("element is no group: ")+n);


      //std::string ename = sc.environment().required_string_param("elem");
      //eh = visualization_w().element_w( ename );
      //if( eh.is_null() )
      //    throw std::runtime_error(std::string("no such element: ")+ename);
      AutoElements ae( sc, visualization_w() );
      int i=0;
      
      while( (eh=ae.top()).is_not_null() )
      {
         i++;
         ge->add_element( *eh );
         ae.pop();
      }

      std::cout << i << " elements added to group " << n << std::endl;

#endif
   }

}
#endif
