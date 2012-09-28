/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/apps_init.h"
#include "_apps_init_cmake.h"
#include <iostream>
#include <stdio.h>

//See: http://gcc.gnu.org/onlinedocs/cpp/Stringification.html (2nd example)
#define xstr(s) str(s)
#define str(s) #s

namespace shawn
{
	void init_apps( SimulationController& sc )
	{
		std::cout << "init_apps: " << xstr(INIT_STATIC_APPS_MODULES) << std::endl;
		
		INIT_STATIC_APPS_MODULES;
	}
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/apps_init.cpp,v $
 * Version $Revision: 337 $
 * Date    $Date: 2009-03-13 11:53:11 +0100 (Fri, 13 Mar 2009) $
 *-----------------------------------------------------------------------
 * $Log: apps_init.cpp,v $
 *-----------------------------------------------------------------------*/
