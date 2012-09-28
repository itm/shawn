/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "shawn_config.h"
#include "sys/logging/logging_task_reconfigure.h"
#include "sys/misc/tokenize.h"

#if defined ( LOGGER_LOG4CXX )
   #include "log4cxx/logger.h"
   #include "log4cxx/fileappender.h"
   #include "log4cxx/consoleappender.h"
   #include "log4cxx/simplelayout.h"
   #include "log4cxx/ttcclayout.h"
   #include "log4cxx/xml/xmllayout.h"
   #include "log4cxx/patternlayout.h"
#elif defined ( LOGGER_LOG4CPLUS )
   #include "log4cplus/logger.h"
   #include "log4cplus/loglevel.h"
#endif

namespace shawn
{

   SimulationTaskLoggingReconfigure::
   SimulationTaskLoggingReconfigure()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskLoggingReconfigure::
   ~SimulationTaskLoggingReconfigure()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLoggingReconfigure::
   name( void )
      const throw()
   {
      return "logging_reconfigure";
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskLoggingReconfigure::
   description( void )
      const throw()
   {
      return "Reconfigure logging library. Add, delete or change logging properties.";
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingReconfigure::
   run( shawn::SimulationController& sc )
      throw ( std::runtime_error )
   {
      require_world( sc );

      std::string action = sc.world().simulation_controller().environment().
         required_string_param( "action" );

      if ( action == "set_level" )
         set_debug_level( sc );
      else if ( action == "set_additivity" )
         set_additivity( sc );
      else if ( action == "rm_appender" )
         rm_appender( sc );
      else if ( action == "add_appender" )
         add_appender( sc );
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingReconfigure::
   set_debug_level( shawn::SimulationController& sc )
      throw ( std::runtime_error )
   {
      std::string logger_name = sc.world().simulation_controller().environment().
         required_string_param( "logger" );
      std::string level = sc.world().simulation_controller().environment().
         required_string_param( "level" );

#if defined ( LOGGER_LOG4CXX )
      log4cxx::LoggerPtr logger;
      if ( logger_name == "root")
         logger = log4cxx::Logger::getRootLogger();
      else
         logger = log4cxx::Logger::getLogger( logger_name );

      logger->setLevel( log4cxx::Level::toLevel( level ) );
#elif defined ( LOGGER_LOG4CPLUS )
      log4cplus::Logger logger = log4cplus::Logger::getInstance( logger_name );
      if ( logger_name == "root")
         logger = log4cplus::Logger::getRoot();

      if ( level == "debug" )
         logger.setLogLevel( log4cplus::DEBUG_LOG_LEVEL );
      else if ( level == "info" )
         logger.setLogLevel( log4cplus::INFO_LOG_LEVEL );
      else if ( level == "warn" )
         logger.setLogLevel( log4cplus::WARN_LOG_LEVEL );
      else if ( level == "error" )
         logger.setLogLevel( log4cplus::ERROR_LOG_LEVEL );
      else if ( level == "fatal" )
         logger.setLogLevel( log4cplus::FATAL_LOG_LEVEL );
      else
      {
         USER( "unknown loglevel. set to 'debug'." );
         logger.setLogLevel( log4cplus::DEBUG_LOG_LEVEL );
      }
#endif
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingReconfigure::
   set_additivity( shawn::SimulationController& sc )
      throw ( std::runtime_error )
   {
      std::string logger_name = sc.world().simulation_controller().environment().
		  required_string_param( "logger" );
		bool additivity = sc.world().simulation_controller().environment().
		  optional_bool_param( "additivity", true );

#if defined ( LOGGER_LOG4CXX )
      log4cxx::LoggerPtr logger;
      if ( logger_name == "root")
         logger = log4cxx::Logger::getRootLogger();
      else
         logger = log4cxx::Logger::getLogger( logger_name );

      logger->setAdditivity( additivity );
#elif defined ( LOGGER_LOG4CPLUS )
      log4cplus::Logger logger = log4cplus::Logger::getInstance( logger_name );
      if ( logger_name == "root")
         logger = log4cplus::Logger::getRoot();

      logger.setAdditivity( additivity );
#endif
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingReconfigure::
   rm_appender( shawn::SimulationController& sc )
      throw ( std::runtime_error )
   {
      std::string logger_name = sc.world().simulation_controller().environment().
         required_string_param( "logger" );

#if defined ( LOGGER_LOG4CXX )
      log4cxx::LoggerPtr logger;
      if ( logger_name == "root")
         logger = log4cxx::Logger::getRootLogger();
      else
         logger = log4cxx::Logger::getLogger( logger_name );

      logger->removeAllAppenders();

//       TODO: Remove appenders by name. Extend the add_appender method to give
//             each appender a name via set_name().
//       for ( StrTok::iterator
//                it = tok.begin();
//                it != tok.end();
//                ++it )
//       {
//          logger->removeAppender( *it );
//       }
#elif defined ( LOGGER_LOG4CPLUS )
      WARN( logger(), "Removing of appenders is not implemented yet." );
#endif
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskLoggingReconfigure::
   add_appender( shawn::SimulationController& sc )
      throw ( std::runtime_error )
   {
      std::string logger_name = sc.world().simulation_controller().environment().
         required_string_param( "logger" );
      std::string appender    = sc.world().simulation_controller().environment().
         required_string_param( "appender" );
      std::string layout_name = sc.world().simulation_controller().environment().
         required_string_param( "layout" );

      StrTok tok( appender, ", " );

#if defined ( LOGGER_LOG4CXX )
      log4cxx::LoggerPtr logger;
      if ( logger_name == "root")
         logger = log4cxx::Logger::getRootLogger();
      else
         logger = log4cxx::Logger::getLogger( logger_name );

      for ( StrTok::iterator
               it = tok.begin();
               it != tok.end();
               ++it )
      {
         if ( *it == "ConsoleAppender" )
         {
            log4cxx::ConsoleAppenderPtr con_app = new log4cxx::ConsoleAppender();
            con_app->setLayout( layout( sc, layout_name ) );

            logger->addAppender( con_app );
         }
         else if ( *it == "FileAppender" )
         {
            std::string logfile = sc.world().simulation_controller().environment().
               optional_string_param( "logfile", "log4xx-tmp.log" );
            bool append = sc.world().simulation_controller().environment().
               optional_bool_param( "append", false );

            log4cxx::LayoutPtr layout_ptr = layout( sc, layout_name );
            log4cxx::FileAppenderPtr file_app = new log4cxx::FileAppender( layout_ptr, logfile, append );

            logger->addAppender( file_app );
         }
         else
         {
            throw std::runtime_error( "Appender '" + *it + "' not known." );
         }
      }
#elif defined ( LOGGER_LOG4CPLUS )
      WARN( logger(), "Alterating of appenders is not implemented yet." );
//       log4cplus::Logger logger = log4cplus::Logger::getInstance( logger_name );
//       if ( logger_name == "root")
//          logger = log4cplus::Logger::getRoot();
//
//       logger.setAdditivity( additivity );
#endif
   }
   // ----------------------------------------------------------------------
#if defined ( LOGGER_LOG4CXX )
   log4cxx::LayoutPtr
   SimulationTaskLoggingReconfigure::
   layout( SimulationController& sc, const std::string& name )
      throw()
   {
      if ( name == "SimpleLayout" )
      {
         return new log4cxx::SimpleLayout();
      }
      else if ( name == "TTCCLayout" )
      {
         return new log4cxx::TTCCLayout();
      }
      else if ( name == "XMLLayout" )
      {
         return new log4cxx::xml::XMLLayout();
      }
      else if ( name == "StdLayout" )
      {
         return new log4cxx::PatternLayout( "%c{1} - %m%n" );
      }
      else if ( name == "StdUserLayout" )
      {
         return new log4cxx::PatternLayout( "%m%n" );
      }
      else
      {
         throw std::runtime_error( "Layout '" + name + "' not known." );
      }
   }
#endif

}


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging_task_reconfigure.cpp,v $
 * Version $Revision: 485 $
 * Date    $Date: 2011-01-12 13:57:34 +0100 (Wed, 12 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: logging_task_reconfigure.cpp,v $
 *-----------------------------------------------------------------------*/
