/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_TRANS_MODEL_KEEPER_H
#define __SHAWN_SYS_TRANS_MODELS_TRANS_MODEL_KEEPER_H

#include <map>
#include <string>
#include <stdexcept>

#include "sys/util/refcnt_pointer.h"
#include "sys/util/handle_keeper.h"
#include "sys/transm_models/transmission_model_factory.h"

namespace shawn
{
   class TransmissionModelFactory;

   /// Keeper for transmission models.
   /** Transmission models can be found by their name. 
     * They are initialised and registered by init_transmission_models() 
     */
   class TransmissionModelKeeper
      : public HandleKeeper<TransmissionModelFactory>
   {
   public:
      TransmissionModelKeeper();
      virtual ~TransmissionModelKeeper();
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/transmission_model_keeper.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: transmission_model_keeper.h,v $
 *-----------------------------------------------------------------------*/
