/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/logging/logging_task_load_cfg.h"

#if defined ( LOGGER_LOG4CXX )
   #include "log4cxx/logger.h"
   #include "log4cxx/basicconfigurator.h"
   #include "log4cxx/propertyconfigurator.h"
   #include "log4cxx/consoleappender.h"
   #include "log4cxx/rollingfileappender.h"
   #include "log4cxx/simplelayout.h"
   #include "log4cxx/patternlayout.h"
   #include "log4cxx/ttcclayout.h"
   #include "log4cxx/helpers/exception.h"
#elif defined ( LOGGER_LOG4CPLUS )
   #include "log4cplus/logger.h"
   #include "log4cplus/configurator.h"
#endif

namespace shawn
{

   SimulationTaskLoggingLoadCfg::
   SimulationTaskLoggingLoadCfg()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLoggingLoadCfg::
   ~SimulationTaskLoggingLoadCfg()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLoggingLoadCfg::
   name( void )
      const throw()
   {
      return "logging_load_cfg";
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLoggingLoadCfg::
   description( void )
      const throw()
   {
      return "Init logging library. Load properties from given file.";
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingLoadCfg::
   run ( shawn::SimulationController& sc)
      throw ( std::runtime_error )
   {
      require_world( sc );

      std::string cfg_file = sc.world().simulation_controller().environment().
         required_string_param( "log_cfg_file" );

#if defined ( LOGGER_LOG4CXX )
      // the stuff with the logger_dummy and resetConfiguration() is just a
      // little workaround, cause appenders/layouts which used in config file
      // can not be found by log4cxx, if they were not been created before.
      // don't know, why this happens. i will try to find this out.
      //
      // tbaum

      log4cxx::LoggerPtr logger_dummy =
         log4cxx::Logger::getLogger( "dummy" );

      logger_dummy->addAppender( new log4cxx::ConsoleAppender(
         new log4cxx::SimpleLayout(), "System.Out" ) );
      logger_dummy->addAppender( new log4cxx::ConsoleAppender(
         new log4cxx::PatternLayout(), "System.Out" ) );
      logger_dummy->addAppender( new log4cxx::ConsoleAppender(
         new log4cxx::TTCCLayout(), "System.Out" ) );

      logger_dummy->addAppender( new log4cxx::RollingFileAppender() );

      log4cxx::BasicConfigurator::resetConfiguration();

      log4cxx::PropertyConfigurator::configure( cfg_file );
#elif defined ( LOGGER_LOG4CPLUS )
      log4cplus::Logger::shutdown();
      log4cplus::PropertyConfigurator::doConfigure( cfg_file );
#endif
   }

}


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging_task_load_cfg.cpp,v $
 * Version $Revision: 480 $
 * Date    $Date: 2010-12-09 13:18:05 +0100 (Thu, 09 Dec 2010) $
 *-----------------------------------------------------------------------
 * $Log: logging_task_load_cfg.cpp,v $
 *-----------------------------------------------------------------------*/
