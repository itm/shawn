/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_RANDOM_VARIABLE_H
#define __SHAWN_SYS_MISC_RANDOM_RANDOM_VARIABLE_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"


namespace shawn
{
   DECLARE_HANDLES(RandomVariable);

   class RandomVariable
      : public KeeperManaged
   {
   public:
      RandomVariable();
      virtual ~RandomVariable();

      virtual operator double( void ) const throw() = 0;
      virtual double mean( void ) const throw() = 0;
      virtual double variance( void ) const throw() = 0;
      virtual double standard_deviation( void ) const throw() = 0;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_variable.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: random_variable.h,v $
 *-----------------------------------------------------------------------*/
