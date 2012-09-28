/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/tasks/vis_show_comradius.h"
#include "apps/vis/elements/vis_drawable_comradius.h"
#include "sys/world.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag.h"
#include <stdlib.h>

using namespace shawn;

namespace vis
{
   ShowComradiusTask::
      ShowComradiusTask()
   {}
   // ----------------------------------------------------------------------
   ShowComradiusTask::
      ~ShowComradiusTask()
   {}
   // ----------------------------------------------------------------------
   std::string ShowComradiusTask::
      name( void )
      const throw()
   { return "vis_show_comradius"; }
   // ----------------------------------------------------------------------
   std::string ShowComradiusTask::
      description( void )
      const throw()
   { return "Shows communication radius"; }
   // ----------------------------------------------------------------------
   void ShowComradiusTask::
      run( SimulationController &sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      double range = sc.environment().optional_double_param("range",1.0);
      DrawableComradius *dcr = new DrawableComradius("comradius", range);
      dcr->init();
      visualization_w().add_element(dcr);

   }

}

#endif
