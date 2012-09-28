/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_estimated_edges.h"
#include "apps/vis/elements/vis_drawable_edge_estimated.h"
#include "apps/vis/elements/vis_drawable_node_default.h"

using namespace shawn;

namespace vis
{

   CreateEstimatedEdgesTask::
   CreateEstimatedEdgesTask()
   {}
   // ----------------------------------------------------------------------
   CreateEstimatedEdgesTask::
   ~CreateEstimatedEdgesTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateEstimatedEdgesTask::
   name( void )
      const throw()
   { return "vis_create_estimated_edges"; }
   // ----------------------------------------------------------------------
   std::string
   CreateEstimatedEdgesTask::
   description( void )
      const throw()
   { return "Creates edges from real to estimated node positions"; }
   // ----------------------------------------------------------------------
   void
   CreateEstimatedEdgesTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      std::string pref = sc.environment().
         optional_string_param("prefix",DrawableEdgeEstimated::PREFIX);
      std::string node_prefix =
         sc.environment().
         optional_string_param("node_prefix",DrawableNodeDefault::PREFIX);
      GroupElement* grp = group(sc);

      for( shawn::World::const_node_iterator
              it    = visualization().world().begin_nodes(),
              endit = visualization().world().end_nodes();
           it != endit; ++it )
      {
         if( it->has_est_position())
         {
            const DrawableNode* dn = drawable_node(*it, node_prefix);
            DrawableEdgeEstimated* dee = new DrawableEdgeEstimated(*it, *dn, 
               DrawableEdgeEstimated::PREFIX);
            dee->init();
            visualization_w().add_element(dee);
            if( grp != NULL )
               grp->add_element(*dee);
         }
      }
   }
   // ----------------------------------------------------------------------
   GroupElement*
   CreateEstimatedEdgesTask::
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
   CreateEstimatedEdgesTask::
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
