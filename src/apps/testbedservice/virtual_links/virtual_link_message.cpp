/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_message.h"
#include <cstring>

namespace testbedservice
{

   VirtualLinkMessage::
   VirtualLinkMessage()
      : type          ( 0 ),
         rssi         ( 0 ),
         lqi          ( 0 ),
         payload_size ( 0 ),
         destination  ( 0 ),
         source       ( 0 ),
         payload      ( 0 ),
         pass_to      ( 0 )
   {}
   // ----------------------------------------------------------------------
   VirtualLinkMessage::
   VirtualLinkMessage( uint8_t *buf )
   {
      // Type at 0
      type = buf[0];
      // RSSI at 1
      rssi = buf[1];
      // LQI at 2
      lqi = buf[2];
      // Payload length at 3
      payload_size = buf[3];
      // Destination at pos 4..11
      // msg->dest = buf[11] + (buf[10] << 8) + ... + ((uint64_t)buf[4] << 56);
      destination = 0;
      for ( int i = 0; i < 8; i++ )
         destination += ((uint64_t)buf[11-i] << (i*8));
      // Source at pos 12..19
      source = 0;
      for ( int i = 0; i < 8; i++ )
         source += ((uint64_t)buf[19-i] << (i*8));
      // Payload beginning at 22
      payload = new uint8_t[payload_size];
      memcpy( payload, &buf[20], payload_size );
   }
   // ----------------------------------------------------------------------
   VirtualLinkMessage::
   ~VirtualLinkMessage()
   {
      delete payload;
   }
   // ----------------------------------------------------------------------
   uint8_t*
   VirtualLinkMessage::
   to_bytes()
      const throw()
   {
      uint8_t *buffer = new uint8_t[length()];

      // Type at 0
      buffer[0] = type;
      // RSSI at 1
      buffer[1] = rssi;
      // LQI at 2
      buffer[2] = lqi;
      // Payload length at 3
      buffer[3] = payload_size;
      // Destination at pos 4..11
      for ( int i = 0; i < 8; i++ )
         buffer[11-i] = (destination >> (i*8)) & 0xff;
      // Source at pos 12..19
      for ( int i = 0; i < 8; i++ )
         buffer[19-i] = (source >> (i*8)) & 0xff;
      // Payload beginning at 22
      memcpy( &buffer[20], payload, payload_size );

      return buffer;
   }
   // ----------------------------------------------------------------------
   std::ostream&
   operator<<( std::ostream& os, const VirtualLinkMessage& vlm )
      throw()
   {
      os << "VirtualLinkMessage: " << std::endl
         << " Type : " << (int)vlm.type << std::endl
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
