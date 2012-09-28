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
#include "apps/vis/writer/vis_writer_keeper.h"

using namespace std;

namespace vis
{
   WriterKeeper::
   WriterKeeper()
      : shawn::HandleKeeper<WriterFactory>( "WriterKeeper", "writers" )
   {}
   // ----------------------------------------------------------------------
   WriterKeeper::
   ~WriterKeeper()
   {}



}

#endif
