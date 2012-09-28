/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/node_finder.h"
#include "sys/vec.h"
#include "sys/node.h"
#include "sys/world.h"

using namespace shawn;

namespace shawn
{
	NodeFinder::
	NodeFinder(World* w){
		w_ = w;
	}


	Node* 
	NodeFinder::
	findByPosition( Vec v){
        double minDist = 9999999999.9;
		double d;
		Node* n = NULL;
		for( shawn::World::node_iterator it = w_->begin_nodes_w(); it != w_->end_nodes_w(); ++it) 
        {            
            Vec pos = (*it).real_position();
			d = dist(pos, v);
			if (d<minDist){
				n = &(*it);
				minDist=d;
			}
		}
		return n;
	}

	NodeSet
	NodeFinder::
	findInCircle( Vec center, double radius){
        NodeSet ns;
		for( shawn::World::node_iterator it = w_->begin_nodes_w(); it != w_->end_nodes_w(); ++it) 
			if (dist((*it).real_position(), center)<=radius) ns.insert(&(*it));
		return ns;
	}


	double
	NodeFinder::
	dist( Vec v1, Vec v2 ) {
		Vec v = v1 - v2;
		return v.euclidean_norm();
	}

	Node* 
		NodeFinder::
		findById( int id )
	{
		 return w_->find_node_by_id_w(id);
	}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/node_finder.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: node_finder.cpp,v $
 *-----------------------------------------------------------------------*/
