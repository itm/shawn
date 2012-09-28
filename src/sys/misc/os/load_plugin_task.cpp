/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "sys/misc/os/load_plugin_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"



namespace shawn
{

   LoadPluginTask::
   LoadPluginTask()
   {}
   // ----------------------------------------------------------------------
   LoadPluginTask::
   ~LoadPluginTask()
   {
      while( !plugin_handles_.empty() )
         {
            unload_plugin(plugin_handles_.back());
            plugin_handles_.pop_back();
         }
   }
   // ----------------------------------------------------------------------
   std::string
   LoadPluginTask::
   name( void )
      const throw()
   { return std::string("load_plugin"); }
   // ----------------------------------------------------------------------
   std::string
   LoadPluginTask::
   description( void )
      const throw()
   { return std::string("loads the shawn plugin $plugin. filename can be passed as $file, or be guessed automatically"); }
   // ----------------------------------------------------------------------
   void
   LoadPluginTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      std::string pluginbase =
         sc.environment().required_string_param("plugin");
      std::string file =
         sc.environment().optional_string_param("file","");
      PluginInitFunc pif = load_plugin(pluginbase,file);
      assert( pif != NULL ); // error handling must be done in load_plugin()
      (*pif)(sc);
   }
   // ----------------------------------------------------------------------
   void
   LoadPluginTask::
   add_plugin_handle( OSPluginHandle h )
      throw()
   { plugin_handles_.push_back(h); }
   // ----------------------------------------------------------------------
   void
   LoadPluginTask::
   unload_plugin( LoadPluginTask::OSPluginHandle h )
      throw()
   {
#ifdef WIN32
      FreeLibrary(h);
#else
      dlclose(h);
#endif
   }
   // ----------------------------------------------------------------------

#ifdef WIN32
   LoadPluginTask::PluginInitFunc
   LoadPluginTask::
   load_plugin( const std::string& plugin_name,
                const std::string& file_name )
      throw(std::runtime_error)
   {
     std::string lib_real_name = plugin_name + ".dll";
     OSPluginHandle handle = LoadLibrary(lib_real_name.c_str());

      if( handle == NULL ) 
         throw std::runtime_error("Error while loading library: ");

      std::string init_func_name = std::string("init_")+plugin_name;
      PluginInitFunc pif = (PluginInitFunc)
	GetProcAddress(handle, init_func_name.c_str());
      if( pif == NULL)
	{
	  std::cerr << GetLastError();
	  throw std::runtime_error("Didn't find plugin init function\n");
	}
      add_plugin_handle(handle);
      return pif;
   }


#else
   LoadPluginTask::OSPluginHandle
   LoadPluginTask::
   unix_load_plugin_object( const std::string& file )
      throw( std::runtime_error )
   {
      OSPluginHandle h = dlopen( file.c_str(), RTLD_LAZY|RTLD_GLOBAL );
      const char* err = dlerror();
      if( err != NULL )
         throw std::runtime_error(file+std::string(": ")+std::string(err));
      return h;
   }
   // ----------------------------------------------------------------------
   LoadPluginTask::OSPluginHandle
   LoadPluginTask::
   unix_load_plugin_object_list( std::list<std::string>& files )
      throw( std::runtime_error )
   {
      std::string errseq("");
      OSPluginHandle handle;

      while( !files.empty() )
         {
            std::string file = files.front();
            files.pop_front();

            try {
               handle = unix_load_plugin_object(file);
               return handle;
            }
            catch( std::runtime_error& re ) {
               if( !errseq.empty() ) errseq += std::string("; ");
               errseq += re.what();
            }
         }
      throw std::runtime_error(errseq);
   }
   // ----------------------------------------------------------------------
   LoadPluginTask::PluginInitFunc
   LoadPluginTask::
   load_plugin( const std::string& plugin_name,
                const std::string& file_name )
      throw(std::runtime_error)
   {
      OSPluginHandle handle;
      std::string errseq;

      std::list<std::string> files;
      if( !file_name.empty() ) files.push_back(file_name);
      
	  //files.push_back(std::string(PLUGIN_PREFIX)+plugin_name+std::string(PLUGIN_INFIX)+std::string(PLUGIN_SUFFIX));
      //files.push_back(std::string(PLUGIN_PREFIX)+plugin_name+std::string(PLUGIN_SUFFIX));

	  #warning Does not work! (prefix, infix, suffix)
	  files.push_back(std::string("prefix")+plugin_name+std::string("infix")+std::string("suffix"));
      files.push_back(std::string("prefix")+plugin_name+std::string("suffix"));
	  

      try {
         handle = unix_load_plugin_object_list(files);
      } catch( std::runtime_error& re ) {
         throw std::runtime_error(std::string("cannot load plugin ") +
                                  plugin_name +
                                  std::string(" (") +
                                  re.what() +
                                  std::string(") -- provide absolute path names with file= or change LD_LIBRARY_PATH in the environment.") );
      }

      dlerror();
      std::string init_func_name = std::string("init_")+plugin_name;
      PluginInitFunc pif = (PluginInitFunc)dlsym(handle, init_func_name.c_str());
      const char* err = dlerror();
      if( err != NULL )
         {
            dlclose(handle);
            throw std::runtime_error(std::string("no plugin initializer in plugin: ") + 
                                     err );
         }

      add_plugin_handle(handle);
      return pif;
   }
#endif



}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/os/load_plugin_task.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: load_plugin_task.cpp,v $
 *-----------------------------------------------------------------------*/
