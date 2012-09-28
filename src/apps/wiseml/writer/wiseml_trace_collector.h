/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_TRACE_COLLECTOR_H
#define __SHAWN_APPS_WISEML_TRACE_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_collector.h"
#include "apps/wiseml/templates.h"
#include "sys/simulation/simulation_controller.h"
using namespace shawn;

namespace wiseml
{
   /**
    * SetupCollector gathers data for the setup section of a wiseml file.
    */
   class WisemlTraceCollector
      : public WisemlDataCollector
   {
   public:
      WisemlTraceCollector(shawn::SimulationController &sc, std::string id);
      virtual ~WisemlTraceCollector();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      virtual double next_timestamp_after(double time);
      virtual void node_movement(std::string node);
      virtual void capability_value(std::string node, 
         std::string capability, std::string value);
      virtual void rssi(std::string src, std::string dst, std::string value);

      virtual std::string generate_xml() const;
      virtual void clear();
   protected:
      virtual std::string generate_timestamp_xml(double timestamp) const;
   };
   DECLARE_HANDLES(WisemlTraceCollector);
}

#endif
#endif
