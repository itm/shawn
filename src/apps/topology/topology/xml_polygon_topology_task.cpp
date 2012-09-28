/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/xml_polygon_topology_task.h"
#include "apps/topology/topology_keepers.h"
#include "sys/simulation/simulation_environment.h"
#include "apps/reading/readings/reading_keeper.h"

#include "shawn_config.h"
#include "apps/topology/topology/xml_polygon_topology.h"

using namespace shawn;
using namespace reading;


namespace topology
{

	// ----------------------------------------------------------------------
	XMLPolygonTopologyTask::
		XMLPolygonTopologyTask()
	{}

	// ----------------------------------------------------------------------
	XMLPolygonTopologyTask::
		~XMLPolygonTopologyTask()
	{}

	// ----------------------------------------------------------------------
	std::string
		XMLPolygonTopologyTask::
		name( void )
		const throw()
	{
		return "xml_polygon_topology";
	}

	// ----------------------------------------------------------------------
	std::string
		XMLPolygonTopologyTask::
		description( void )
		const throw()
	{
		return "Creates a 2D polygonal topology named $name from file $file. "
			"The optional parameter $create_outer_hull (or for compatibility $xml_create_outer_hull) "
			"creates the outer polygon automatically. "
			"In addition, $fix_non_simple_polygons will create an convex hull around non simple polygons. "
			"A simple polygon is a polygon which its sides do not intersect."
			;
	}

	// ----------------------------------------------------------------------
	void
		XMLPolygonTopologyTask::
		run( shawn::SimulationController& sc )
		throw( std::runtime_error )
	{

			std::string f = sc.environment().required_string_param("file");
			std::string n = sc.environment().required_string_param("name");
			bool create_outer =
				sc.environment().optional_bool_param("xml_create_outer_hull", false) ||
				sc.environment().optional_bool_param("create_outer_hull", false);

			bool fix_non_simple_polygons = sc.environment().optional_bool_param("fix_non_simple_polygons", false);

			XMLPolygonTopology* p = new XMLPolygonTopology;
			p->set_name(n);
			p->read(sc, f, create_outer, fix_non_simple_polygons);

			topology_keeper_w(sc).add(p);

	}

}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology_task.cpp,v $
* Version $Revision: 348 $
* Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology_task.cpp,v $
*-----------------------------------------------------------------------*/
