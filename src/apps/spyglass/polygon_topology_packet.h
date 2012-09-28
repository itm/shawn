/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_POLYGON_TOPOLOGY_PACKET_H
#define __SHAWN_APPS_POLYGON_TOPOLOGY_PACKET_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include <vector>
#include <list>
#include "apps/spyglass/spyglass_packet.h"

namespace shawn {
	class Vec;
}

namespace spyglass
{

   class PolygonTopologyPacket : public SpyglassPacket
   {
   public:
	  PolygonTopologyPacket();
	   virtual ~PolygonTopologyPacket();
	   
	   std::vector<char>* serialize( void ) throw(); 
	   int get_size( void ) throw();
	   void set_polygon( std::list<shawn::Vec> ) throw();
	   void add_position( shawn::Vec ) throw();
	   void set_polygon_type( int ) throw();
	   void clear_positions( void ) throw();

   protected:
	   std::list<shawn::Vec> polygon_;
	   int polType_;
   };
}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/polygon_topology_packet.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: polygon_topology_packet.h,v $
 *-----------------------------------------------------------------------*/
