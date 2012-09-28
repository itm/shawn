/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <limits>
#include "apps/localization/tasks/localization_select_node.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/taggings/basic_tags.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/vec.h"
#include "apps/localization/util/localization_defutils.h"


namespace localization
{

   SimulationTaskLocalizationSelectNode::
   SimulationTaskLocalizationSelectNode()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLocalizationSelectNode::
   ~SimulationTaskLocalizationSelectNode()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskLocalizationSelectNode::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      double pos_x = sc.world().simulation_controller().environment().
         optional_double_param( "loc_sel_pos_x", 0.0 );
      double pos_y = sc.world().simulation_controller().environment().
         optional_double_param( "loc_sel_pos_y", 0.0 );
      double pos_z = sc.world().simulation_controller().environment().
         optional_double_param( "loc_sel_pos_z", 0.0 );
      
      shawn::Node *node = NULL;
      double min_dist = std::numeric_limits<double>::max();
      shawn::Vec pos( pos_x, pos_y, pos_z );
      
      for( shawn::World::node_iterator
               it = sc.world_w().begin_nodes_w();
               it != sc.world_w().end_nodes_w();
               ++it )
      {
         double distance = euclidean_distance( pos, it->real_position() );
         if ( distance < min_dist )
         {
            min_dist = distance;
            node = &(*it);
         }
      }

      if ( node )
      {
         shawn::BoolTag *boolt = new shawn::BoolTag( SEL_NODE_TAG_NAME, true );
         boolt->set_persistency( true );
         node->add_tag( boolt );
      }
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationSelectNode::
   name( void )
      const throw()
   {
      return std::string( "localization_select_node" );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLocalizationSelectNode::
   description( void ) 
      const throw()
   {
      return std::string( "..." );
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/tasks/localization_select_node.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_select_node.cpp,v $
 *-----------------------------------------------------------------------*/
