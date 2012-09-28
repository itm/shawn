/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include <iostream>
#include "apps/vis/elements/vis_drawable_node_keeper.h"

using namespace std;

namespace vis
{
   DrawableNodeKeeper::
   DrawableNodeKeeper()
      : shawn::HandleKeeper<DrawableNodeFactory>( "DrawableNodeKeeper", "drawablenodes" )
   {}
   // ----------------------------------------------------------------------
   DrawableNodeKeeper::
   ~DrawableNodeKeeper()
   {}



}

#endif
