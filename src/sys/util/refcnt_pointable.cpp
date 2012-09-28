/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <cassert>
#include <cstdlib>

#include "sys/util/refcnt_pointable.h"

namespace shawn
{

   RefcntPointable::~RefcntPointable()
   {
      assert( reference_counter_ == 0 );
   }
   // ----------------------------------------------------------------------
   RefcntPointable::RefcntPointable( const RefcntPointable& )
   { abort(); }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/refcnt_pointable.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: refcnt_pointable.cpp,v $
 *-----------------------------------------------------------------------*/
