/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_apps_WISEML_XML_WISEML_TIMESTAMPS_H
#define __SHAWN_apps_WISEML_XML_WISEML_TIMESTAMPS_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML
#include "sys/xml/sax_reader.h"
#include "sys/xml/tag_parser.h"
#include "sys/tag.h"
#include "sys/taggings/tag_factory.h"
#include "sys/world.h"
#include "sys/event_scheduler.h"
#include "apps/wiseml/templates.h"
#include "apps/wiseml/sensors/wiseml_sensor.h"
#include "apps/wiseml/sensors/wiseml_sensor_updater.h"

using namespace std;
using namespace shawn;
using namespace shawn::xml;
namespace wiseml
{
   enum TSLoaderState
   {
      TSL_NONE,
      TSL_SID,          // Search for scenario
      TSL_STIMESTAMP,   // Search scenario timestamp
      TSL_SCENARIO,     // Handle scenario contents
      TSL_TID,          // Search for trace
      TSL_TTIMESTAMP,   // Search trace timestamp
      TSL_TRACE,        // Handle trace contents
      TSL_INTERRUPT     // Exit
   };
   /**
    * Handles timestamps inside the trace and scenario sections of a wiseml file.
    * It works the following way:
    * - 1st called at simulation time 0.0 automatically
    * - Search for current timestamp inside the trace section
    * -- If found, update node positions, links, etc. until reaching a new timestamp
    *    or trace end; next timestamp gets saved
    * - Search for current timestamp in scenario section (as it overwrites the tracing info)
    * -- If found, update node positions, links, etc. until reaching a new timestamp
    *    or scenario end; next timestamp gets saved
    * - Update node positions
    * - Move event to next timestamp
    */
   class WiseMlTimestamps:
      public EventScheduler::EventHandler,
      public SAXSimpleSkipReader,
      public WisemlSensorUpdater
   {
   public:
      WiseMlTimestamps(string filename, string scenario_id, string trace_id, shawn::World &w);
      virtual ~WiseMlTimestamps();
      virtual void reset() throw();

      virtual void set_timefactor(double t);
   protected:
      // Basic XML reader functions
      virtual void skip_target_reached(std::string, AttList& atts);
      virtual void handle_start_element(std::string name, AttList& atts)
              throw(std::runtime_error);
      virtual void handle_end_element(std::string name)
              throw(std::runtime_error);
      virtual void handle_text_element(std::string content)
              throw(std::runtime_error);

      // XML handler for trace section
      // Handles text element when searching for the current timestamp
      virtual void handle_ttimestamp_text(string content);
      // Handles text elements while reading out the contents of the current timestamp
      virtual void handle_trace_text(string content);
      // Handles start elements while reading out the contents of the current timestamp
      virtual void handle_trace_start(std::string name, AttList& atts);

      // XML handler for scenario section
      // Handles text element when searching for the current timestamp
      virtual void handle_stimestamp_text(string content);
      // Handles text elements while reading out the contents of the current timestamp
      virtual void handle_scenario_text(string content);
      // Handles start elements while reading out the contents of the current timestamp
      virtual void handle_scenario_start(std::string name, AttList& atts);

      // EventHandler::timeout
      virtual void timeout( EventScheduler&, EventScheduler::EventHandle, double, EventScheduler::EventTagHandle& ) throw();

      // Iterates through all nodes which are to be updated in the current timestamp
      // and updates them.
      void update_node_positions();

      shawn::World &world_;
      WmlList elements_;   // Current XML element stack
      int elem_count_;     // Number of elements currently inside the XML element stack
      double time_;        // Current timestamp
      double next_timestamp_; // Next timestamp
      double timefactor_;
      string scenario_id_; // Scenario ID
      string trace_id_;    // Trace ID
      bool scenario_finished_;   // True if last timestamp reached in scenario section
      bool trace_finished_;      // True if last timestamp reached in trace section
      TSLoaderState state_;// Current Loader state
      map<string, NodeTemplate*> nodes_;  // Nodes to be updated
   };




}
#endif
#endif
