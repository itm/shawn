/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/worlds/flegsens_world_factory.h"
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
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace shawn::xml;

namespace shawn {

// ----------------------------------------------------------------------
FlegsensWorldFactory::FlegsensWorldFactory() :
	node_count_(0), world_(NULL), parsing_state_(Unknown), flegsens_xml_world(), current_node_(NULL) {
}

void FlegsensWorldFactory::set_parameters(int in_x_count, int in_y_count, double in_x_dist, double in_y_dist, double in_x_off,
		int in_gps_row1[], int in_gps_row2[], int in_gps_row3[], int in_gps_row4[],
		int in_gateway_row1[], int in_gateway_row2[], int in_gateway_row3[], int in_gateway_row4[]) {
	x_count_ = max(1,abs(in_x_count));	// Anzahl Sensorknoten auf der x-Achse
	y_count_ = max(1,abs(in_y_count));	// Anzahl Sensorknoten auf der y-Achse
	x_dist_ = abs(in_x_dist);  	// Abstand zwischen zwei Knoten auf der x-Achse in [m]
	y_dist_ = abs(in_y_dist);  	// Abstand zwischen zwei Knoten auf der y-Achse in [m]
	x_off_ = abs(in_x_off);   	// Offset zwischen zwei Reihen auf der x-Achse in [m]

	int length = sizeof(int)*5;

	memcpy(gps_row1_, in_gps_row1, length);
	memcpy(gps_row2_, in_gps_row2, length);
	memcpy(gps_row3_, in_gps_row3, length);
	memcpy(gps_row4_, in_gps_row4, length);

	memcpy(gateway_row1_, in_gateway_row1, length);
	memcpy(gateway_row2_, in_gateway_row2, length);
	memcpy(gateway_row3_, in_gateway_row3, length);
	memcpy(gateway_row4_, in_gateway_row4, length);
	/*
	cout << "GPS1: " << gps_row1_[0] << " " << gps_row1_[1] << " " << gps_row1_[2] << " " << gps_row1_[3] << " " << gps_row1_[4] << endl;
	cout << "GPS2: " << gps_row2_[0] << " " << gps_row2_[1] << " " << gps_row2_[2] << " " << gps_row2_[3] << " " << gps_row2_[4] << endl;
	cout << "GPS3: " << gps_row3_[0] << " " << gps_row3_[1] << " " << gps_row3_[2] << " " << gps_row3_[3] << " " << gps_row3_[4] << endl;
	cout << "GPS4: " << gps_row4_[0] << " " << gps_row4_[1] << " " << gps_row4_[2] << " " << gps_row4_[3] << " " << gps_row4_[4] << endl;

	cout << "Gateway1: " << gateway_row1_[0] << " " << gateway_row1_[1] << " " << gateway_row1_[2] << " " << gateway_row1_[3] << " " << gateway_row1_[4] << endl;
	cout << "Gateway2: " << gateway_row2_[0] << " " << gateway_row2_[1] << " " << gateway_row2_[2] << " " << gateway_row2_[3] << " " << gateway_row2_[4] << endl;
	cout << "Gateway3: " << gateway_row3_[0] << " " << gateway_row3_[1] << " " << gateway_row3_[2] << " " << gateway_row3_[3] << " " << gateway_row3_[4] << endl;
	cout << "Gateway4: " << gateway_row4_[0] << " " << gateway_row4_[1] << " " << gateway_row4_[2] << " " << gateway_row4_[3] << " " << gateway_row4_[4] << endl;
	 */
}


// ----------------------------------------------------------------------
void FlegsensWorldFactory::fill_world(shawn::World& w) throw() {
	world_ = &w;
	sim_controller_ = &w.simulation_controller_w();
	set_tag_factory_keeper(w.simulation_controller_w().tag_factory_keeper_w() );

	// {Filling the FlegSens World}
	char text_buf[100];

	double x_orig = 100.0;
	double y_orig = 100.0;

	int ext_rotate_alpha_d = w.simulation_controller_w().environment_w().optional_int_param("flegsens_world_rotation", 0);

	while (ext_rotate_alpha_d>=360) {
		ext_rotate_alpha_d = ext_rotate_alpha_d - 360;
	}
	while (ext_rotate_alpha_d < 0) {
		ext_rotate_alpha_d = 360+ext_rotate_alpha_d;
	}
	cout << "Rotating the world by " << ext_rotate_alpha_d << " degrees. " << endl;

	// ext_rotate_alpha_d = max(min(ext_rotate_alpha_d,90),0);
	double pi = 3.1415926535897932384626433832795;

	double rotate_alpha = (((double)ext_rotate_alpha_d)/90)*(pi/2);

	double x = 0.0;
	double y = 0.0;
	double x2=0.0;
	double y2=0.0;
	double x_center = (((x_count_-1)*x_dist_)/2);
	double y_center = (((y_count_-1)*y_dist_)/2);
	/*
	cout << "ext_rotate_alpha_d" << ext_rotate_alpha_d << endl;
	cout << "rotate_alpha" << rotate_alpha << endl;
	cout << "x" << x << endl;
	cout << "y" << x << endl;
	cout << "x_center" << x_center << endl;
	cout << "y_center" << y_center << endl;
	 */

	double z = 0.0;
	bool is_anchor = false;
	bool is_gateway = false;
	double min_x = 0.0;
	double min_y = 0.0;


	// Create x and y position of all nodes
	for (int j=0; j<y_count_; j++) {
		for (int i=0; i<x_count_; i++) {
			x = (i*x_dist_) + (x_off_*(j%2));
			y = (j*y_dist_);
			z = 0.0;
			if (ext_rotate_alpha_d!=0) {
				// Rotate the coordinate_system
				x = x - x_center;
				y = y - y_center;
				x2 = (x*cos(rotate_alpha))-(y*sin(rotate_alpha));
				y2 = (x*sin(rotate_alpha))+(y*cos(rotate_alpha));
				x = x2 + x_center;
				y = y2 + y_center;
			}
			min_x = min(min_x, x);
			min_y = min(min_y, y);
		}
	}



	x = 0.0;
	y = 0.0;
	x2=0.0;
	y2=0.0;

	flegsens_xml_world.appendLine("<scenario>");
	flegsens_xml_world.appendLine("<snapshot id=\"0\">");

	int gps_row_data[5];
	int gateway_row_data[5];
	int emtpy_row_data[5] = {-1,500,500,0,0};

	int length = sizeof(int)*5;


	//cout << "force_last_node_anchor = " << force_last_node_anchor << endl;
	// Create x and y position of all nodes
	for (int j=0; j<y_count_; j++) {
		if (j==gps_row1_[0]) {
			memcpy(gps_row_data, gps_row1_ ,length);
		} else if (j==gps_row2_[0]) {
			memcpy(gps_row_data, gps_row2_ ,length);
		} else if (j==gps_row3_[0]) {
			memcpy(gps_row_data, gps_row3_ ,length);
		} else if (j==gps_row4_[0]) {
			memcpy(gps_row_data, gps_row4_ ,length);
		} else {
			memcpy(gps_row_data, emtpy_row_data ,length);
		}

		if (j==gateway_row1_[0]) {
			memcpy(gateway_row_data, gateway_row1_ ,length);
		} else if (j==gateway_row2_[0]) {
			memcpy(gateway_row_data, gateway_row2_ ,length);
		} else if (j==gateway_row3_[0]) {
			memcpy(gateway_row_data, gateway_row3_ ,length);
		} else if (j==gateway_row4_[0]) {
			memcpy(gateway_row_data, gateway_row4_ ,length);
		} else {
			memcpy(gateway_row_data, emtpy_row_data ,length);
		}

		for (int i=0; i<x_count_; i++) {
			x = (i*x_dist_) + (x_off_*(j%2));
			y = (j*y_dist_);
			z = 0.0;
			if (ext_rotate_alpha_d!=0) {
				// Rotate the coordinate_system
				x = x - x_center;
				y = y - y_center;
				x2 = (x*cos(rotate_alpha))-(y*sin(rotate_alpha));
				y2 = (x*sin(rotate_alpha))+(y*cos(rotate_alpha));
				x = x2 + x_center;
				y = y2 + y_center;
			}

			sprintf(text_buf, "<node id=\"node_%d-%d\">", i,j);
			flegsens_xml_world.appendLine(text_buf);

			x = x + x_orig - min_x;
			y = y + y_orig - min_y;

			sprintf(text_buf, "<location x=\"%f.2\" y=\"%f.2\" z=\"%f.2\" />", x,y,z);
			flegsens_xml_world.appendLine(text_buf);

			// GPS ########################################################################
			// Is this a row with gps?
			is_anchor = false;
			if (j==gps_row_data[0]) {
				if ( (gps_row_data[4]>0) && (i==(x_count_-1)) ) {
					// Force last
					is_anchor = true;
				} else if ( (gps_row_data[3]>0) && (i==0))  {
					// Force first
					is_anchor = true;
				} else {
					is_anchor = false;
				}
				int counter = gps_row_data[2]; //offset
				while (counter<=i) {
					if (counter==i) {
						is_anchor= true;
					}
					counter = counter + gps_row_data[1]; //interval
				}
			}


			// Gateway ########################################################################
			// Is this a row with gateways?
			is_gateway = false;
			if (j==gateway_row_data[0]) {
				if ( (gateway_row_data[4]>0) && (i==(x_count_-1)) ) {
					// Force last
					is_gateway = true;
				} else if ( (gateway_row_data[3]>0) && (i==0))  {
					// Force first
					is_gateway = true;
				} else {
					is_gateway = false;
				}
				int counter = gateway_row_data[2]; //offset
				while (counter<=i) {
					if (counter==i) {
						is_gateway= true;
					}
					counter = counter + gateway_row_data[1]; //interval
				}
			}

			// XML ########################################################################
			// Attach information
			if (is_anchor) {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"anchor\" value=\"true\"/>");
			} else {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"anchor\" value=\"false\"/>");
			}

			if (is_gateway) {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"gateway\" value=\"true\"/>");
			} else {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"gateway\" value=\"false\"/>");
			}

			/*
			if ((is_anchor)&&(is_gateway)) {
				cout << "Node " << j << " / "<< i << " is a Anchor and Gateway." << endl;
			} else if (is_anchor) {
				cout << "Node " << j << " / "<< i << " is a Anchor" << endl;
			} else if (is_gateway) {
				cout << "Node " << j << " / "<< i << " is a Gateway." << endl;
			}*/

			flegsens_xml_world.appendLine("</node>");
		}
	}
	flegsens_xml_world.appendLine("</snapshot>");
	flegsens_xml_world.appendLine("</scenario>");
	// END {Filling the FlegSens World}

	parse_Object(&flegsens_xml_world);
	sim_controller_ = NULL;
	world_ = NULL;
}

