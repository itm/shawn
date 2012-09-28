/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "temperature_packet.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/temperature_packet.h"

#include <vector>

namespace spyglass
{
	//
	TemperaturePacket::
	TemperaturePacket()
	{}
	// ----------------------------------------------------------------------
	TemperaturePacket::
	~TemperaturePacket()
	{}
	// ----------------------------------------------------------------------
	std::vector<char>*
	TemperaturePacket::
	serialize( void ) 
	throw()
	{
		 std::vector<char> *b = (static_cast<SpyglassPacket*>(this))->serialize();
		 int standardsize = b->size()-1;
		 length_ = get_size();
		 b->resize(length_+1);
		 writeChar( length_, b, 0);

         writeFloat( temperature_, b, standardsize+1);
		 return b;
     }
	 // ----------------------------------------------------------------------
	int 
	TemperaturePacket::
	get_size() 
	throw()
	{ 
		int standardsize = (static_cast<SpyglassPacket*>(this))->get_size();
		return standardsize + 4;
	} 
	// ----------------------------------------------------------------------
	void 
	TemperaturePacket::
	set_temperature( float temp ) 
	throw()
	{
		temperature_ = temp;
	}
	// ---------------------------------------------------------------------- 

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/temperature_packet.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: temperature_packet.cpp,v $
 *-----------------------------------------------------------------------*/
