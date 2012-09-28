/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/localization_init.h"
#include "apps/localization/localization_processor_factory.h"
#include "apps/localization/tasks/localization_evaluation.h"
#include "apps/localization/tasks/localization_select_node.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"


using namespace localization;

extern "C" void init_localization( shawn::SimulationController& sc )
{
   LocalizationProcessorFactory::register_factory( sc );
   sc.simulation_task_keeper_w().add( new SimulationTaskLocalizationEvaluation );
   sc.simulation_task_keeper_w().add( new SimulationTaskLocalizationSelectNode );
}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/localization_init.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_init.cpp,v $
 *-----------------------------------------------------------------------*/
