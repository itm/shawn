/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_apps_WISEML_WORLD_WISEMLWORLD_FACTORY_H
#define __SHAWN_apps_WISEML_WORLD_WISEMLWORLD_FACTORY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISEML

#include "sys/worlds/processor_world_factory.h"
#include "sys/xml/sax_reader.h"
#include "sys/xml/tag_parser.h"
#include "sys/tag.h"
#include "sys/taggings/tag_factory.h"
#include "sys/world.h"
#include "apps/wiseml/templates.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <set>
#include <exception>
#include <string>
#include <map>

using namespace std;
using namespace shawn;
using namespace shawn::xml;

namespace wiseml
{
    class TagFactoryKeeper;
    class SimulationController;

    enum LoaderState
    {
       WML_NONE,
       WML_WORLDINFO, // Gather world informations
       WML_DEFAULTSNODE, // Read default node properties
       WML_DEFAULTSLINK, // Read default link properties
       WML_TOPOLOGY, // Read node and link instances
       WML_INTERRUPT // Interrupting
    };

    /**
     * Reads out a WiseMl file and creates nodes and links.
     */
    class WiseMLWorldFactory:
       public SAXSimpleSkipReader,
       public shawn::ProcessorWorldFactory
    {
    public:
        WiseMLWorldFactory();
        ~WiseMLWorldFactory();

        virtual void fill_world(shawn::World&) throw();
        /// Set wiseml filename
        void set_filename(string filename);
        /// Set scenario ID
        void set_scenario_id(string filename);
        /// Set trace ID
        void set_trace_id(string filename);
        /// Set timefactor
        void set_timefactor(double t);

    protected:
       virtual void skip_target_reached(std::string, AttList& atts);
       virtual void handle_start_element(std::string name, AttList& atts)
                 throw(std::runtime_error);
       virtual void handle_end_element(std::string name)
                 throw(std::runtime_error);
       virtual void handle_text_element(std::string content)
                 throw(std::runtime_error);


       virtual void handle_start_node(std::string name, AttList& atts)
                 throw(std::runtime_error);
       virtual void handle_end_node(std::string name)
                 throw(std::runtime_error);
       virtual void handle_start_link(std::string name, AttList& atts)
                 throw(std::runtime_error);
       virtual void handle_end_link(std::string name)
                 throw(std::runtime_error);

       virtual void set_state(LoaderState state);

       World *world_;

       string filename_;
       /// Properties of the current node
       NodeTemplate new_node_;
       /// Properties of all nodes
       NodeTemplate def_node_;
       /// Current xml parent elements
       WmlList elements_;
       /// Number of parent elements
       int elem_count_;
       /// Current loader state
       LoaderState state_;

       string scenario_id_;
       string trace_id_;
       double timefactor_;
    };
}

#endif
#endif
