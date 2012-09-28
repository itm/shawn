/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/double/vis_properties_double_init.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/properties/double/vis_property_smooth_double.h"
#include "apps/vis/properties/double/vis_property_breathing_double.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"


namespace vis
{

  void init_vis_double_properties( shawn::SimulationController& sc )
  {
     sc.simulation_task_keeper_w().add( new PropertyConstantDoubleTask );
     sc.simulation_task_keeper_w().add( new PropertySmoothDoubleTask );
     sc.simulation_task_keeper_w().add( new PropertyBreathingDoubleTask );
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/double/vis_properties_double_init.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/05 20:22:35 $
 *-----------------------------------------------------------------------
 * $Log: vis_properties_double_init.cpp,v $
 * Revision 1.3  2006/02/05 20:22:35  ali
 * more vis
 *
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
