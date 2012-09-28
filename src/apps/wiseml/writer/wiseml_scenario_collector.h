/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SCENARIO_COLLECTOR_H
#define __SHAWN_APPS_WISEML_SCENARIO_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_collector.h"
#include "apps/wiseml/templates.h"
#include "sys/simulation/simulation_controller.h"
#include <iostream>
using namespace shawn;

namespace wiseml
{
   /**
    * SetupCollector gathers data for the setup section of a wiseml file.
    */
   class WisemlScenarioCollector
      : public WisemlDataCollector
   {
   public:
   ///@name Constructor/Destructor
   ///@{
      WisemlScenarioCollector(shawn::SimulationController &sc, std::string id);
      virtual ~WisemlScenarioCollector();
   ///@}

   ///@name KeeperManaged implementations
   ///@{
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   ///@}

   ///@name WisemlDataCollector implementations
   ///@{
      virtual double next_timestamp_after(double time);
   ///@}

   ///@name Data collection methods
   ///@{
      virtual void enable_node(std::string node);
      virtual void disable_node(std::string node);
      virtual void enable_link(std::string src, std::string dst);
      virtual void disable_link(std::string src, std::string dst);
      virtual void node_movement(std::string node);
      virtual void capability_value(std::string node, 
         std::string capability, std::string value);
   ///@}

      virtual std::string generate_xml() const;
   protected:
      virtual std::string generate_timestamp_xml(double timestamp) const;
      virtual void clear();
   };
   DECLARE_HANDLES(WisemlScenarioCollector);
}

#endif
#endif
