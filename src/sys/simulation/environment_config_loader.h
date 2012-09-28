/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_SIMULATION_ENVIRONMENT_CONFIG_LOADER_H
#define __SHAWN_SYS_SIMULATION_ENVIRONMENT_CONFIG_LOADER_H

#include "shawn_config.h"
#include "sys/xml/sax_reader.h"
#include "sys/xml/tag_parser.h"
#include <exception>

namespace shawn 
{
	class SimulationController;

	/**
      *
      */
	class EnvironmentConfigLoader : 
		public shawn::xml::SAXSimpleSkipReader, 
		public shawn::xml::TagParser
	{

	public:
		///The current parsing state
		enum ParsingState 
		{ 
			Unknown,        ///< No expected tag has been encoutered yet
			Scenario,       ///< Inside a scenario tag
            Environment,    ///< Environment tag
            Done            ///< Parsing has stopped
		};            

		///@name Construction / Destruction
		///@{
		EnvironmentConfigLoader(SimulationController&);
		virtual ~EnvironmentConfigLoader();
		///@}

	protected:

		/// Callback handler for Expat opening tag events. Real handling is delegated to parse_xml().
		virtual void handle_start_element(std::string name, shawn::xml::AttList& atts) throw(std::runtime_error);

		/// Callback handler for Expat closing tag events. Real handling is delegated to parse_xml().
		virtual void handle_end_element(std::string name) throw(std::runtime_error);

		///Keeps the current parsing state and extracts the relevant information from the XML file.
		void parse_xml(std::string name, shawn::xml::AttList& atts, bool opening_tag) throw(std::runtime_error);        

		///Callback handler from SAXSkipReader. Invoked if the target tag in the XML file has been reached.
		virtual void skip_target_reached(std::string name, shawn::xml::AttList& atts);

	private:
		ParsingState parsing_state_;			///< The current parsing state
		SimulationController& sim_controller_;	///< Pointer to the simulation controller
	};

}

#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/simulation/environment_config_loader.h,v $
* Version $Revision: 189 $
* Date    $Date: 2008-04-24 14:14:34 +0200 (Thu, 24 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: environment_config_loader.h,v $
 *-----------------------------------------------------------------------*/
