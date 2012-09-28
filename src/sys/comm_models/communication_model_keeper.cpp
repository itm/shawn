/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include "sys/comm_models/communication_model_factory.h"
#include "sys/comm_models/communication_model_keeper.h"

using namespace std;

namespace shawn
{
   CommunicationModelKeeper::
   CommunicationModelKeeper()
      : HandleKeeper<CommunicationModelFactory>( "CommunicationModelKeeper", "communication_models" )
   {}
   // ----------------------------------------------------------------------
   CommunicationModelKeeper::
   ~CommunicationModelKeeper()
   {}

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_model_keeper.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: communication_model_keeper.cpp,v $
 *-----------------------------------------------------------------------*/
