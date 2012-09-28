/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_MESSAGE_IN_H_
#define __SHAWN_APPS_TESTBEDSERVICE_VIRTUAL_LINK_MESSAGE_IN_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "sys/event_scheduler.h"
#include <ostream>
#include <stdint.h>


namespace testbedservice
{

   class VirtualLinkMessageIn
      : public shawn::EventScheduler::EventTag
   {
   public:
      VirtualLinkMessageIn();
      VirtualLinkMessageIn( uint8_t *buf );
      virtual ~VirtualLinkMessageIn();
      // --------------------------------------------------------------------
      uint8_t* to_bytes() const throw();
      // --------------------------------------------------------------------
      friend std::ostream& operator<<( std::ostream&, const VirtualLinkMessageIn& ) throw();
      // --------------------------------------------------------------------
      inline long length() const throw()
      { return 21 + payload_size; }
      // --------------------------------------------------------------------
      // --------------------------------------------------------------------
      // --------------------------------------------------------------------
      uint8_t type;          // 1 Byte
      uint8_t req_id;        // 1 Byte
      uint8_t rssi;          // 1 Byte
      uint8_t lqi;           // 1 Byte
      uint8_t payload_size;  // 1 Byte
      uint64_t destination;  // 8 Byte
      uint64_t source;       // 8 Byte
      uint8_t *payload;      // x Byte
   };

}

#endif
#endif
