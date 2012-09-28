/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_COMM_MODEL_KEEPER_H
#define __SHAWN_SYS_COMM_MODELS_COMM_MODEL_KEEPER_H

#include <map>
#include <string>
#include <stdexcept>

#include "sys/util/refcnt_pointer.h"
#include "sys/util/handle_keeper.h"
#include "sys/comm_models/communication_model_factory.h"

namespace shawn
{
   class CommunicationModelFactory;

   /// Keeper for communication models.
   /** Communication models can be found by their name. 
     * They are initialised and registered by init_communication_models() 
     */
   class CommunicationModelKeeper
      : public HandleKeeper<CommunicationModelFactory>
   {
   public:
      CommunicationModelKeeper();
      virtual ~CommunicationModelKeeper();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/communication_model_keeper.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: communication_model_keeper.h,v $
 *-----------------------------------------------------------------------*/
