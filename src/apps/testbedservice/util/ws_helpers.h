/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TESTBEDSERVICE_WS_HELPERS_H_
#define __SHAWN_APPS_TESTBEDSERVICE_WS_HELPERS_H_

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include <stdsoap2.h>
#include <string>


namespace testbedservice
{

   std::string create_response_id();

   char* allocate_string( struct soap *soap, std::string s );

   char* allocate_string( std::string s );
   int* allocate_int( int i );

}

#endif
#endif
