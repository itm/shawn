/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_dynamic_tree_edges.h"
#include "apps/vis/elements/vis_drawable_edge_default.h"
#include "apps/vis/elements/vis_drawable_edge_dynamic_tree.h"
#include "apps/vis/elements/vis_drawable_node_default.h"

#ifdef HAVE_BOOST_REGEX
#include <boost/regex.hpp>
#endif


using namespace shawn;

namespace vis
{

   CreateDynamicTreeEdgesTask::
   CreateDynamicTreeEdgesTask()
   {}
   // ----------------------------------------------------------------------
   CreateDynamicTreeEdgesTask::
   ~CreateDynamicTreeEdgesTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateDynamicTreeEdgesTask::
   name( void )
      const throw()
   { return "vis_create_dynamic_tree_edges"; }
   // ----------------------------------------------------------------------
   std::string
   CreateDynamicTreeEdgesTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateDynamicTreeEdgesTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      GroupElement* all_edges = new GroupElement("all.edges");
      visualization_w().add_element(all_edges);

      std::string pref = sc.environment().
            optional_string_param("prefix",DrawableEdgeDynamicTree::PREFIX);
      std::string node_prefix = sc.environment().
            optional_string_param("node_prefix",DrawableNodeDefault::PREFIX);

      std::string _source_regex = sc.environment().optional_string_param("source_regex", ".*");
      std::string _target_regex = sc.environment().optional_string_param("target_regex", ".*");

      const shawn::Node &dummy = *(visualization().world().begin_nodes());
      std::cout << "Regex: " << _source_regex << std::endl;
      DrawableEdgeDynamicTree* ded =
                        new DrawableEdgeDynamicTree(dummy,dummy, pref, node_prefix, _source_regex, _target_regex);
      ded->init();
      visualization_w().add_element(ded);
      all_edges->add_element(*ded);
   }
   // ----------------------------------------------------------------------
   GroupElement*
   CreateDynamicTreeEdgesTask::
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
   CreateDynamicTreeEdgesTask::
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
