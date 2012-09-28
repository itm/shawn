/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/worlds/load_world_factory.h"
#include "sys/world.h"
#include "sys/vec.h"
#include "sys/util/string_conv.h"
#include "sys/comm_models/manual_communication_model.h"
#include "sys/node_movements/no_movement.h"
#include "sys/taggings/tag_factory.h"
#include "sys/taggings/tag_factory_keeper.h"
#include "sys/simulation/simulation_controller.h"

//TODO: Still necessary? Included these files for KiSeq-Loading
#include "sys/util/string_conv.h"
#include "sys/comm_models/rim_comm_model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace shawn::xml;

namespace shawn
{

    // ----------------------------------------------------------------------
    LoadWorldFactory::
    	LoadWorldFactory() 
        : node_count_(0),
          world_(NULL),
          parsing_state_(Unknown),
          current_node_(NULL)
    {} 

    // ----------------------------------------------------------------------
    void 
    	LoadWorldFactory::
    	fill_world( shawn::World& w ) 
    	throw()
    {
        world_ = &w;
        sim_controller_ = &w.simulation_controller_w();
		  set_tag_factory_keeper( w.simulation_controller_w().tag_factory_keeper_w() );

        parse();

        sim_controller_ = NULL;
        world_ = NULL;
    }

    // ----------------------------------------------------------------------
    LoadWorldFactory::
    	~LoadWorldFactory()  
    {} 

    // ----------------------------------------------------------------------
    void 
    	LoadWorldFactory::
    	skip_target_reached(std::string, AttList& atts) 
    {
        cerr << "Load World: Snapshot found: " << attribute("id", atts) << endl;
    }

    // ----------------------------------------------------------------------
    void 
    	LoadWorldFactory::
    	handle_start_element(string name, AttList& atts) 
        throw(runtime_error)
    {
        SAXSkipReader::handle_start_element(name, atts);
        
		if( !skipping() )
            parse_xml(name, atts, true);
    }

    // ----------------------------------------------------------------------
    void 
    	LoadWorldFactory::
    	handle_end_element(string name) 
    	throw(runtime_error)
    {
        SAXSkipReader::handle_end_element(name);
        AttList a;

        if( !skipping() )
            parse_xml(name, a, false);
    }


    // ----------------------------------------------------------------------
    inline void 
    	LoadWorldFactory::
    	parse_xml(string name, AttList& atts, bool opening_tag) 
    	throw(runtime_error) 
    {
        if( parsing_state_ == Done )
            return;

        if("node" == name) 
            handle_tag_node(atts, opening_tag);
        else if("location" == name && opening_tag && parsing_state_ == Node) 
            handle_tag_location(atts, opening_tag);
        else if("snapshot" == name) 
            handle_tag_snapshot(atts, opening_tag);
        else if("world" == name)
            handle_tag_world(atts, opening_tag);
        else if("sizehint" == name && opening_tag ) 
            handle_tag_sizehint(atts, opening_tag);
        else if("scenario" == name) 
            handle_tag_scenario(atts, opening_tag);
        else if("entry" == name && opening_tag) 
				handle_tag_entry(atts);
        else if("tag" == name) 
		{
			TagContainer* tc = world_;
			if(parsing_state_ == Node )
				tc = current_node_;

			if(opening_tag)
				handle_open_tag_tag(atts, *tc);
			else
				handle_close_tag_tag(atts, *tc);
		}
    }

    // ----------------------------------------------------------------------
    inline void 
        LoadWorldFactory::
        handle_tag_scenario(AttList& atts, bool opening_tag)
    {
        opening_tag? parsing_state_ = Scenario : parsing_state_ = Done;
    }

    // ----------------------------------------------------------------------
    inline void 
        LoadWorldFactory::
        handle_tag_world(AttList& atts, bool opening_tag)    
    {
        opening_tag ? parsing_state_ = World : parsing_state_ = Snapshot;
    }

    // ----------------------------------------------------------------------
    inline void 
        LoadWorldFactory::
        handle_tag_snapshot(AttList& atts, bool opening_tag)    
    {
        if (opening_tag) 
            parsing_state_ = Snapshot;
        else
        {
            interrupt();
            parsing_state_ = Done; //One snapshot loaded
            cerr << "Load World: Done." << endl;
        }
    }

    // ----------------------------------------------------------------------
    inline void 
        LoadWorldFactory::
        handle_tag_node(AttList& atts, bool opening_tag)        
    {
        if( opening_tag ) 
        {
            parsing_state_ = Node;
            current_node_ = new_node(Vec(0,0,0));//new Node();
            current_node_->set_label(attribute("id", atts));
        } 
        else 
        {
			add_node( current_node_);
            parsing_state_ = Snapshot;
            node_count_++;

            if( node_count_ % 5000 == 0 )
                cerr << "  Node count: " << node_count_ << " so far" << endl;   
        }
    }

    // ----------------------------------------------------------------------
	void 
        LoadWorldFactory::
		add_node(shawn::Node* n)
	{
		//n=current_node_
		world_->add_node( *n );
	}
	// ----------------------------------------------------------------------

    inline void 
        LoadWorldFactory::
        handle_tag_location(AttList& atts, bool opening_tag)        
    {
        if(!opening_tag)
            return;

        double x, y, z;
        string tmp;

        tmp = attribute("x", atts);
        x = tmp != "" ? atof(tmp.c_str()) : 0.0;

        tmp = attribute("y", atts);
        y = tmp != "" ? atof(tmp.c_str()) : 0.0;

        tmp = attribute("z", atts);
        z = tmp != "" ? atof(tmp.c_str()) : 0.0;

        Vec loc(x,y,z);
        current_node_->set_real_position(loc);
    }

    // ----------------------------------------------------------------------
    inline void 
        LoadWorldFactory::
        handle_tag_sizehint(AttList& atts, bool opening_tag)       
    {
        if(!opening_tag)
            return;

        world_->set_size_hint( conv_string_to_double( attribute("value", atts) ));
    }

   

    // ----------------------------------------------------------------------
    void 
    	LoadWorldFactory::
    	set_snapshot_id( const std::string tim ) 
    	throw()
    {
        cerr << "Load World: Loading snapshot: " << tim << endl;
        set_skip_target("snapshot");
        set_skip_target_add_attr("id", tim);
    }

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/worlds/load_world_factory.cpp,v $
* Version $Revision: 391 $
* Date    $Date: 2010-03-16 14:43:20 +0100 (Tue, 16 Mar 2010) $
*-----------------------------------------------------------------------
* $Log: load_world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
