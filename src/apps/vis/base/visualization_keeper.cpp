/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/visualization_keeper.h"

namespace vis
{
   const std::string VisualizationKeeper::KEEPER_NAME( "visualization" );
   // ----------------------------------------------------------------------
   VisualizationKeeper::
   VisualizationKeeper()
      : shawn::HandleKeeper<Visualization>( KEEPER_NAME, "visualizations" )
   {}
   // ----------------------------------------------------------------------
   VisualizationKeeper::
   ~VisualizationKeeper()
   {}
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/base/visualization_keeper.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: visualization_keeper.cpp,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
