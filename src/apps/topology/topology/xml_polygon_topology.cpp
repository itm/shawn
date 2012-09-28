/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"

#include "sys/vec.h"

#include "apps/topology/topology/xml_polygon_topology.h"
#include "apps/topology/topology/polygon_topology_helpers.h"
#include "sys/util/string_conv.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/topology/polygon/segment_2d.h"

#include <iostream>
#include <iterator>
#include <vector>

using namespace shawn;
using namespace shawn::xml;
using namespace std;

namespace topology
{

	// ----------------------------------------------------------------------
	XMLPolygonTopology::
		XMLPolygonTopology()
		: parsing_state_(UnknownState),
		polygon_type_(HoleType),
		polygon_(NULL),
		outer_(NULL),
		polygon_count_(0),
		tag_count_(0),
		create_outer_polygon_(false),
		fix_non_simple_polygons_(false)
	{}

	// ----------------------------------------------------------------------
	XMLPolygonTopology::
		~XMLPolygonTopology()
	{}

	// ----------------------------------------------------------------------
	void
		XMLPolygonTopology::
		read(shawn::SimulationController& sc, const string& filename, bool create_outer_polygon /* = false */, bool fix_non_simple_polygons /* = false */)
		throw( runtime_error )
	{
		create_outer_polygon_ = create_outer_polygon;
		fix_non_simple_polygons_ = fix_non_simple_polygons;

		set_tag_factory_keeper( sc.tag_factory_keeper_w() );

		if( create_outer_polygon_ ) {
			outer_ = new Polygon;
			cerr << "XMLPolygonTopology: Will create outer convex hull polygon" << endl;
		}

		set_document_uri(filename);
		parse();

		if( create_outer_polygon_ )
		{
			Polygon& tmp = *new Polygon;
			tmp = outer_->compute_convex_hull();
			set_outer_polygon(tmp);
			
		}	
		cerr << "XMLPolygonTopology: Loaded " << polygon_count_ << " polygons and " << tag_count_ << " tags" << endl;    
	}

	// ----------------------------------------------------------------------
	void
		XMLPolygonTopology::
		handle_start_element(string name, AttList& atts) 
		throw(std::runtime_error)
	{
		if( parsing_state_ == UnknownState && "topology" == name )
		{
			parsing_state_ = TopologyState;
		}
		else if( parsing_state_ == TopologyState && "polygon" == name ) 
		{
			polygon_type_ = polygon_type(atts);
			parsing_state_ = PolygonState;
			polygon_ = new Polygon;
		} 
		else if( parsing_state_ == PolygonState && "vertex" == name ) 
		{
			assert( polygon_ != NULL );

			polygon_->push_back( to_point(atts) );

			if( create_outer_polygon_ && polygon_type_ != OuterType )
				outer_->push_back( to_point(atts) );
		} 
		else if( parsing_state_ == PolygonState && "tag" == name )
		{
			parsing_state_ = TagState;
			handle_open_tag_tag(atts, tags_w(*polygon_));
		}
		else if( parsing_state_ == TagState && "entry" == name )
		{
			handle_tag_entry(atts);
		}
	}

	// ----------------------------------------------------------------------
	void
		XMLPolygonTopology::
		handle_end_element(string name) 
		throw(std::runtime_error)
	{
		if( parsing_state_ == TopologyState && "topology" == name )
		{
			parsing_state_ = DoneState;
		}
		else if( parsing_state_ == PolygonState && "polygon" == name ) 
		{
			assert( polygon_ != NULL );
			parsing_state_ = TopologyState;
			polygon_count_++;

			//If fix_non_simple_polygons_ is set and the input polygon is not simple, 
			//a convex hull around the polygons vertices is created instead
			if( (!polygon_->is_simple()) && fix_non_simple_polygons_ ) 
			{
				cerr << "XMLPolygonTopology: Warning: Polygon is not simple, creating conex hull instead" << endl;    
				Polygon pnew = polygon_->compute_convex_hull(); // convex hull of the polygon
				delete polygon_;
				polygon_ = &pnew;
			}

			//Add the polygon to the list of polygons depending on the type specified in the 
			//opening 'polygon' xml tag before
			if( HoleType == polygon_type_ )
			{
				cerr << "XMLPolygonTopology: Info: Loaded hole polygon" << endl;    
				add_hole_polygon( *polygon_  );
			}
			else if( OuterType == polygon_type_ && !create_outer_polygon_ )
			{
				cerr << "XMLPolygonTopology: Info: Loaded outer polygon" << endl;    
				set_outer_polygon( *polygon_  );
			}

			polygon_ = NULL;
		}
		else if( parsing_state_ == TagState && "tag" == name )
		{
			AttList a;
			handle_close_tag_tag(a, tags_w(*polygon_));
			parsing_state_ = PolygonState;
			tag_count_++;
		}

	}

	// ----------------------------------------------------------------------
	Vec
		XMLPolygonTopology::
		to_point(AttList& atts) 
		const throw(std::runtime_error)
	{
		double x = conv_string_to_double( attribute("x", atts, "0.0") );
		double y = conv_string_to_double( attribute("y", atts, "0.0") );
		return Vec(x,y,0.0);
	}

	// ----------------------------------------------------------------------
	XMLPolygonTopology::PolygonType
		XMLPolygonTopology::
		polygon_type(AttList& atts) 
		throw(runtime_error)
	{       
		if( attribute("type", atts, "unknown") ==  "outer")
			return OuterType;

		if( attribute("type", atts, "unknown") == "hole")
			return HoleType;

		throw runtime_error("Polygon type unknown");
	}

}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology.cpp,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology.cpp,v $
*-----------------------------------------------------------------------*/
