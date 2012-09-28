/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_TASKS_SELECT_NODE_H
#define __SHAWN_APPS_LOCALIZATION_TASKS_SELECT_NODE_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "sys/simulation/simulation_task.h"


namespace localization
{

   class SimulationTaskLocalizationSelectNode
      : public shawn::SimulationTask
   {

   public:

      SimulationTaskLocalizationSelectNode();
      virtual ~SimulationTaskLocalizationSelectNode();
      
      virtual void run( shawn::SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/tasks/localization_select_node.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_select_node.h,v $
 *-----------------------------------------------------------------------*/
