/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "color_packet.h"
#ifdef ENABLE_SPYGLASS

#include <vector>
#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/color_packet.h"
#include "sys/vec.h"

namespace spyglass
{
	ColorPacket::
	ColorPacket()
	{
		inner_color_.clear();
		inner_color_.push_back(0);
		inner_color_.push_back(0);
		inner_color_.push_back(0);
		outer_color_.clear();
		outer_color_.push_back(0);
		outer_color_.push_back(0);
		outer_color_.push_back(0);
		duration_ = 0;
	}
	// ----------------------------------------------------------------------
	ColorPacket::
	~ColorPacket()
	{}
	// ----------------------------------------------------------------------
	std::vector<char>*
	ColorPacket::
	serialize( void ) 
	throw()
	{
		 std::vector<char> *b = (static_cast<SpyglassPacket*>(this))->serialize();
		 int standardsize = b->size()-1;
		 length_ = get_size();
		 b->resize(length_+1);
		 writeChar( length_, b, 0);
         writeChar( inner_color_[0], b, standardsize+1);
		 writeChar( inner_color_[1], b, standardsize+2);
		 writeChar( inner_color_[2], b, standardsize+3);
		 writeChar( outer_color_[0], b, standardsize+4);
		 writeChar( outer_color_[1], b, standardsize+5);
		 writeChar( outer_color_[2], b, standardsize+6);
		 writeFloat( float(duration_), b, standardsize+7);
		 return b;
     }
	 // ----------------------------------------------------------------------
	int 
	ColorPacket::
	get_size() 
	throw()
	{ 
		int standardsize = (static_cast<SpyglassPacket*>(this))->get_size();
		return standardsize + 10; // = 2*3Bytes + 4Bytes(for 1Float)
	} 
	// ----------------------------------------------------------------------
	void 
	ColorPacket::
	set_inner_color( int r, int g, int b)
	throw()
	{
		inner_color_.clear();
		inner_color_.push_back(r);
		inner_color_.push_back(g);
		inner_color_.push_back(b);
	}	
	// ----------------------------------------------------------------------
	void 
	ColorPacket::
	set_outer_color( int r, int g, int b) 
	throw()
	{
		outer_color_.clear();
		outer_color_.push_back(r);
		outer_color_.push_back(g);
		outer_color_.push_back(b);
	}
	// ---------------------------------------------------------------------- 
	void 
	ColorPacket::
	set_duration( double d) 
	throw()
	{
		duration_ = d;
	}
	// ---------------------------------------------------------------------- 

}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/color_packet.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: color_packet.cpp,v $
 *-----------------------------------------------------------------------*/
