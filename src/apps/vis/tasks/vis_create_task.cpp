/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_task.h"
#include "apps/vis/base/vis_keepers.h"
#include "apps/vis/base/visualization_keeper.h"
#include "apps/vis/elements/vis_drawable_node_default.h"
#include "apps/vis/elements/vis_drawable_node_factory.h"
#include "apps/vis/elements/vis_drawable_node_keeper.h"
#include "apps/vis/elements/vis_group_element.h"

using namespace shawn;

namespace vis
{

   VisualizationTaskCreate::
   VisualizationTaskCreate()
   {}
   // ----------------------------------------------------------------------
   VisualizationTaskCreate::
   ~VisualizationTaskCreate()
   {}
   // ----------------------------------------------------------------------
   std::string
   VisualizationTaskCreate::
   name( void )
      const throw()
   { return "vis_create"; }
   // ----------------------------------------------------------------------
   std::string
   VisualizationTaskCreate::
   description( void )
      const throw()
   { return "Creates a new visualization named $vis for the current world"; }
   // ----------------------------------------------------------------------
   void
   VisualizationTaskCreate::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      Visualization* vis;

      require_world(sc);

      std::string name = sc.environment().optional_string_param( "vis", "visualization" );
      visualization_keeper_w(sc).add(vis=new Visualization(name));

      std::string type = sc.environment().optional_string_param( "node_type", "default" );
      DrawableNodeFactoryHandle dnfh = sc.keeper_by_name_w<DrawableNodeKeeper>("DrawableNodeKeeper")
         ->find_w(sc.environment().optional_string_param("drawable_nodes", type));

      sc.world_w().add_node_change_listener(*vis);
      vis->set_world( sc.world() );
      vis->init();

      DEBUG( logger(),
             "created visualization '" << name << "'" );

      GroupElement* ge =
         new GroupElement( "all.nodes" );
      ge->init();
      vis->add_element(ge);

      for( shawn::World::const_node_iterator
              it = sc.world().begin_nodes();
           it != sc.world().end_nodes();
           ++it )
         {
            DrawableNode *dn = dnfh->create(*it);
            dn->init();
            vis->add_element(dn);
            ge->add_element(*dn);
         }
   }


}
#endif
