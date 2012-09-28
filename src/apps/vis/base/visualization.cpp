/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/visualization.h"
#include "apps/vis/elements/vis_drawable_node_factory.h"
#include "apps/vis/elements/vis_drawable_node_keeper.h"
#include "apps/vis/elements/vis_drawable_node_default.h"
#include "sys/logging/logger.h"
#include <sstream>
#include <algorithm>

namespace vis
{

   Visualization::
   Visualization( const std::string& n )
      : name_ ( n )
   {}
   // ----------------------------------------------------------------------
   Visualization::
   ~Visualization()
   {
      /*shawn::World *w = const_cast<shawn::World*>(&(world()));
      if(w!=NULL)
         w->remove_node_change_listener(*this);*/
      std::cout << "~Visualization " << name_ << " (" << elements_.size()
         << " elems)" << std::endl;
   }
   // ----------------------------------------------------------------------
   std::string
   Visualization::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   Visualization::
   description( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   void
   Visualization::
   set_world( const shawn::World& w )
      throw()
   {
      world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   Visualization::
   init( void )
      throw()
   {
      assert( world_ != NULL );
      add_element( elem_camera_ = init_camera() );
   }
   // ----------------------------------------------------------------------
   Camera*
   Visualization::
   init_camera( void )
      throw()
   {
      Camera* cam = new Camera;
      cam->init();
      return cam;
   }
   // ----------------------------------------------------------------------
   void
   Visualization::
   add_element( const ElementHandle& eh )
      throw( std::runtime_error )
   {
      if( elements_.find(eh->name()) != elements_.end() )
         throw std::runtime_error( std::string(
               "duplicate visualization elements with name ") +
                                   eh->name() );
      elements_[eh->name()] = eh;

      Drawable* d = dynamic_cast<Drawable*>(eh.get());
      if( d != NULL )
         drawables_.push_back(d);

      eh->set_visualization(*this);
   }
   // ----------------------------------------------------------------------
   // NodeChangeListener Implementation
   // ----------------------------------------------------------------------
   void Visualization::node_removed(shawn::Node &node) throw()
   {
      std::cout << "Vis: Removing node: " << node.label() << std::endl;

      int e_index = 0;
      for( DrawableList::const_iterator
                    it    = drawables_.begin(),
                    endit = drawables_.end();
                 it != endit; ++it )
      {
         Drawable * dable = (*it).get();
         DrawableNode * dnode = dynamic_cast<DrawableNode *>(dable);
         if(dnode != NULL && dnode->node() == node)
         {
            std::string dnode_name = dnode->name();
            // Remove from global element list:
            elements_.erase(dnode_name);
            // Remove from drawable element list:
            drawables_.erase(drawables_.begin() + e_index);
            // Remove from "all.nodes" group:
            GroupElement *nodes = dynamic_cast<GroupElement*>
               (elements_.find("all.nodes")->second.get());
            nodes->remove_element(*dnode);

            std::cout << "Vis: DrawableNode removed: " << dnode_name
               << std::endl;

            break;
         }
         e_index++;

      }
   }
   // ----------------------------------------------------------------------
   void Visualization::node_added(shawn::Node &node) throw()
   {
      std::cout << "Vis: Adding node: " << node.label() << std::endl;
      shawn::SimulationController &sc =
         const_cast<shawn::SimulationController &>(
               world().simulation_controller());

      DrawableNodeFactoryHandle dnfh = sc.
         keeper_by_name_w<DrawableNodeKeeper>("DrawableNodeKeeper")
         ->find_w(sc.environment().optional_string_param(
         "drawable_nodes", "default"));

      GroupElement *nodes = dynamic_cast<GroupElement*>
                     (elements_.find("all.nodes")->second.get());

      DrawableNode *dn = dnfh->create(node);
      dn->init();
      add_element(dn);
      nodes->add_element(*dn);
   }
   // ----------------------------------------------------------------------
   void Visualization::id_changed(int oldid, int newid) throw()
   {
      // Vis doesn't use IDs
   }
   // ----------------------------------------------------------------------
   bool Visualization::invalidate( void ) throw()
   {
      return false;
   }
   // ----------------------------------------------------------------------
   class PriorityDescOrder
   {
   public:
      PriorityDescOrder( double t ):t_(t){};
      ~PriorityDescOrder(){};
      bool operator() ( const DrawableHandle& d1,
                        const DrawableHandle& d2 )
      { return d1->priority(t_) < d2->priority(t_); }
      double t_;
   };
   // ----------------------------------------------------------------------
   void
   Visualization::
   draw( cairo_t* cr,
         double t,
         const Context& C )
      const throw( std::runtime_error )
   {
      PriorityDescOrder order(t);
      std::stable_sort(drawables_.begin(), drawables_.end(), order);
      for( DrawableList::const_iterator
              it    = drawables_.begin(),
              endit = drawables_.end();
           it != endit; ++it ) {
         try {
            (**it).draw(cr,t,C);
         }
         catch( std::runtime_error& re ) {
            std::ostringstream oss;
            oss << "cannot draw '"
                << (**it).name()
                << "' at time "
                << t
                << ": " << re.what();
            throw std::runtime_error(oss.str());
         }
      }
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/base/visualization.cpp,v $
 * Version $Revision: 1.5 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: visualization.cpp,v $
 * Revision 1.5  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/01/31 12:44:41  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
