/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __VIS_DRAWABLE_NODE_FACTORY_H
#define __VIS_DRAWABLE_NODE_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "apps/vis/elements/vis_drawable_node.h"
#include "sys/simulation/simulation_controller.h"


namespace vis
{
   DECLARE_HANDLES(DrawableNodeFactory);

   /** \brief Base class for node factories.
    * This class is the base class for all vis node factories, which create 
    * nodes of a given type. When creating new node types, add a factory for
    * every new type and add it to the node keeper.
    */
   class DrawableNodeFactory
	   : public shawn::KeeperManaged
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      DrawableNodeFactory( void );
      virtual ~DrawableNodeFactory();
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

      /**
       * Abstract method to create a DrawableNode.
       */
	   virtual DrawableNode* create( const shawn::Node& v ) throw(std::runtime_error) = 0;
   };
}

#endif
#endif
