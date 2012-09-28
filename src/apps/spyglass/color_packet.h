/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_COLOR_PACKET_H
#define __SHAWN_APPS_COLOR_PACKET_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include "apps/spyglass/spyglass_packet.h"

#include <vector>


namespace spyglass
{
   class ColorPacket : public SpyglassPacket
   {
   public:
	  ColorPacket();
	   virtual ~ColorPacket();
	   
	   std::vector<char>* serialize( void ) throw(); 
	   int get_size( void ) throw();
	   void set_inner_color( int, int, int ) throw();
	   void set_outer_color( int, int, int ) throw();
	   void set_duration( double ) throw();

   protected:
	   std::vector<int> inner_color_;
	   std::vector<int> outer_color_;
	   double duration_;

   };
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/color_packet.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: color_packet.h,v $
 *-----------------------------------------------------------------------*/
