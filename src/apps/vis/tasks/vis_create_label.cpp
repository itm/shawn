/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/tasks/vis_create_label.h"
#include "apps/vis/elements/vis_drawable_label.h"
#include "apps/vis/elements/vis_drawable_node_default.h"
#include "sys/world.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag.h"
#include <stdlib.h>

using namespace shawn;

namespace vis
{
   CreateLabelTask::
      CreateLabelTask()
   {}
   // ----------------------------------------------------------------------
   CreateLabelTask::
      ~CreateLabelTask()
   {}
   // ----------------------------------------------------------------------
   std::string CreateLabelTask::
      name( void )
      const throw()
   { return "vis_create_label"; }
   // ----------------------------------------------------------------------
   std::string CreateLabelTask::
      description( void )
      const throw()
   { return "Creates labels attached to nodes"; }
   // ----------------------------------------------------------------------
   void CreateLabelTask::
      run( SimulationController &sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      std::string node_prefix =
         sc.environment().
         optional_string_param("node_prefix",DrawableNodeDefault::PREFIX);
      std::string tagname = sc.environment().optional_string_param("tag", "");

      //double range = sc.environment().optional_double_param("range",1.0);
      //DrawableComradius *dcr = new DrawableComradius("comradius", range);
      //dcr->init();
      //visualization_w().add_element(dcr);
      //visualization_w().elements().

      for( shawn::World::const_node_iterator
              it    = visualization().world().begin_nodes(),
              endit = visualization().world().end_nodes();
           it != endit; ++it )
      {
         const DrawableNode* dsrc = drawable_node(*it,node_prefix);
         std::string prefix("label.");
         DrawableLabel *dl = new DrawableLabel(prefix + it->label(), dsrc);
         dl->use_tag(tagname);

         dl->init();

         visualization_w().add_element(dl);

      }

   }

   const DrawableNode*
   CreateLabelTask::
   drawable_node( const shawn::Node& v,
                  const std::string& nprefix )
      throw( std::runtime_error )
   {
      std::string n = nprefix+std::string(".")+v.label();
      ConstElementHandle eh =
         visualization().element( n );
      if( eh.is_null() )
         throw std::runtime_error(std::string("no such element: ")+n);
      const DrawableNode* dn = dynamic_cast<const DrawableNode*>(eh.get());
      if( dn == NULL )
         throw std::runtime_error(std::string("element is no DrawableNode: ")+n);
      return dn;
   }

}

#endif
