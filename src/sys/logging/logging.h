/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_LOGGING_LOGGING_H
#define __SHAWN_SYS_LOGGING_LOGGING_H

#include "shawn_config.h"
#include <string>

/* If one of the known logger types is configured, the variable
 * ENABLE_LOGGING is defined. Otherwise it is left undefined. */

#define LOG_DEFAULT_NAME std::string("default")
#define LOG_USER_NAME std::string("user")

#if defined ( LOGGER_LOG4CXX )

   #define ENABLE_LOGGING

   #include "log4cxx/logger.h"

   #define __LOGGER_OBJECT log4cxx::LoggerPtr
   #define __INIT_LOGGER_OBJECT( logname ) log4cxx::Logger::getLogger( logname )

   #define USER( message ) LOG4CXX_INFO( log4cxx::Logger::getLogger( LOG_USER_NAME ), message )

   #define DEBUG( logger, message ) LOG4CXX_DEBUG( logger, message )
   #define INFO( logger, message ) LOG4CXX_INFO( logger, message )
   #define WARN( logger, message ) LOG4CXX_WARN( logger, message )
   #define ERROR( logger, message ) LOG4CXX_ERROR( logger, message )
   #define FATAL( logger, message ) LOG4CXX_FATAL( logger, message )

#elif defined ( LOGGER_LOG4CPLUS )

   #define ENABLE_LOGGING

   #include "log4cplus/logger.h"

   #define __LOGGER_OBJECT log4cplus::Logger
   #define __INIT_LOGGER_OBJECT( logname ) log4cplus::Logger::getInstance( logname )

   #define USER( message ) LOG4CPLUS_INFO( log4cplus::Logger::getInstance( LOG_USER_NAME ), message )

   #define DEBUG( logger, message ) LOG4CPLUS_DEBUG( logger, message )
   #define INFO( logger, message ) LOG4CPLUS_INFO( logger, message )
   #define WARN( logger, message ) LOG4CPLUS_WARN( logger, message )
   #define ERROR( logger, message ) LOG4CPLUS_ERROR( logger, message )
   #define FATAL( logger, message ) LOG4CPLUS_FATAL( logger, message )

#elif defined( LOGGER_NONE )

   // ENABLE_LOGGING is not defined in this case

   // Here, __LOGGER_OBJECT must be any type that does not hurt much.
   #define __LOGGER_OBJECT char
   #define __INIT_LOGGER_OBJECT( logname )

   #define USER( message ) std::cout << message << std::endl;

   #define DEBUG( logger, message )
   #define INFO( logger, message )
   #define WARN( logger, message )
   #define ERROR( logger, message )
   #define FATAL( logger, message )

#else

   // Default is logging to stdout.
   // (CMake sets LOGGER_STDOUT for completeness, but it is not used here.)

   #define ENABLE_LOGGING

   #include <iostream>

   #define __LOGGER_OBJECT std::string
   #define __INIT_LOGGER_OBJECT( logname ) logname

   #define USER( message ) std::cout << message << std::endl;

   // The debug level is configurable with CMake, if logging to STDOUT
   #ifndef LOG_DEBUG_DISABLED
      #define DEBUG( logger, message ) std::cout << logger << ": " << message << std::endl;
   #else
      #define DEBUG( logger, message )
   #endif

   // The debug level is configurable with CMake, if logging to STDOUT
   #ifndef LOG_INFO_DISABLED
      #define INFO( logger, message ) std::cout << logger << ": " << message << std::endl;
   #else
      #define INFO( logger, message )
   #endif

   #define WARN( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define ERROR( logger, message ) std::cout << logger << ": " << message << std::endl;
   #define FATAL( logger, message ) std::cout << logger << ": " << message << std::endl;

#endif /* defined ( LOGGER_STDOUT ) */

#endif /* __SHAWN_SYS_LOGGING_LOGGING_H */

/*-----------------------------------------------------------------------
 * Source  $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/sys/logging/logging.h $
 * Version $Revision: 480 $
 * Date    $Date: 2010-12-09 13:18:05 +0100 (Thu, 09 Dec 2010) $
 * Author  $Author: simsiem $
 *-----------------------------------------------------------------------*/
