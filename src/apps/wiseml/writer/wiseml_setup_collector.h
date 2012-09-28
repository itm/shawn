/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_SETUP_COLLECTOR_H
#define __SHAWN_APPS_WISEML_SETUP_COLLECTOR_H

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_WISEML
#include "apps/wiseml/writer/wiseml_data_collector.h"
#include "apps/wiseml/templates.h"
#include "sys/simulation/simulation_controller.h"
#include <time.h>
using namespace shawn;

namespace wiseml
{
   /**
    * SetupCollector gathers data for the setup section of a wiseml file.
    */
   class WisemlSetupCollector
      : public WisemlDataCollector
   {
   public:
      WisemlSetupCollector(shawn::SimulationController &sc);
      virtual ~WisemlSetupCollector();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
      
      virtual double next_timestamp_after(double time);

      virtual void set_origin(shawn::Vec pos, double phi, double theta);

      virtual void set_timeinfo_start(std::string start);
      virtual void set_timeinfo_end(std::string end);
      virtual void set_timeinfo_duration(double rounds);
      virtual void set_timeinfo_factor(double factor);
      virtual void set_timeinfo_unit(std::string unit);

      virtual void set_interpolation(std::string mode);
      virtual void set_description(std::string desc);

      virtual void set_node_defaults(NodeTemplate &defaults);
      virtual void set_default_node_capability(Capability &cap);
      virtual void set_link_defaults(LinkInfo &defaults);
      virtual void set_default_link_capability(Capability &cap);

      virtual void set_bool_param(std::string param, bool value, std::string node);
      virtual void set_bool_param(std::string param, bool value, std::string src, std::string tgt);
      virtual void set_string_param(std::string param, std::string value, std::string node);

      virtual void gather_topology();
      virtual void add_capability(std::string node, Capability &cap);
      virtual void add_capability(std::string src, std::string tgt, Capability &cap);
      virtual void add_node(NodeTemplate &node);
      virtual void add_link(LinkInfo &link);

      virtual std::string generate_xml() const;
      virtual void clear();
      std::string generate_timestring() const;
   protected:
      NodeTemplate* find_node(std::string node);
      LinkInfo* find_link(std::string src, std::string tgt);
      // <Origin>
      shawn::Vec origin_position_;
      double origin_phi_, origin_theta_;
      // <Timeinfo>
      std::string start_, end_;
      double duration_start_;
      double duration_;
      double duration_factor_;
      std::string unit_;
      // General information
      std::string interpolation_;
      std::string desc_;
      // Node data
      NodeTemplate node_defaults_;
      std::list<NodeTemplate> nodes_;
      // Link data
      LinkInfo link_defaults_;
      std::list<LinkInfo> links_;
   };
   DECLARE_HANDLES(WisemlSetupCollector);
}

#endif
#endif
