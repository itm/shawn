/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_keepers.h"
#include "apps/vis/base/visualization_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace vis
{

   const VisualizationKeeper&
   visualization_keeper( shawn::SimulationController& sc )
      throw()
   {
      const VisualizationKeeper* vk =
         sc.keeper_by_name<VisualizationKeeper>(VisualizationKeeper::KEEPER_NAME);
      assert( vk != NULL );
      return *vk;
   }
   // ----------------------------------------------------------------------
   VisualizationKeeper&
   visualization_keeper_w( shawn::SimulationController& sc )
      throw()
   {
      VisualizationKeeper* vk =
         sc.keeper_by_name_w<VisualizationKeeper>(VisualizationKeeper::KEEPER_NAME);
      assert( vk != NULL );
      return *vk;
   }




}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/base/vis_keepers.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: vis_keepers.cpp,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
