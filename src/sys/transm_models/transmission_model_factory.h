/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TRANS_MODELS_TRANS_MODEL_FACTORY_H
#define __SHAWN_SYS_TRANS_MODELS_TRANS_MODEL_FACTORY_H

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"

namespace shawn
{
   class TransmissionModel;
   class SimulationController;

   DECLARE_HANDLES(TransmissionModelFactory);

   /// Abstract parent class for transmission model factories. 
   /* Implement a factory class for each transmission model you implement. Then register the factory 
    * with the TransmissionModelKeeper in the init method init_transmission_models().
    */
   class TransmissionModelFactory
      : public KeeperManaged
   {
   public:

      virtual ~TransmissionModelFactory();

      virtual TransmissionModel* create( const SimulationController& ) throw() = 0;
   };



}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/transmission_model_factory.h,v $
 * Version $Revision: 66 $
 * Date    $Date: 2007-09-14 14:53:16 +0200 (Fri, 14 Sep 2007) $
 *-----------------------------------------------------------------------
 * $Log: transmission_model_factory.h,v $
 *-----------------------------------------------------------------------*/
