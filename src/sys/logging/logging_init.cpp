/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/logging/logging_init.h"
#include "sys/logging/logging_task_load_cfg.h"
#include "sys/logging/logging_task_reconfigure.h"

#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"


#if defined ( LOGGER_LOG4CXX )
   #include "log4cxx/logger.h"
   #include "log4cxx/consoleappender.h"
   #include "log4cxx/patternlayout.h"
#elif defined ( LOGGER_LOG4CPLUS )
   #include "log4cplus/logger.h"
   #include "log4cplus/consoleappender.h"
   #include "log4cplus/layout.h"
#endif


namespace shawn
{

   void
   init_logging( SimulationController& sc )
      throw()
   {
      sc.simulation_task_keeper_w().add( new SimulationTaskLoggingLoadCfg );
      sc.simulation_task_keeper_w().add( new SimulationTaskLoggingReconfigure );

#if defined ( LOGGER_LOG4CXX )
      log4cxx::LoggerPtr root_logger = log4cxx::Logger::getRootLogger();
      root_logger->addAppender( new log4cxx::ConsoleAppender(
         new log4cxx::PatternLayout( "%c{1} - %m%n" ), "System.Out" ) );

      log4cxx::LoggerPtr user_logger = log4cxx::Logger::getLogger( LOG_USER_NAME );
      user_logger->addAppender( new log4cxx::ConsoleAppender(
         new log4cxx::PatternLayout( "%m%n" ), "System.Out" ) );
      user_logger->setAdditivity( false );
#elif defined ( LOGGER_LOG4CPLUS )
      log4cplus::SharedAppenderPtr root_appender( new log4cplus::ConsoleAppender() );
      log4cplus::SharedAppenderPtr user_appender( new log4cplus::ConsoleAppender() );
      root_appender->setName( "default_root_console" );
      user_appender->setName( "default_user_console" );

      std::auto_ptr<log4cplus::Layout> root_layout =
         std::auto_ptr<log4cplus::Layout>( new log4cplus::PatternLayout( "%c{1} - %m%n" ) );
      std::auto_ptr<log4cplus::Layout> user_layout =
         std::auto_ptr<log4cplus::Layout>( new log4cplus::PatternLayout( "%m%n" ) );

      root_appender->setLayout( root_layout );
      user_appender->setLayout( user_layout );

      log4cplus::Logger root_logger = log4cplus::Logger::getRoot();
      log4cplus::Logger user_logger = log4cplus::Logger::getInstance( LOG_USER_NAME );
      root_logger.addAppender( root_appender );
      user_logger.addAppender( user_appender );
      user_logger.setAdditivity( false );
#endif
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging_init.cpp,v $
 * Version $Revision: 480 $
 * Date    $Date: 2010-12-09 13:18:05 +0100 (Thu, 09 Dec 2010) $
 *-----------------------------------------------------------------------
 * $Log: logging_init.cpp,v $
 *-----------------------------------------------------------------------*/
