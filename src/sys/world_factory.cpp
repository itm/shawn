/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/world.h"
#include "sys/world_factory.h"

#include "sys/edge_models/lazy_edge_model.h"
#include "sys/comm_models/disk_graph_model.h"


namespace shawn
{

   WorldFactory::
   WorldFactory()
   {}
   // ----------------------------------------------------------------------
   WorldFactory::
   ~WorldFactory()
   {}
   // ----------------------------------------------------------------------
   World*
   WorldFactory::
   create( void ) 
      throw()
   {
      World* w = create_empty_world();
      fill_world( *w );
      return w;
   }
   // ----------------------------------------------------------------------
   World* 
   WorldFactory::
   create_empty_world( void ) 
      throw()
   {
      World* nw = new World;
      nw->set_communication_model( *(new DiskGraphModel) );
      nw->set_edge_model( *(new LazyEdgeModel) );
      nw->init();
      return nw;
   }
   // ----------------------------------------------------------------------
   void
   WorldFactory::
   fill_world( World& )
      throw()
   {}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/world_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
