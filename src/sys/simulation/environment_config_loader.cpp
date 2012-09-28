/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/simulation/environment_config_loader.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn::xml;

namespace shawn
{

    // ----------------------------------------------------------------------
    EnvironmentConfigLoader::
    	EnvironmentConfigLoader(SimulationController& sc) 
        : parsing_state_(Unknown), 
          sim_controller_(sc)
    {
		set_tag_factory_keeper( sc.tag_factory_keeper_w() );
	} 

    // ----------------------------------------------------------------------
    EnvironmentConfigLoader::
    	~EnvironmentConfigLoader()  
    {} 

    // ----------------------------------------------------------------------
    void 
    	EnvironmentConfigLoader::
    	skip_target_reached(string, AttList& atts) 
    {}

    // ----------------------------------------------------------------------
    void 
    	EnvironmentConfigLoader::
    	handle_start_element(string name, AttList& atts) 
        throw(runtime_error)
    {
        SAXSkipReader::handle_start_element(name, atts);

        if( !skipping() )
            parse_xml(name, atts, true);
    }

    // ----------------------------------------------------------------------
    void 
    	EnvironmentConfigLoader::
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
    	EnvironmentConfigLoader::
    	parse_xml(string name, AttList& atts, bool opening_tag) 
    	throw(runtime_error) 
    {
        if( parsing_state_ == Done )
            return;

        if("scenario" == name && parsing_state_ == Unknown) 
        {
            opening_tag? parsing_state_ = Scenario : parsing_state_ = Done;
        }
        else if("environment" == name && (parsing_state_ == Scenario || parsing_state_ == Environment) ) 
        {
            opening_tag ? parsing_state_ = Environment : parsing_state_ = Done;
        }
        else if("tag" == name && parsing_state_ == Environment) 
		{
			if(opening_tag)
				handle_open_tag_tag(atts, sim_controller_.environment_w());
			else
				handle_close_tag_tag(atts, sim_controller_.environment_w());
		}
        else if("entry" == name && parsing_state_ == Environment && opening_tag) 
        {
            handle_tag_entry(atts);
        }

    }

}


/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/simulation/environment_config_loader.cpp,v $
* Version $Revision: 189 $
* Date    $Date: 2008-04-24 14:14:34 +0200 (Thu, 24 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: environment_config_loader.cpp,v $
 *-----------------------------------------------------------------------*/
