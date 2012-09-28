/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_UTIL_STRING_CONV_H
#define __SHAWN_SYS_UTIL_STRING_CONV_H

#include <string>
#include <list>
#include <stdexcept>

#include "shawn_config.h"

namespace shawn
{
   
   int 
   conv_string_to_int( const std::string& )
      throw( std::runtime_error );


   double
   conv_string_to_double( const std::string& )
      throw( std::runtime_error );


   bool
   conv_string_to_bool( const std::string& )
      throw( std::runtime_error );

   void
   split_comma_separated( const std::string&,
                          std::list<std::string>& )
      throw();

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/string_conv.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: string_conv.h,v $
 *-----------------------------------------------------------------------*/
