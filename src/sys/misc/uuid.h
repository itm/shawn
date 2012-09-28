/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_UUID_H
#define __SHAWN_SYS_MISC_UUID_H

#include <string>

namespace shawn
{

   class UUIDGenerator
   {
   public:

      static std::string uuid( void ) throw();

   private:
      UUIDGenerator();
      ~UUIDGenerator();

      std::string ulong2string( unsigned long int ) throw();
      std::string generate( void ) throw();

      unsigned long int current_;
      std::string prefix_;

      static UUIDGenerator *instance_;
      static std::string alphabet_;
      static unsigned long int alphabet_length_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/uuid.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: uuid.h,v $
 *-----------------------------------------------------------------------*/
