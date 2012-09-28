/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_UTIL_HANDLE_KEEPER_BASE_H
#define __SHAWN_SYS_UTIL_HANDLE_KEEPER_BASE_H

#include "sys/util/keeper_managed.h"

#include <string>
#include <stdexcept>

namespace shawn
{


   class HandleKeeperBase
   {
   public:
      virtual ~HandleKeeperBase();
      virtual void print_contents( std::ostream& os,
                                   bool verbose )
         const throw() = 0;
      virtual ConstKeeperManagedHandle find_managed( const std::string& ) const throw( std::runtime_error ) = 0;
      virtual KeeperManagedHandle find_managed_w( const std::string& ) throw( std::runtime_error ) = 0;
      virtual const std::string& name( void ) const throw() = 0;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/handle_keeper_base.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: handle_keeper_base.h,v $
 *-----------------------------------------------------------------------*/
