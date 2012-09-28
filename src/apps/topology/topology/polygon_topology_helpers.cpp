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

#include "apps/topology/topology/polygon_topology_helpers.h"

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace shawn;
using namespace polygon;

namespace topology
{

    // ----------------------------------------------------------------------
/*		// deprecated! use "apps/topology/polygon/polygon.h"
	Polygon& 
        convex_hull(Polygon::Vertex_const_iterator begin, Polygon::Vertex_const_iterator end) 
    	throw()
    {
        vector <CGAL2D> vertices;		//deprecated - use "apps/topology/polygon/polygon.h"
        CGAL::ch_graham_andrew(begin, end, back_inserter(vertices));
        Polygon& pnew = *(new Polygon);

        for(vector <CGAL2D>::iterator it = vertices.begin(); it != vertices.end(); ++it) 
            pnew.push_back( *it );

        return pnew;
    }
*/    
    
    // ----------------------------------------------------------------------
/*    Segment2D shortest_distance_segment(const Polygon& poly, const Vec p) 
        throw()
    {
        double min = numeric_limits<double>::max(), d;
        Segment2D min_segment;

        for(Polygon::Edge_const_iterator it = poly.e_begin(); it != poly.e_end() ; ++it)
        {
            d = squared_distance((*it), p);
            if( d < min )
            {
                min = d;
                min_segment = *it;
            }
        }
        return min_segment;
    }
*/

    // ----------------------------------------------------------------------
/*    double shortest_distance(const Polygon& poly, const Vec p) 
        throw()
    {
        return sqrt( squared_distance( shortest_distance_segment(poly, p), p ));
    }
*/


}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/polygon_topology_helpers.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: polygon_topology_helpers.cpp,v $
 * Revision 1.2  2005/08/05 10:00:35  ali
 * 2005 copyright notice
 *
 * Revision 1.1  2005/07/31 17:01:42  ali
 * ported polygon stuff to new topo
 *
 * Revision 1.1  2005/03/02 10:04:28  pfister
 * Topology stuff updated and changes for the new config system
 *
 *-----------------------------------------------------------------------*/
