/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_message_in.h"
#include <cstring>

namespace testbedservice
{

   VirtualLinkMessageIn::
   VirtualLinkMessageIn()
      : type          ( 0 ),
         req_id       ( 0 ),
         rssi         ( 0 ),
         lqi          ( 0 ),
         payload_size ( 0 ),
         destination  ( 0 ),
         source       ( 0 ),
         payload      ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkMessageIn::
   VirtualLinkMessageIn( uint8_t *buf )
   {
      // Type at 0
      type = buf[0];
      // Request Id at 1
      req_id = buf[1];
      // RSSI at 2
      rssi = buf[2];
      // LQI at 3
      lqi = buf[3];
      // Payload length at 4
      payload_size = buf[4];
      // Destination at pos 5..12
      // msg->dest = buf[11] + (buf[10] << 8) + ... + ((uint64_t)buf[4] << 56);
      destination = 0;
      for ( int i = 0; i < 8; i++ )
         destination += ((uint64_t)buf[12-i] << (i*8));
      // Source at pos 13..20
      source = 0;
      for ( int i = 0; i < 8; i++ )
         source += ((uint64_t)buf[20-i] << (i*8));
      // Payload beginning at 21
      payload = new uint8_t[payload_size];
      memcpy( payload, &buf[21], payload_size );
   }
   // ----------------------------------------------------------------------
   VirtualLinkMessageIn::
   ~VirtualLinkMessageIn()
   {
      delete payload;
   }
   // ----------------------------------------------------------------------
   uint8_t*
   VirtualLinkMessageIn::
   to_bytes()
      const throw()
   {
      uint8_t *buffer = new uint8_t[length()];

      // Type at 0
      buffer[0] = type;
      // Request Id at 1
      buffer[1] = req_id;
      // RSSI at 2
      buffer[2] = rssi;
      // LQI at 3
      buffer[3] = lqi;
      // Payload length at 4
      buffer[4] = payload_size;
      // Destination at pos 5..12
      for ( int i = 0; i < 8; i++ )
         buffer[12-i] = (destination >> (i*8)) & 0xff;
      // Source at pos 13..20
      for ( int i = 0; i < 8; i++ )
         buffer[20-i] = (source >> (i*8)) & 0xff;
      // Payload beginning at 21
      memcpy( &buffer[21], payload, payload_size );

      return buffer;
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<( std::ostream& os, const VirtualLinkMessageIn& vlm )
      throw()
   {
      os << "VirtualLinkMessageIn: " << std::endl
         << " Type : " << (int)vlm.type << std::endl
         << " ReqId: " << (int)vlm.req_id << std::endl
         << " RSSI : " << (int)vlm.rssi << std::endl
         << " LQI  : " << (int)vlm.lqi << std::endl
         << " PSize: " << (uint32_t)vlm.payload_size << std::endl
         << " Dest : " << (uint64_t)vlm.destination << std::endl
         << " Src  : " << (uint64_t)vlm.source << std::endl
         << " Payload:";
      for ( int i = 0; i < vlm.payload_size; i++ )
         os << " " << (int)(vlm.payload[i]);
      os << std::endl;
      os << " Overall Message Length: " << (int)vlm.length() << std::endl;
   }

}

#endif
