/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_WORLDS_FLEGSENS_WORLD_FACTORY_H
#define __SHAWN_SYS_WORLDS_FLEGSENS_WORLD_FACTORY_H

#include "sys/worlds/processor_world_factory.h"
#include "sys/xml/sax_reader.h"
#include "sys/xml/tag_parser.h"
#include "sys/tag.h"
#include "sys/taggings/tag_factory.h"
#include "sys/world.h"

//#include <expat.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <set>
#include <exception>
#include <string>
#include <map>

namespace shawn 
{
	class TagFactoryKeeper;
	class SimulationController;

	///Creates nodes in a world by reading node information from an XML File.
	/** Currently honors location and tag information of nodes and reads the tags for the
	* world. The XML file is NOT checked for errors or schema conformance. Only one snapshot 
	* is currently read from the file. Set the desired snapshot time using set_snapshot_id().
	*
	*
	*	Expected call in .conf File:
	*	load_world file=Filename.xml processors=Processortype snapshot=0
	*/
	class FlegsensWorldFactory : 
		public shawn::xml::SAXSimpleSkipReader, 
		public ProcessorWorldFactory,
		public shawn::xml::TagParser
	{

	public:
		///The current parsing state
		enum ParsingState 
		{ 
			Unknown,    ///< No expected tag has been encoutered yet
			Scenario,   ///< Inside a scenario tag
			Snapshot,   ///< Inside a snapshot tag
			Node,       ///< Inside a node tag
			World,      ///< Inside a world tag
			KiSeq,		///< Inside a tag tag named "ki_seq"
			Done        ///< Parsing has stopped
		};            

		///@name Construction / Destruction
		///@{
		FlegsensWorldFactory();
		virtual ~FlegsensWorldFactory();
		///@}

		void set_parameters(int in_x_count, int in_y_count, double in_x_dist, double in_y_dist, double in_x_off,
				int in_gps_row1[], int in_gps_row2[], int in_gps_row3[], int in_gps_row4[],
				int in_gateway_row1[], int in_gateway_row2[], int in_gateway_row3[], int in_gateway_row4[]);
		///@name Configuration and action triggering
		///@{

		/** Set the snapshot id which will be used when reading the file */
		void set_snapshot_id( const std::string ) throw();

		/** Actual parsing is triggered here */
		virtual void fill_world( shawn::World& ) throw();

		///@}

	protected:

		/// Callback handler for Expat opening tag events. Real handling is delegeated to parse_xml().
		virtual void handle_start_element(std::string name, shawn::xml::AttList& atts) throw(std::runtime_error);

		/// Callback handler for Expat closing tag events. Real handling is delegeated to parse_xml().
		virtual void handle_end_element(std::string name) throw(std::runtime_error);

		///Keeps the current parsing state and extracts the relevant information from the XML file.
		/** Creates Nodes in the world (Location and Tags are currently read). Also tags for the shwan::World are
		* read and attached to the world. Only called if not in skipping mode.
		*/
		void parse_xml(std::string name, shawn::xml::AttList& atts, bool opening_tag) throw(std::runtime_error);        

		///Handles &lt;scenario&gt; tags
		void handle_tag_scenario(shawn::xml::AttList& atts, bool opening_tag);        
		///Handles &lt;world&gt; tags
		void handle_tag_world(shawn::xml::AttList& atts, bool opening_tag);        
		///Handles &lt;snapshot&gt; tags
		void handle_tag_snapshot(shawn::xml::AttList& atts, bool opening_tag);        
		///Handles &lt;node&gt; tags
		void handle_tag_node(shawn::xml::AttList& atts, bool opening_tag);        
		///Handles &lt;location&gt; tags
		void handle_tag_location(shawn::xml::AttList& atts, bool opening_tag);        
		///Handles &lt;sizehint&gt; tags
		void handle_tag_sizehint(shawn::xml::AttList& atts, bool opening_tag);        

		/** Callback handler from SAXSkipReader. Invoked if the target tag in the XML file has been reached. */
		virtual void skip_target_reached(std::string name, shawn::xml::AttList& atts);

		virtual void add_node(shawn::Node*); //add nodes to world - can be rimplemented in subclasses in order to trigger the addition of nodes
		
		void create_xml_file();

	protected:
		int node_count_;						///< The number of created nodes
		shawn::World* world_;					///< Reference to the world in which we create nodes
		ParsingState parsing_state_;			///< The current parsing state
						
		SimulationController* sim_controller_;	///< Pointer to the simulation controller
		
		xml::XMLObj flegsens_xml_world;
	private:	
		shawn::Node* current_node_;///< The node that is currently filled with data
		
		
		int x_count_; // Anzahl Sensorknoten auf der x-Achse
		int y_count_;	// Anzahl Sensorknoten auf der y-Achse
		double x_dist_;  // Abstand zwischen zwei Knoten auf der x-Achse in [m]
		double y_dist_;  // Abstand zwischen zwei Knoten auf der y-Achse in [m] 
		double x_off_;   // Offset zwischen zwei Reihen auf der x-Achse in [m]
		int gps_row1_[5];
		int gps_row2_[5];
		int gps_row3_[5];
		int gps_row4_[5];
		int gateway_row1_[5];
		int gateway_row2_[5];
		int gateway_row3_[5];
		int gateway_row4_[5];
		
		
	};

}

#endif

