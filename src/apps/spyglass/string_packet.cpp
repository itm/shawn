/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "string_packet.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/string_packet.h"

#include <vector>
#include <string>


namespace spyglass
{
	//
	StringPacket::
	StringPacket()
	{}
	// ----------------------------------------------------------------------
	StringPacket::
	~StringPacket()
	{}
	// ----------------------------------------------------------------------
	std::vector<char>*
	StringPacket::
	serialize( void ) 
	throw()
	{
		 std::vector<char> *b = (static_cast<SpyglassPacket*>(this))->serialize();
		 int standardsize = b->size()-1;
		 length_ = get_size();
		 b->resize(length_+1);
		 writeChar( length_, b, 0);
		 writeString(str_, b, standardsize+1);
		 return b;
     }
	 // ----------------------------------------------------------------------
	int 
	StringPacket::
	get_size() 
	throw()
	{ 
		int standardsize = (static_cast<SpyglassPacket*>(this))->get_size();
		return standardsize + str_.size() + 4;
	} 
	// ----------------------------------------------------------------------
	void 
	StringPacket::
	set_string( std::string str ) 
	throw()
	{
		str_ = str;
	}
	// ---------------------------------------------------------------------- 

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/string_packet.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: string_packet.cpp,v $
 *-----------------------------------------------------------------------*/
