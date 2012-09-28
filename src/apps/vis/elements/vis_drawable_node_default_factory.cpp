/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/elements/vis_drawable_node_default_factory.h"
#include "apps/vis/elements/vis_drawable_node_keeper.h"
#include "apps/vis/elements/vis_drawable_node_default.h"

namespace vis
{
   DrawableNodeDefaultFactory::
   DrawableNodeDefaultFactory( void )
   {}

   DrawableNodeDefaultFactory::~DrawableNodeDefaultFactory()
   {}

   // ----------------------------------------------------------------------
   void
   DrawableNodeDefaultFactory::
   register_factory( shawn::SimulationController& sc )
      throw()
   {
	   DrawableNodeKeeper* nkeeper = sc.keeper_by_name_w<DrawableNodeKeeper>( "DrawableNodeKeeper" );
	   if(nkeeper != NULL)
	   {
		   nkeeper->add(new DrawableNodeDefaultFactory());
	   }
   }

   // ----------------------------------------------------------------------
   std::string
   DrawableNodeDefaultFactory::
   name( void )
      const throw()
   { 
	   return "default"; 
   }
   
   // ----------------------------------------------------------------------
   std::string 
   DrawableNodeDefaultFactory::
   description( void )
      const throw()
   {
      return "DrawableNodeDefault Factory";
   }

   // ----------------------------------------------------------------------
   DrawableNode* 
   DrawableNodeDefaultFactory::
   create( const shawn::Node& v )
	throw( std::runtime_error )
   {
	   return new DrawableNodeDefault( v );
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
