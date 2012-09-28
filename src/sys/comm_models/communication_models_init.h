/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_COMM_MODELS_INIT_H
#define __SHAWN_SYS_COMM_MODELS_COMM_MODELS_INIT_H



namespace shawn
{
   class SimulationController;
   
   ///Initialises all communication models by registering them with the CommunicationModelKeeper
   void init_communication_models( SimulationController& ) throw();

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_models_init.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: communication_models_init.h,v $
 *-----------------------------------------------------------------------*/
