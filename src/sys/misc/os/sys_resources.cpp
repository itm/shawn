/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "sys/misc/os/sys_resources.h"

#ifdef unix
#include <sys/resource.h>
#include <sys/time.h>
#endif

namespace shawn
{
   SystemResources SystemResources::instance_;
   // ----------------------------------------------------------------------
   const SystemResources&
   SystemResources::instance( void )
      throw()
   { return instance_; }
   // ----------------------------------------------------------------------
   SystemResources::
   SystemResources()
   {
      app_start_=time(NULL);
   }
   // ----------------------------------------------------------------------
   SystemResources::
   ~SystemResources()
   {}
   // ----------------------------------------------------------------------
   int
   SystemResources::
   wc_time_seconds( void )
      const throw()
   {
      return time(NULL) - app_start_;
   }


   


#ifdef unix

   int
   SystemResources::
   memory_usage_mbytes( void )
      const throw()
   { return -1; }
   // ----------------------------------------------------------------------
   int 
   SystemResources::
   user_time_seconds( void )
      const throw()
   {
      struct rusage ru;
      if( getrusage( RUSAGE_SELF, &ru )==0 )
         return ru.ru_utime.tv_sec;
      else
         return -1; 
   }
   // ----------------------------------------------------------------------
   int
   SystemResources::
   system_time_seconds( void )
      const throw()
   { 
      struct rusage ru;
      if( getrusage( RUSAGE_SELF, &ru )==0 )
         return ru.ru_stime.tv_sec;
      else
         return -1; 
   }
   // ----------------------------------------------------------------------
   int
   SystemResources::
   time_seconds( void )
      const throw()
   { 
      struct rusage ru;
      if( getrusage( RUSAGE_SELF, &ru )==0 )
         return ru.ru_utime.tv_sec + ru.ru_stime.tv_sec;
      else
         return -1; 
   }

#else


   int
   SystemResources::
   memory_usage_mbytes( void )
      const throw()
   { return -1; }
   // ----------------------------------------------------------------------
   int 
   SystemResources::
   user_time_seconds( void )
      const throw()
   { return -1; }
   // ----------------------------------------------------------------------
   int
   SystemResources::
   system_time_seconds( void )
      const throw()
   { return -1; }
   // ----------------------------------------------------------------------
   int
   SystemResources::
   time_seconds( void )
      const throw()
   { return -1; }

#endif

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/sys_resources.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: sys_resources.cpp,v $
 *-----------------------------------------------------------------------*/
