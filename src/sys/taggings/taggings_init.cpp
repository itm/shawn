/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/taggings_init.h"
#include "sys/taggings/taggings_factories.h"
#include "sys/taggings/tag_factory_keeper.h"
#include "sys/taggings/map_tags.h"
#include "sys/taggings/taggings_tasks.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"
#include <iostream>

namespace shawn
{

   void 
   init_taggings( SimulationController& sc )
      throw()
   {
      sc.tag_factory_keeper_w().add( new IntegerTagFactory );
      sc.tag_factory_keeper_w().add( new DoubleTagFactory );
      sc.tag_factory_keeper_w().add( new BoolTagFactory );
      sc.tag_factory_keeper_w().add( new StringTagFactory );
      sc.tag_factory_keeper_w().add( new NodeReferenceTagFactory );

	  sc.tag_factory_keeper_w().add( new GroupTagFactory );

      sc.tag_factory_keeper_w().add( new MapTagFactory<IntegerIntegerMapTag> );
      sc.tag_factory_keeper_w().add( new MapTagFactory<IntegerDoubleMapTag > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<IntegerStringMapTag > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<IntegerBoolMapTag   > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<DoubleIntegerMapTag > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<DoubleDoubleMapTag  > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<DoubleStringMapTag  > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<DoubleBoolMapTag    > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<StringIntegerMapTag > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<StringDoubleMapTag  > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<StringStringMapTag  > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<StringBoolMapTag    > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<BoolIntegerMapTag   > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<BoolDoubleMapTag    > );
      sc.tag_factory_keeper_w().add( new MapTagFactory<BoolStringMapTag    > );
	  
	  sc.simulation_task_keeper_w().add( new SimulationTaskSetGlobalTagInteger );
	  sc.simulation_task_keeper_w().add( new SimulationTaskSetGlobalTagDouble );
	  sc.simulation_task_keeper_w().add( new SimulationTaskSetGlobalTagString );
	  sc.simulation_task_keeper_w().add( new SimulationTaskSetGlobalTagBool );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/taggings_init.cpp,v $
 * Version $Revision: 649 $
 * Date    $Date: 2012-07-09 17:30:43 +0200 (Mon, 09 Jul 2012) $
 *-----------------------------------------------------------------------
 * $Log: taggings_init.cpp,v $
 *-----------------------------------------------------------------------*/
