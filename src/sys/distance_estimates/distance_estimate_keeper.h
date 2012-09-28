/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_DISTANCE_ESTIMATE_KEEPER_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_DISTANCE_ESTIMATE_KEEPER_H

#include "shawn_config.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/handle_keeper.h"


namespace shawn
{

   class NodeDistanceEstimate;

   class DistanceEstimateKeeper
      : public HandleKeeper<NodeDistanceEstimate>
   {
   public:
      DistanceEstimateKeeper();
      virtual ~DistanceEstimateKeeper();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/distance_estimate_keeper.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: distance_estimate_keeper.h,v $
 *-----------------------------------------------------------------------*/
