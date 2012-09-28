/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/readings/reading_keeper.h"

namespace reading
{

    // ----------------------------------------------------------------------
    ReadingKeeper::ReadingKeeper(): shawn::HandleKeeper<Reading>( "ReadingKeeper", "reading" )
    {}

    // ----------------------------------------------------------------------
    ReadingKeeper::
        ~ReadingKeeper()
    {}

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading_keeper.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Di, 29. Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: reading_keeper.cpp,v $
 *-----------------------------------------------------------------------*/


