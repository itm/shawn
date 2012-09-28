/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_BASIC_RANDOM_H
#define __SHAWN_SYS_MISC_RANDOM_BASIC_RANDOM_H


namespace shawn
{


	double uniform_random_0i_1i( void ) throw();
	double uniform_random_0i_1e( void ) throw();
	double uniform_random_0e_1i( void ) throw();
	double uniform_random_0e_1e( void ) throw();

   int uniform_discrete_random( int maxval ) throw();

	void init_random_sequence(void) throw();

}


#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/basic_random.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: basic_random.h,v $
 *-----------------------------------------------------------------------*/
