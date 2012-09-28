/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "polygon_topology_packet.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/polygon_topology_packet.h"
#include "sys/vec.h"

#include <vector>

namespace spyglass
{
	PolygonTopologyPacket::
	PolygonTopologyPacket()
	{}
	// ----------------------------------------------------------------------
	PolygonTopologyPacket::
	~PolygonTopologyPacket()
	{}
	// ----------------------------------------------------------------------
	std::vector<char>*
	PolygonTopologyPacket::
	serialize( void ) 
	throw()
	{
		 std::vector<char> *b = (static_cast<SpyglassPacket*>(this))->serialize();
		 int standardsize = b->size()-1;
		 length_ = get_size();
		 b->resize(length_+1);
		 writeChar( length_, b, 0);
		 writeChar( polType_, b, standardsize+1 );
		 int size = polygon_.size();
		 for (int i=0; i<size; ++i)
		 {
			 shawn::Vec pos = polygon_.front();
			 writeFloat(pos.x(), b, standardsize+2+i*8);
			 writeFloat(pos.y(), b, standardsize+6+i*8);
			 polygon_.pop_front();
		 } 
		 return b;
     }
	 // ----------------------------------------------------------------------
	int 
	PolygonTopologyPacket::
	get_size() 
	throw()
	{ 
		int standardsize = (static_cast<SpyglassPacket*>(this))->get_size();
		return standardsize + 8* polygon_.size() + 1;
	} 
	// ----------------------------------------------------------------------
	void 
	PolygonTopologyPacket::
	set_polygon( std::list<shawn::Vec> pol ) 
	throw()
	{
		polygon_ = pol;
	}
	// ---------------------------------------------------------------------- 
	void 
	PolygonTopologyPacket::
	set_polygon_type( int type ) 
	throw()
	{
		polType_ = type;
	}
	// ---------------------------------------------------------------------- 
	void 
	PolygonTopologyPacket::
	add_position( shawn::Vec pos ) 
	throw()
	{
		polygon_.push_back(pos);
	}
	// ---------------------------------------------------------------------- 
	void 
	PolygonTopologyPacket::
	clear_positions( void ) 
	throw()
	{
		polygon_.clear();
	}
	// ---------------------------------------------------------------------- 
	
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/polygon_topology_packet.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: polygon_topology_packet.cpp,v $
 *-----------------------------------------------------------------------*/
