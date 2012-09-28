/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __VIS_DRAWABLE_NODE_KEEPER_H
#define __VIS_DRAWABLE_NODE_KEEPER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <map>
#include <string>
#include <stdexcept>

#include "shawn_config.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/handle_keeper.h"
#include "apps/vis/elements/vis_drawable_node_factory.h"

namespace vis
{

   /**
    * A keeper that holds node factories.
    */
   class DrawableNodeKeeper
	   : public shawn::HandleKeeper<DrawableNodeFactory>
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      DrawableNodeKeeper();
      virtual ~DrawableNodeKeeper();
      ///@}
   };


}

#endif
#endif

