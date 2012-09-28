/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_VISUALIZATION_H
#define __SHAWN_TUBSAPPS_VIS_VISUALIZATION_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_element.h"
#include "sys/util/handle_keeper.h"
#include "apps/vis/elements/vis_element.h"
#include "apps/vis/elements/vis_drawable.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "apps/vis/elements/vis_camera.h"
#include "apps/vis/elements/vis_group_element.h"
#include "apps/vis/base/vis_needs_cairo.h"
#include "apps/vis/base/vis_context.h"
#include <map>
#include <iostream>
#include <vector>

namespace shawn
{ class SimulationController; class World; }

namespace vis
{
   DECLARE_HANDLES( Visualization );

   /** \brief Visualization main class.
    *
    * This class represents the visualization inside Shawn. It's the base class,
    * which manages all visualization issues.
    */
   class Visualization
      : public shawn::KeeperManaged, public shawn::NodeChangeListener
   {
   public:
      typedef std::map<std::string,ElementHandle> ElementMap;
      typedef std::vector<DrawableHandle> DrawableList;

      ///@name Constructor/Destructor
      ///@{
      Visualization( const std::string& );
      virtual ~Visualization();
      ///@}

      ///@name Getter/Setter
      ///@{
      /**
       * Returns the name of the visualization.
       */
      virtual std::string name( void ) const throw();
      /**
       * Shot description of the KeeperManaged subclass.
       */
      virtual std::string description( void ) const throw();

      /**
       * Returns the camera of the visualization.
       */
      inline Camera& camera_w( void ) throw()
      { assert( elem_camera_ != NULL ); return *elem_camera_; }
      /**
       * Returns the camera of the visualization (constant).
       */
      inline const Camera& camera( void ) const throw()
      { assert( elem_camera_ != NULL ); return *elem_camera_; }

      /**
       * Sets the network world to draw.
       */
      virtual void set_world( const shawn::World& ) throw();

      /**
       * Returns the element with the given name.
       *
       * \param n Element name.
       */
      inline ElementHandle element_w( const std::string& n ) throw()
      {
         ElementMap::const_iterator it = elements_.find(n);
         return it==elements_.end()
            ? NULL
            : it->second;
      }
      /**
       * Returns the constant element with the given name.
       *
       * \param n Element name.
       */
      inline ConstElementHandle element( const std::string& n ) const throw()
      {
         ElementMap::const_iterator it = elements_.find(n);
         return it==elements_.end()
            ? NULL
            : it->second;
      }

      /**
       * Returns the world, that is currently associated with the visualization
       * (constant).
       */
      inline const shawn::World& world( void ) const throw()
      { assert( world_ != NULL ); return *world_; }

      /**
       * Returns a map of all Vis elements, that are derived from the
       * vis::Element class.
       */
      inline ElementMap& elements( void ) throw()
      { return elements_; }
      ///@}

      /**
       * Base initialization.
       */
      virtual void init( void ) throw();

      /**
       * Adds a new element to the visualization.
       */
      virtual void add_element( const ElementHandle& )
         throw( std::runtime_error );

      /**
       * Starts the drawing process. Draws all drawable elements iteratively.
       */
      virtual void draw( cairo_t*, double, const Context& )
         const throw( std::runtime_error );

      /// NodeChangeListener implementations
      virtual void node_added(shawn::Node &) throw();
      virtual void node_removed(shawn::Node &) throw();
      virtual void id_changed(int,int) throw();
      virtual bool invalidate(void) throw();

   protected:
      /**
       * Creates a camera instance.
       */
      virtual Camera* init_camera( void ) throw();

   private:
      /// Visualization name.
      std::string name_;
      /// Network world.
      const shawn::World* world_;
      /// Camera instance.
      Camera* elem_camera_;
      /// Map of all vis::Element objects.
      ElementMap elements_;
      /// List of all vis::Drawable objects.
      mutable DrawableList drawables_;
   };


}

#endif
#endif
