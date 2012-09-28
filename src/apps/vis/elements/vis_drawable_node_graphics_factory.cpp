/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "sys/node.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/vis/elements/vis_drawable_node_graphics_factory.h"
#include "apps/vis/elements/vis_drawable_node_keeper.h"
#include "apps/vis/elements/vis_drawable_node_graphics.h"

namespace vis
{
   DrawableNodeGraphicsFactory::
   DrawableNodeGraphicsFactory( void )
   {}
   // ----------------------------------------------------------------------
   DrawableNodeGraphicsFactory::
   ~DrawableNodeGraphicsFactory()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableNodeGraphicsFactory::
   register_factory( shawn::SimulationController& sc )
      throw()
   {
      DrawableNodeKeeper* nkeeper = sc.keeper_by_name_w<DrawableNodeKeeper>( "DrawableNodeKeeper" );
      if (nkeeper != 0)
      {
         nkeeper->add(new DrawableNodeGraphicsFactory());
      }
   }
   // ----------------------------------------------------------------------
   std::string
   DrawableNodeGraphicsFactory::
   name( void )
      const throw()
   {
      return "graphics";
   }
   // ----------------------------------------------------------------------
   std::string 
   DrawableNodeGraphicsFactory::
   description( void )
      const throw()
   {
      return "Draw a given image as a node.";
   }
   // ----------------------------------------------------------------------
   DrawableNode* 
   DrawableNodeGraphicsFactory::
   create( const shawn::Node& v )
      throw( std::runtime_error )
   {
      std::string fname = v.world().simulation_controller().environment().optional_string_param("filename", "");
      return new DrawableNodeGraphics( v, fname );
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processors/processor_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: processor_factory.cpp,v $
 *-----------------------------------------------------------------------*/
