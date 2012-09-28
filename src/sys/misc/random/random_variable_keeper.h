/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_RANDOM_VARIABLE_KEEPER_H
#define __SHAWN_SYS_MISC_RANDOM_RANDOM_VARIABLE_KEEPER_H

#include "sys/util/handle_keeper.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/misc/random/random_variable.h"

namespace shawn
{
   class RandomVariable;
   
   class RandomVariableKeeper
       : public HandleKeeper<RandomVariable>
   {
   public:
      RandomVariableKeeper();
      virtual ~RandomVariableKeeper();
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_variable_keeper.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: random_variable_keeper.h,v $
 *-----------------------------------------------------------------------*/
