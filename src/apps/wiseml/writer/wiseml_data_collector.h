/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_DATA_COLLECTOR_H
#define __SHAWN_APPS_WISEML_DATA_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "sys/util/keeper_managed.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
#include <list>
using namespace shawn;

namespace wiseml
{
   /**
    * Base class for all DataCollectors used to gather all data to
    * generate a WiseML file.
    */
   class WisemlDataCollector
      : public KeeperManaged
   {
   public:
      WisemlDataCollector(shawn::SimulationController &sc, std::string id);
      virtual ~WisemlDataCollector();

      virtual double next_timestamp_after(double time) = 0;
      virtual std::string generate_xml() const = 0;
      virtual void clear() = 0;
   protected:
      shawn::SimulationController &sc_;
      std::multimap<double,std::string> items_;
      std::string id_;
      double current_time();
      std::list<std::string> make_list(std::string str) const; 
   };
   DECLARE_HANDLES(WisemlDataCollector);
}

#endif
#endif
