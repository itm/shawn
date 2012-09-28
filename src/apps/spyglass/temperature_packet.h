/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TEMPERATURE_PACKET_H
#define __SHAWN_APPS_TEMPERATURE_PACKET_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include "apps/spyglass/spyglass_packet.h"
#include <vector>

namespace spyglass
{
   class TemperaturePacket : public SpyglassPacket
   {
   public:
	  TemperaturePacket();
	   virtual ~TemperaturePacket();
	   
	   std::vector<char>* serialize( void ) throw(); 
	   int get_size( void ) throw();
	   void set_temperature( float ) throw();

   protected:
	   float temperature_;

   };
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/temperature_packet.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: temperature_packet.h,v $
 *-----------------------------------------------------------------------*/
