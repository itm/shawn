/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SPYGLASS_PACKET_H
#define __SHAWN_APPS_SPYGLASS_PACKET_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_SPYGLASS

#include <vector>
#include <string>

#include "sys/util/refcnt_pointable.h"

namespace spyglass
{
   class SpyglassPacket
	   : public shawn::RefcntPointable
   {
   public:
	   SpyglassPacket();
	   virtual ~SpyglassPacket();
	   
	   int readChar(std::vector<char>, int ) throw();
	   void writeChar( int, std::vector<char>*, int ) throw();
	   std::string readString(std::vector<char> data, int start) throw();
	   void writeString(std::string s, std::vector<char> *b, int start) throw();
	   int readShort(std::vector<char>, int ) throw();
	   void writeShort( int, std::vector<char>* , int ) throw();
	   int readInt(std::vector<char>, int ) throw();
	   void writeInt( int, std::vector<char>*, int ) throw();
	   float readFloat(std::vector<char>, int ) throw();
	   void writeFloat( float, std::vector<char>*, int ) throw();
	   
	   std::vector<char>* serialize( void ) throw();
	   void deserialize( std::vector<char> ) throw();

	   int get_size( void ) throw();
	   int get_type( void ) throw();

	   void set_version( int ) throw();
	   void set_type( int ) throw();
	   void set_nodeId( int ) throw();
	   void set_hour( int ) throw();
	   void set_minute( int ) throw();
	   void set_second( int ) throw();
	   void set_x( float ) throw();
	   void set_y( float ) throw();
	   void set_z( float ) throw();
	   // The parameters are: version, type, nodeId, hour, minute, second, x, y, z
	   void set_parameters(int, int, int, int, int, int, float, float, float ) throw();
	   // version, type, nodeId, time, x, y, z
	   void set_parameters(int, int, int, double, float, float, float ) throw();
	   

   protected:
	   int length_;
	   int version_;
	   int type_;
	   int nodeId_;
	   int hour_;
	   int minute_;
	   int second_;
	   float x_;
	   float y_;
	   float z_;

   };
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass_packet.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: spyglass_packet.h,v $
 *-----------------------------------------------------------------------*/
