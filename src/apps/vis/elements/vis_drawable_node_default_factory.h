/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __VIS_DRAWABLE_NODE_DEFAULT_FACTORY_H
#define __VIS_DRAWABLE_NODE_DEFAULT_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/elements/vis_drawable_node_factory.h"
#include "apps/vis/elements/vis_drawable_node.h"


namespace vis
{
   DECLARE_HANDLES(DrawableNodeDefaultFactory);

   /** \brief A factory for creating DrawableNodeDefault instances.
    * This WriterFactory creates instances of DrawableNodeDefault.
    */
   class DrawableNodeDefaultFactory
	   : public DrawableNodeFactory
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      DrawableNodeDefaultFactory( void );
      virtual ~DrawableNodeDefaultFactory();
      ///@}

      ///@name Getters
      ///@{
      /**
       * The name of this factory.
       */
      virtual std::string name( void ) const throw();
      /**
       * Short description of this factory.
       */
      virtual std::string description( void ) const throw();
      ///@}

      /**
       * Creates and returns a DrawableNode.
       */
      virtual DrawableNode* create( const shawn::Node& v ) throw(std::runtime_error);

      /**
       * Static method to register the factory to the NodeKeeper.
       */
      static void register_factory( shawn::SimulationController& ) throw();
   };
}

#endif
#endif