// ----------------------------------------------------------------------
FlegsensWorldFactory::~FlegsensWorldFactory() {
	// empty
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::skip_target_reached(std::string, AttList& atts) {
	cerr << "Load World: Snapshot found: " << attribute("id", atts) << endl;
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::handle_start_element(string name, AttList& atts)
		throw(runtime_error) {
	SAXSkipReader::handle_start_element(name, atts);

	if ( !skipping() )
		parse_xml(name, atts, true);
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::handle_end_element(string name) throw(runtime_error) {
	SAXSkipReader::handle_end_element(name);
	AttList a;

	if ( !skipping() )
		parse_xml(name, a, false);
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::parse_xml(string name, AttList& atts,
		bool opening_tag) throw(runtime_error) {
	if (parsing_state_ == Done)
		return;

	if ("node" == name)
		handle_tag_node(atts, opening_tag);
	else if ("location" == name && opening_tag && parsing_state_ == Node)
		handle_tag_location(atts, opening_tag);
	else if ("snapshot" == name)
		handle_tag_snapshot(atts, opening_tag);
	else if ("world" == name)
		handle_tag_world(atts, opening_tag);
	else if ("sizehint" == name && opening_tag)
		handle_tag_sizehint(atts, opening_tag);
	else if ("scenario" == name)
		handle_tag_scenario(atts, opening_tag);
	else if ("entry" == name && opening_tag)
		handle_tag_entry(atts);
	else if ("tag" == name) {
		TagContainer* tc = world_;
		if (parsing_state_ == Node)
			tc = current_node_;

		if (opening_tag)
			handle_open_tag_tag(atts, *tc);
		else
			handle_close_tag_tag(atts, *tc);
	}
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_scenario(AttList& atts,
		bool opening_tag) {
	opening_tag ? parsing_state_ = Scenario : parsing_state_ = Done;
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_world(AttList& atts,
		bool opening_tag) {
	opening_tag ? parsing_state_ = World : parsing_state_ = Snapshot;
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_snapshot(AttList& atts,
		bool opening_tag) {
	if (opening_tag)
		parsing_state_ = Snapshot;
	else {
		interrupt();
		parsing_state_ = Done; //One snapshot loaded
		cerr << "Load World: Done." << endl;
	}
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_node(AttList& atts,
		bool opening_tag) {
	if (opening_tag) {
		parsing_state_ = Node;
		current_node_ = new_node(Vec(0, 0, 0));//new Node();
		current_node_->set_label(attribute("id", atts));
	} else {
		add_node(current_node_);
		parsing_state_ = Snapshot;
		node_count_++;

		if (node_count_ % 5000 == 0)
			cerr << "  Node count: " << node_count_ << " so far" << endl;
	}
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::add_node(shawn::Node* n) {
	//n=current_node_
	world_->add_node( *n);
}
// ----------------------------------------------------------------------

inline void FlegsensWorldFactory::handle_tag_location(AttList& atts,
		bool opening_tag) {
	if (!opening_tag)
		return;

	double x, y, z;
	string tmp;

	tmp = attribute("x", atts);
	x = tmp != "" ? atof(tmp.c_str()) : 0.0;

	tmp = attribute("y", atts);
	y = tmp != "" ? atof(tmp.c_str()) : 0.0;

	tmp = attribute("z", atts);
	z = tmp != "" ? atof(tmp.c_str()) : 0.0;

	Vec loc(x, y, z);
	current_node_->set_real_position(loc);
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_sizehint(AttList& atts,
		bool opening_tag) {
	if (!opening_tag)
		return;

	world_->set_size_hint(conv_string_to_double(attribute("value", atts) ));
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::set_snapshot_id(const std::string tim) throw() {
	cerr << "Load World: Loading snapshot: " << tim << endl;
	set_skip_target("snapshot");
	set_skip_target_add_attr("id", tim);
}

// ----------------------------------------------------------------------

void FlegsensWorldFactory::create_xml_file() {
	cerr << "Creating World and Node XML Representation..." << endl;

}

// ----------------------------------------------------------------------


}
