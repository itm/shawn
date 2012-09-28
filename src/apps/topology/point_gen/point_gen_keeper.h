/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_POINT_GEN_KEEPER_H
#define __SHAWN_APPS_TOPOLOGY_POINT_GEN_KEEPER_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/util/handle_keeper.h"
#include "apps/topology/point_gen/point_gen.h"
#include <string>



namespace topology
{
   

   class PointGeneratorKeeper
      : public shawn::HandleKeeper<PointGenerator>
   {
   public:
      static const std::string KEEPER_NAME;

      PointGeneratorKeeper();
      virtual ~PointGeneratorKeeper();
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/point_gen_keeper.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: point_gen_keeper.h,v $
 *-----------------------------------------------------------------------*/
