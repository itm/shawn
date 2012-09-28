/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_UTIL_TYPES_H
#define __SHAWN_APPS_TESTBEDSERVICE_UTIL_TYPES_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include <string>
#include <vector>
#include <stdint.h>

namespace testbedservice
{

   enum CommandTypes
   {
      DEBUG_MESSAGE            = 10,
      VIRTUAL_LINK_MESSAGE     = 11,
      BYTE_MESSAGE             = 12,
      FLASH_MESSAGE            = 13,

      NODE_OUTPUT_TEXT         = 50,
      NODE_OUTPUT_BYTE         = 51,
      NODE_OUTPUT_VIRTUAL_LINK = 52
   };

   struct TestbedserviceMessage
   {
      std::string source;
      time_t timestamp;
      int size;
      uint8_t *buffer;
   };

   typedef std::vector<std::string> NodeIdVector;
   typedef NodeIdVector::iterator NodeIdVectorIterator;
   typedef std::vector<int> StatusValueVector;
   typedef StatusValueVector::iterator StatusValueIterator;
   typedef std::vector<std::string> StatusMsgVector;
   typedef StatusMsgVector::iterator StatusMsgVectorIterator;

   const std::string SEND_MODE_DEFAULT = "default";
   const std::string SEND_MODE_TESTBEDRUNTIME_ISENSE = "tr-isense";
   
}
#endif
#endif
