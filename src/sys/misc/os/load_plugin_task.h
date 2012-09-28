/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_OS_LOAD_PLUGIN_TASK_H
#define __SHAWN_SYS_MISC_OS_LOAD_PLUGIN_TASK_H

#include "sys/simulation/simulation_task.h"
#include <stdexcept>
#include <list>

#ifdef WIN32
#ifdef ERROR
#undef ERROR
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace shawn
{

   class LoadPluginTask
      : public SimulationTask
   {
   public:
      typedef void(*PluginInitFunc)(SimulationController&);
#ifdef WIN32
      typedef HINSTANCE OSPluginHandle;
#else
      typedef void* OSPluginHandle;
#endif

      LoadPluginTask();
      virtual ~LoadPluginTask();
      virtual std::string name( void )
         const throw();
      virtual std::string description( void )
         const throw();
      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:

      void add_plugin_handle( OSPluginHandle ) throw();


      PluginInitFunc load_plugin( const std::string& plugin_name,
                                  const std::string& file_name ) throw(std::runtime_error);
      void unload_plugin( OSPluginHandle ) throw();

      
#ifndef WIN32
      OSPluginHandle unix_load_plugin_object( const std::string& ) throw( std::runtime_error );
      OSPluginHandle unix_load_plugin_object_list( std::list<std::string>& ) throw( std::runtime_error );
#endif

   private:
      std::list<OSPluginHandle> plugin_handles_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/load_plugin_task.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: load_plugin_task.h,v $
 *-----------------------------------------------------------------------*/
