/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_KEEPERS_H
#define __SHAWN_TUBSAPPS_VIS_KEEPERS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

namespace shawn {
   class SimulationController;
}

namespace vis
{
   class VisualizationKeeper;
   
   const VisualizationKeeper& visualization_keeper( shawn::SimulationController& ) throw();
   VisualizationKeeper& visualization_keeper_w( shawn::SimulationController& ) throw();
   
}
#endif
#endif
