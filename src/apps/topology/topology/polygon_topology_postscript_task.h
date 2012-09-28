/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_POLYGON_TOPOLOGY_POSTSCRIPT_TASK_H
#define __SHAWN_APPS_TOPOLOGY_POLYGON_TOPOLOGY_POSTSCRIPT_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include "apps/topology/topology/polygon_topology.h"
#include "apps/topology/polygon/segment_2d.h"
#include "apps/topology/polygon/polygon.h"
#include <iostream>
#include <fstream>

namespace topology
{

	class PolygonTopologyPostscriptTask
		: public shawn::SimulationTask
	{
	public:
		PolygonTopologyPostscriptTask();
		virtual ~PolygonTopologyPostscriptTask();

		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();
		virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );

	protected:
		virtual void write_header(shawn::SimulationController& sc, std::ostream& out);
		virtual void paint(shawn::SimulationController& sc, PolygonTopology& p, std::ostream& out);
		virtual void draw(const PolygonTopology::Polygon& p, shawn::SimulationController& sc, std::ostream& out);
		shawn::Vec pos2ps( const shawn::Vec& v ) const throw();
		double scale(double d)  const throw();

	protected:
		double picture_dimension_;
		double border_width_;
		double scale_;
		double ps_x0_;
		double ps_y0_;
	};

}


#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/xml_polygon_topology_task.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: xml_polygon_topology_task.h,v $
*-----------------------------------------------------------------------*/
