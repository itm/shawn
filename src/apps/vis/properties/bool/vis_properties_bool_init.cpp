/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/bool/vis_properties_bool_init.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"


namespace vis
{

  void init_vis_bool_properties( shawn::SimulationController& sc )
  {
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/bool/vis_properties_bool_init.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/31 12:44:00 $
 *-----------------------------------------------------------------------
 * $Log: vis_properties_bool_init.cpp,v $
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
