/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_TRANS_MODELS_INIT_H
#define __SHAWN_SYS_TRANS_MODELS_TRANS_MODELS_INIT_H



namespace shawn
{
   class SimulationController;
   
   ///Initialises all transmission models by registering them with the TransmisionModelKeeper
   void init_transmission_models( SimulationController& ) throw();

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/transmission_models_init.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: transmission_models_init.h,v $
 *-----------------------------------------------------------------------*/
