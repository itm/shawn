/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_WISEML_GENERATOR_H
#define __SHAWN_APPS_WISEML_GENERATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "shawn_config.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/world.h"
using namespace shawn;
namespace wiseml
{
   /** \brief Generates WiseML strings
    * A class for generating strings with WiseML structures, representing
    * the current network topology. This class is used internally by the
    * WisemlWriterTask simulation task.
    */
   class WisemlGenerator
   {
   public:
      /// Constructor/Destructor
      WisemlGenerator( SimulationController & );
      virtual ~WisemlGenerator();

      /// String generators
      /// Returns a complete WiseML file structure
      virtual std::string generate_wiseml( bool use_tabs=true );
      /// Returns only a scenario timestamp TODO
      //virtual std::string generate_scenario_timestamp();
      /// Returns only a trace timestamp TODO
      //virtual std::string generate_trace_timestamp();
      /// Appends a scenario timestamp TODO
      //virtual std::string append_scenario_timestamp( std::string );
      /// Appends a trace timestamp TODO
      //virtual std::string append_trace_timestamp( std::string );
   protected:
      /// Partial generators
      virtual std::string generate_setup();
      virtual std::string generate_origin();
      virtual std::string generate_timeinfo();
      virtual std::string generate_timestring();
      virtual std::string generate_defaults();
      virtual std::string generate_node_inst();
      virtual std::string generate_link_inst();
      /// World access
      const World& world() const;
      World& world_w();
      /// Tab indention
      void add_tab();
      void remove_tab();
      std::string tabs_;
      bool use_tabs_;
   private:
      SimulationController &ctrl_;
      
   };
}
#endif
#endif
