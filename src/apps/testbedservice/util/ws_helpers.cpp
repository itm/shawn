/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/util/ws_helpers.h"
#include <string>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace testbedservice
{

   static const std::string resp_prefix_ = "shawn::nc-";
   static int resp_id_ = 0;
   boost::mutex id_mutex_;
   // -----------------------------------------------------------------------
   std::string create_response_id()
   {
      boost::lock_guard<boost::mutex> sc_lock( id_mutex_ );

      std::stringstream out;
      out << resp_prefix_ << resp_id_;

      resp_id_++;
      return out.str();
   }
   // -----------------------------------------------------------------------
   char* allocate_string( struct soap *soap, std::string s )
   {
      char* ret = (char*)soap_malloc( soap, s.size() + 1 );
      memcpy( ret, s.c_str(), s.size() + 1 );

      return ret;
   }
   // -----------------------------------------------------------------------
   char* allocate_string( std::string s )
   {
      char *ret = new char[s.size() + 1];
      memcpy( ret, s.c_str(), s.size() + 1 );

      return ret;
   }
   // -----------------------------------------------------------------------
   int* allocate_int( int i )
   {
      int *ret = new int;
      *ret = i;

      return ret;
   }

}

#endif
