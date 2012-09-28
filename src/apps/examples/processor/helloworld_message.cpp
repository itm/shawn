/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/examples/processor/helloworld_message.h"
#ifdef ENABLE_EXAMPLES

namespace helloworld
{

	// ----------------------------------------------------------------------
	HelloworldMessage::
		HelloworldMessage()
	{}
   
	// ----------------------------------------------------------------------
	HelloworldMessage::
		HelloworldMessage(int size)
	{
		setSize(size);
	}

	// ----------------------------------------------------------------------
	HelloworldMessage::
		~HelloworldMessage()
	{}

}
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_message.cpp,v $
 * Version $Revision: 126 $
 * Date    $Date: 2008-02-21 16:33:02 +0100 (Thu, 21 Feb 2008) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_message.cpp,v $
 *-----------------------------------------------------------------------*/
