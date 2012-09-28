/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_OS_SYS_RESOURCES_H
#define __SHAWN_SYS_MISC_OS_SYS_RESOURCES_H

#include <ctime>

namespace shawn
{

   class SystemResources
   {
   public:
      static const SystemResources& instance( void ) throw();


      /// \returns estimates memory footprint in megabytes,
      /// or -1 if unknown
      int memory_usage_mbytes( void ) const throw();

      /// \returns user time since application start, in seconds,
      /// or -1 if unknown
      int user_time_seconds( void ) const throw();

      /// \returns kernel/system time since application start,
      /// in seconds, or -1 if unknown
      int system_time_seconds( void ) const throw();

      /// user_time_seconds() + system_time_seconds(),
      /// or -1 if unknown
      int time_seconds( void ) const throw();

      /// wall clock time since application start,
      /// in seconds, or -1 if unknown
      int wc_time_seconds( void ) const throw();

   private:
      static SystemResources instance_;

      SystemResources();
      ~SystemResources();

      time_t app_start_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/sys_resources.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: sys_resources.h,v $
 *-----------------------------------------------------------------------*/
