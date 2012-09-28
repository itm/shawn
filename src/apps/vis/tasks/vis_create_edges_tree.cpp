/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_edges_tree.h"
#include "apps/vis/elements/vis_drawable_edge_default.h"
#include "apps/vis/elements/vis_drawable_edge_estimated.h"
#include "apps/vis/elements/vis_drawable_node_default.h"
#include "sys/world.h"

using namespace shawn;

namespace vis
{

   CreateEdgesTreeTask::
   CreateEdgesTreeTask()
   {}
   // ----------------------------------------------------------------------
   CreateEdgesTreeTask::
   ~CreateEdgesTreeTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTreeTask::
   name( void )
      const throw()
   { return "vis_create_edges_tree"; }
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTreeTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateEdgesTreeTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      const std::string taglabel = "predecessor";

      std::string pref = sc.environment().
         optional_string_param( "prefix", DrawableEdgeDefault::PREFIX );
      std::string node_prefix =
         sc.environment().
         optional_string_param( "node_prefix", DrawableNodeDefault::PREFIX );
      GroupElement* grp = group(sc);

      for( shawn::World::const_node_iterator
              it    = visualization().world().begin_nodes(),
              endit = visualization().world().end_nodes();
           it != endit; ++it )
      {
         std::string pred = read_predecessor( *it, taglabel );
         if ( pred.empty() )
            continue;
         
         const shawn::Node *predecessor = sc.world().find_node_by_label( pred );
         if ( !predecessor )
            continue;
         
         const DrawableNode* dsrc = drawable_node( *it,node_prefix );
         const DrawableNode* dtgt = drawable_node( *predecessor, node_prefix );
         DrawableEdgeDefault* ded = new DrawableEdgeDefault( *it, *predecessor, *dsrc, *dtgt, pref );
         ded->init();
         visualization_w().add_element(ded);
         if( grp != NULL )
            grp->add_element(*ded);
         
      }
   }
   // ----------------------------------------------------------------------
   GroupElement*
   CreateEdgesTreeTask::
   group( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string n = sc.environment().optional_string_param("group","");
      if( n.empty() ) 
         return NULL;
      ElementHandle eh = 
          visualization_w().element_w( n );
      if( eh.is_null() )
         throw std::runtime_error(std::string("no such group: ")+n);
      GroupElement* ge = dynamic_cast<GroupElement*>(eh.get());
      if( ge == NULL )
         throw std::runtime_error(std::string("element is no group: ")+n);
      return ge;
   }
   // ----------------------------------------------------------------------
   const DrawableNode*
   CreateEdgesTreeTask::
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
   // ----------------------------------------------------------------------
   std::string 
   CreateEdgesTreeTask::
   read_predecessor( const shawn::Node& node, const std::string& taglabel ) 
      const throw()
   {
      shawn::ConstTagHandle htag = node.find_tag( taglabel );
      if( htag != NULL )
      {
         const shawn::StringTag *strt = NULL;
         strt = dynamic_cast<const shawn::StringTag*>(htag.get());
         if ( strt )
            return strt->value(); 
      }

      return std::string("");
   }

}

#endif
