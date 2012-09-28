/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_DATA_KEEPER_H
#define __SHAWN_APPS_WISEML_DATA_KEEPER_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include <map>
#include <string>
#include <stdexcept>

#include "shawn_config.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/handle_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/wiseml/writer/wiseml_data_collector.h"
#include "apps/wiseml/writer/wiseml_setup_collector.h"
#include "apps/wiseml/writer/wiseml_trace_collector.h"
#include "apps/wiseml/writer/wiseml_scenario_collector.h"
 
namespace wiseml
{
   ///  Keeper that keeps WisemlDataCollectors
   /**  
   */
   class WisemlDataKeeper
      : public shawn::HandleKeeper<WisemlDataCollector>
   {
   public:
      ///@name construction / destruction
      ///@{
      ///
      WisemlDataKeeper(shawn::SimulationController &sc);
      ///
      virtual ~WisemlDataKeeper();
      ///@}

      virtual WisemlTraceCollector* add_trace(std::string trace_id);
      virtual WisemlScenarioCollector* add_scenario(std::string trace_id);
      virtual WisemlSetupCollector& setup();
      virtual WisemlTraceCollector& trace(std::string id) throw(std::runtime_error);
      virtual WisemlScenarioCollector& scenario(std::string id) throw(std::runtime_error);
      virtual std::list<WisemlTraceCollector*> traces();
      virtual std::list<WisemlScenarioCollector*> scenarios();
      virtual double next_timestamp_after(double time);
      virtual std::string generate_xml();
   protected:
      shawn::SimulationController &sc_;
      WisemlSetupCollector *setup_;
   };
}

#endif
#endif

