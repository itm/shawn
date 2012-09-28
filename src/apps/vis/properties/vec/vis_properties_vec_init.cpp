/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vec/vis_properties_vec_init.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/properties/vec/vis_property_smooth_vec.h"
#include "apps/vis/properties/vec/vis_property_real_node_position.h"
#include "apps/vis/properties/vec/vis_property_tag_color_vec.h"
#include "apps/vis/properties/vec/vis_property_vec_extended_tag_color.h"
#include "apps/vis/properties/vec/vis_property_indexed_tag_color_vec.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"


namespace vis
{

  void init_vis_vec_properties( shawn::SimulationController& sc )
  {
     sc.simulation_task_keeper_w().add( new PropertyConstantVecTask );
     sc.simulation_task_keeper_w().add( new PropertySmoothVecTask );
     sc.simulation_task_keeper_w().add( new PropertyRealNodePositionTask );
     sc.simulation_task_keeper_w().add( new PropertyTagColorVecTask );
     sc.simulation_task_keeper_w().add( new PropertyExtendedTagColorVecTask );
	 sc.simulation_task_keeper_w().add( new PropertyIndexedTagColorVecTask);
  }

}

#endif
