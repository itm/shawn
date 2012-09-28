/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_edges_tag.h"
#include "apps/vis/elements/vis_drawable_edge_default.h"
#include "apps/vis/elements/vis_drawable_edge_estimated.h"
#include "apps/vis/elements/vis_drawable_node_default.h"
#include "sys/world.h"

#ifdef HAVE_BOOST_REGEX
#include <boost/regex.hpp>
#endif


using namespace shawn;

namespace vis
{

   CreateEdgesTagTask::
   CreateEdgesTagTask()
   {}
   // ----------------------------------------------------------------------
   CreateEdgesTagTask::
   ~CreateEdgesTagTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTagTask::
   name( void )
      const throw()
   { return "vis_create_edges_by_tag"; }
   // ----------------------------------------------------------------------
   std::string
   CreateEdgesTagTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateEdgesTagTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
#ifndef HAVE_BOOST_REGEX
      throw std::runtime_error("no boost::regex support compiled in");
#else
      boost::regex sources(sc.environment().required_string_param("source_regex"));
      boost::regex targets(sc.environment().required_string_param("target_regex"));
      std::string taglabel = sc.environment().required_string_param("tag");

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
         
         if( boost::regex_search(get_stringtag(&*it,taglabel)->value(),sources))
            {
               for( shawn::Node::const_adjacency_iterator
                       ait    = it->begin_adjacent_nodes(),
                       endait = it->end_adjacent_nodes();
                    ait != endait; ++ait )
                  if( *it != *ait )
                     if( boost::regex_search(get_stringtag(&*ait,taglabel)->value(),targets) )
                        if( (ait->label() > it->label()) ||
                            (!boost::regex_search(get_stringtag(&*it,taglabel)->value(),targets)) ||
                            (!boost::regex_search(get_stringtag(&*ait,taglabel)->value(),sources)) )
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
                           }
            }
      }
#endif
   }
   // ----------------------------------------------------------------------
   GroupElement*
   CreateEdgesTagTask::
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
   CreateEdgesTagTask::
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
   const shawn::StringTag* CreateEdgesTagTask::get_stringtag(const shawn::Node *node, std::string taglabel)
   {
      const shawn::StringTag *out = NULL;
      shawn::ConstTagHandle htag = node->find_tag(taglabel);
      if(htag != NULL)
      {
         out = dynamic_cast<const shawn::StringTag*>(htag.get());
      }

      return out;
   }

}
#endif
