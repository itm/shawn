/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_edges.h"
#include "apps/vis/elements/vis_drawable_edge_default.h"
#include "apps/vis/elements/vis_drawable_edge_estimated.h"
#include "apps/vis/elements/vis_drawable_node_default.h"

#ifdef HAVE_BOOST_REGEX
#include <boost/regex.hpp>
#endif


using namespace shawn;

namespace vis
{

   CreateEdgesTask::
   CreateEdgesTask()
   {}
   // ----------------------------------------------------------------------
   CreateEdgesTask::
   ~CreateEdgesTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTask::
   name( void )
      const throw()
   { return "vis_create_edges"; }
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateEdgesTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      GroupElement* all_edges = new GroupElement("all.edges");
      visualization_w().add_element(all_edges);

#ifndef HAVE_BOOST_REGEX

      std::string pref = sc.environment().
            optional_string_param("prefix",DrawableEdgeDefault::PREFIX);
      std::string node_prefix = sc.environment().
            optional_string_param("node_prefix",DrawableNodeDefault::PREFIX);

      for( shawn::World::const_node_iterator
              it    = visualization().world().begin_nodes(),
              endit = visualization().world().end_nodes();
           it != endit; ++it )
      {
         for( shawn::Node::const_adjacency_iterator
                       ait    = it->begin_adjacent_nodes(),
                       endait = it->end_adjacent_nodes();
                    ait != endait; ++ait )
         {
            if( *it != *ait && (ait->label() > it->label()) )
            {
               const DrawableNode* dsrc =
                        drawable_node(*it,node_prefix);
               const DrawableNode* dtgt =
                        drawable_node(*ait,node_prefix);
               DrawableEdgeDefault* ded =
                        new DrawableEdgeDefault(*it,*ait,*dsrc,*dtgt,pref);
               ded->init();
               visualization_w().add_element(ded);
               all_edges->add_element(*ded);
            }
         }

      }
#else
      boost::regex sources(sc.environment().optional_string_param("source_regex", ".*"));
      boost::regex targets(sc.environment().optional_string_param("target_regex", ".*"));

      std::string pref = sc.environment().
         optional_string_param("prefix",DrawableEdgeDefault::PREFIX);
      std::string node_prefix =
         sc.environment().
         optional_string_param("node_prefix",DrawableNodeDefault::PREFIX);
      GroupElement* grp = group(sc);

      for( shawn::World::const_node_iterator
              it    = visualization().world().begin_nodes(),
              endit = visualization().world().end_nodes();
           it != endit; ++it )
      {
         if( boost::regex_search(it->label(),sources))
            {
               for( shawn::Node::const_adjacency_iterator
                       ait    = it->begin_adjacent_nodes(),
                       endait = it->end_adjacent_nodes();
                    ait != endait; ++ait )
                  if( *it != *ait )
                     if( boost::regex_search(ait->label(),targets) )
                        if( (ait->label() > it->label()) ||
                            (!boost::regex_search(it->label(),targets)) ||
                            (!boost::regex_search(ait->label(),sources)) )
                           {
                              const DrawableNode* dsrc =
                                 drawable_node(*it,node_prefix);
                              const DrawableNode* dtgt =
                                 drawable_node(*ait,node_prefix);
                              DrawableEdgeDefault* ded =
                                 new DrawableEdgeDefault(*it,*ait,*dsrc,*dtgt,pref);
                              ded->init();
                              visualization_w().add_element(ded);
                              if( grp != NULL )
                                 grp->add_element(*ded);
                              all_edges->add_element(*ded);
                           }
            }
      }
#endif
   }
   // ----------------------------------------------------------------------
   GroupElement*
   CreateEdgesTask::
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
   CreateEdgesTask::
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
