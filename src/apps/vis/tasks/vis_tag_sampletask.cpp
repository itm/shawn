/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/tasks/vis_tag_sampletask.h"
#include "sys/world.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag.h"
#include <stdlib.h>

using namespace shawn;

namespace vis
{
   TagSampleTask::
      TagSampleTask()
   {}
   // ----------------------------------------------------------------------
   TagSampleTask::
      ~TagSampleTask()
   {}
   // ----------------------------------------------------------------------
   std::string TagSampleTask::
      name( void )
      const throw()
   { return "vis_tag_sample"; }
   // ----------------------------------------------------------------------
   std::string TagSampleTask::
      description( void )
      const throw()
   { return "Adds some sample string tags to nodes."; }
   // ----------------------------------------------------------------------
   void TagSampleTask::
      run( SimulationController &sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      World::node_iterator it = sc.world_w().begin_nodes_w();
      World::node_iterator endit = sc.world_w().end_nodes_w();
      bool second = true;

      shawn::TagHandle tag = NULL;
      shawn::TagHandle tag2 = NULL;

      while(it != endit)
      {

         if(second)
         {
            tag = new shawn::StringTag("VisTestTag", "VisTagSecond");
         }
         else
         {
            tag = new shawn::StringTag("VisTestTag", "VisTagFirst");
         }

         double randomVal = (double)rand()/RAND_MAX;
         tag2 = new shawn::DoubleTag("VisBattery", randomVal);

         it->add_tag(tag);
         it->add_tag(tag2);

         second = !second;
         ++it;

      }

      std::cout << "Tags attached" << std::endl;

   }

}

#endif
