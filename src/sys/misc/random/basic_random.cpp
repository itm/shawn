/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/basic_random.h"

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>

namespace shawn
{

	double uniform_random_0i_1i( void ) throw()
	{ return double(rand()) / double(RAND_MAX); }

	double uniform_random_0i_1e( void ) throw()
	{ return double(rand()) / (double(RAND_MAX)+1.0); }

	double uniform_random_0e_1i( void ) throw()
	{ return (double(rand())+1.0) / (double(RAND_MAX)+1.0); }

	double uniform_random_0e_1e( void ) throw()
	{ return (double(rand())+1.0) / (double(RAND_MAX)+2.0); }





   int uniform_discrete_random( int maxval ) throw()
   {
      assert( maxval>=0 );
      return rand() % (maxval+1);
   }


	void init_random_sequence(void) throw()
   { srand((int)time(NULL)); }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/basic_random.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: basic_random.cpp,v $
 *-----------------------------------------------------------------------*/
