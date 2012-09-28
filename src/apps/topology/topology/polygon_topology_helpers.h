/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_TOPOLOGY_HELPERS_H
#define __SHAWN_APPS_TOPOLOGY_TOPOLOGY_HELPERS_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"			

#include "sys/vec.h"
#include "apps/topology/polygon/polygon.h"
#include "apps/topology/polygon/segment_2d.h"
#include "sys/tag.h"
#include "sys/node.h"
#include "sys/edge_model.h"

namespace topology
{

    /** Creates a convex hull around the given vertices.
      *
      */
/*    shawn::Polygon& 	// deprecated! use "apps/topology/polygon/polygon.h"
        convex_hull(shawn::Polygon::Vertex_const_iterator begin, shawn::Polygon::Vertex_const_iterator end)	
    	throw();
*/
    /** Calculates the shortest distance line segment between a polygon and a point
      *
      */
/*    shawn::CGALSegment2D shortest_distance_segment(const shawn::Polygon& poly, const shawn::CGAL2D p) //deprecated - use "apps/topology/polygon/polygon.h"
    	throw();
*/
    /** Calculates the shortest distance between a polygon and a point
      *
      */
 /*   double shortest_distance(const shawn::Polygon& poly, const shawn::CGAL2D p) //deprecated - use "apps/topology/polygon/polygon.h"
    	throw();
*/
    /** Checks whether a node has the tag tag_name attached
      *
      */
    bool has_tag(shawn::Node& node, const std::string tag_name);


}


#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology_helpers.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: polygon_topology_helpers.h,v $
* Revision 1.2  2005/08/05 10:00:35  ali
* 2005 copyright notice
*
* Revision 1.1  2005/07/31 17:01:42  ali
* ported polygon stuff to new topo
*
* Revision 1.5  2005/03/08 09:58:30  pfister
* Compile issues and warnings removed
*
* Revision 1.4  2005/03/08 09:52:42  pfister
* minor compile issues on linux resolved
*
* Revision 1.3  2005/03/08 09:29:51  pfister
* initial polygon topology stuff
* minor bugfixes
*
* Revision 1.2  2005/03/02 10:18:58  pfister
* Added missing return type
*
* Revision 1.1  2005/03/02 10:04:28  pfister
* Topology stuff updated and changes for the new config system
*
*-----------------------------------------------------------------------*/
