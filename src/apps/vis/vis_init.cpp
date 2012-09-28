/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/vis_init.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/vis/base/visualization_keeper.h"
#include "apps/vis/base/vis_base_init.h"
#include "apps/vis/tasks/vis_tasks_init.h"
#include "apps/vis/properties/vis_properties_init.h"

extern "C" void init_vis( shawn::SimulationController& sc )
{
   if( sc.keeper_by_name<vis::VisualizationKeeper>(vis::VisualizationKeeper::KEEPER_NAME)
        == NULL )
       {
          vis::init_vis_base(sc);
          vis::init_vis_tasks(sc);
          vis::init_vis_properties(sc);
       }
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/vis_init.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: vis_init.cpp,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
