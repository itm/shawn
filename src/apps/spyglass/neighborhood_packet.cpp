/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "neighborhood_packet.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/neighborhood_packet.h"
#include "sys/vec.h"
#include "sys/node.h"

#include <vector>
#include <string>

namespace spyglass
{
	//
	NeighborhoodPacket::
	NeighborhoodPacket()
	{}
	// ----------------------------------------------------------------------
	NeighborhoodPacket::
	~NeighborhoodPacket()
	{}
	// ----------------------------------------------------------------------
	std::vector<char>*
	NeighborhoodPacket::
	serialize( void ) 
	throw()
	{
		 std::vector<char> *b = (static_cast<SpyglassPacket*>(this))->serialize();
		 int standardsize = b->size()-1;
		 length_ = get_size();
		 b->resize(length_+1);
		 writeChar( length_, b, 0);
		 int count_neighbours = neighbours_.size();
		 writeInt(count_neighbours, b , standardsize +1);
		 for (int i=0; i<count_neighbours; i++)
		 {
			 writeInt( neighbours_.front(), b, standardsize+1+4+4*i);
			 neighbours_.pop_front();
		 }
		 return b;
     }
	 // ----------------------------------------------------------------------
	int 
	NeighborhoodPacket::
	get_size() 
	throw()
	{ 
		int standardsize = (static_cast<SpyglassPacket*>(this))->get_size();
		return standardsize + 4 + 4*neighbours_.size();
	} 
	// ----------------------------------------------------------------------
	void 
	NeighborhoodPacket::
	add_neighbour( int id ) 
	throw()
	{
		neighbours_.push_back(id);
	}
	// ---------------------------------------------------------------------- 
	void 
	NeighborhoodPacket::
	add_neighbour( const shawn::Node *node) 
	throw()
	{
		neighbours_.push_back(node->id());
	}
	// ---------------------------------------------------------------------- 
	void 
	NeighborhoodPacket::
	clear_neighbours( void) 
	throw()
	{
		neighbours_.clear();
	}
	// ---------------------------------------------------------------------- 
	
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/neighborhood_packet.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: neighborhood_packet.cpp,v $
 *-----------------------------------------------------------------------*/
