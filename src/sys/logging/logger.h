/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_LOGGING_LOGGER_H
#define __SHAWN_SYS_LOGGING_LOGGER_H

#include <string>
#include "sys/logging/logging.h"

namespace shawn 
{
   /// Main logging class
   /** This class offers logging features to nearly every class in this
    *  simulation framework. There is a chance to use existing logging
    *  libraries like log4cxx or log4cplus, or the use of the integrated
    *  logging features (which just put everything to stdout).
    *
    *  On the one hand there are already many classes which support
    *  logging functions (to call by name: everything derived from Processor
    *  and all classes in the KeeperManaged hierarchy like a
    *  SimulationTask). On the other hand you are able to enable the logging
    *  functions yourself. Both ways are described below.
    *  <hr>
    *
    *  <b>Setting up the right library</b>
    *
    *  To chose your favorated logging library, you have to add one of the
    *  following lines to the shawn_config.h:
    *  \code
    *    #define LOGGER_LOG4CXX
    *  \endcode
    *  to enable log4cxx,
    *  \code
    *    #define LOGGER_LOG4CPLUS
    *  \endcode
    *  to enable log4cplus, or
    *  \code
    *    #define DISABLE_LOGGING
    *  \endcode
    *  to disable logging completly (just the user-messages will be
    *  displayed and nothing of the logging stuff will be compiled, more
    *  precisely removed by preprocessor directives). If there is nothing
    *  written, the internal logging functions will be used.
    * 
    *  Moreover, if you want to remove debugging or info messages completly,
    *  just write on (or both) of the  following lines to your shwan_config.h:
    *  \code
    *    #define DISABLE_LOGLEVEL_DEBUG
    *    #define DISABLE_LOGLEVEL_INFO
    *  \endcode
    *  The affected lines will will not be compiled.
    *  <hr>
    * 
    *  <b>Using logging functions</b>
    * 
    *  Using the logging functions is quite simple. There is an example in
    *  <em>shawn/legacyapps/logging_demo/</em> where the standard logging
    *  functions are used. Moreover there are exemplary configuration files
    *  for use of log4cxx and log4cplus (namely log4cxx.cfg and
    *  log4cplus.cfg).
    * 
    *  As mentioned above, logging is not hard to be used. There are six
    *  different logging levels, which are implemented as macros:
    *  \code
    *    USER( message )
    *    DEBUG( logger, message )
    *    INFO( logger, message )
    *    WARN( logger, message )
    *    ERROR( logger, message )
    *    FATAL( logger, message )
    *  \endcode
    * 
    *  First, the USER macro is a simple output of the main user information
    *  (just like which iteration step is computed etc.) and should not be
    *  disabled. There is only the message needed, which is to be printed out.
    * 
    *  The other levels, from debug to fatal, need two arguments: At first the
    *  logger which is used to print out the message and second the message
    *  itself. Normally, a class is derived from Logger and owns an own logger
    *  object, so that, e.g. in a processor, the user is able to just call
    *  \code
    *    DEBUG( logger(), "My message" )
    *  \endcode
    * 
    *  Moreover there is the chance, particularly if a class is not derived
    *  from a Logger (and the user don't want to derive it from a Logger as
    *  described in next section), the user is allowed to take the logger
    *  object from another class, e.g. SimulationController:
    *  \code
    *    DEBUG( simulation_controller.logger(), "My message" )
    *  \endcode
    *  <hr>
    * 
    *  <b>Enable logging functions yourself</b>
    * 
    *  If you want to spend a class it's own logging functions, this is done
    *  in a few steps:
    *  At first you should include the header file of Logger as follows:
    *  \code
    *    #include "sys/logging/logger.h"
    *  \endcode
    *  Second, you have to derive your class from the Logger:
    *  \code
    *       class MyClass
    *    #ifdef ENABLE_LOGGING
    *         : public Logger
    *    #endif
    *  \endcode
    *  Third and last, but optional, you should set the name of the logger in
    *  constructor of your class as follows
    *  \code
    *    #ifdef ENABLE_LOGGING
    *       set_logger_name( "my_logger" );
    *    #endif
    *  \endcode
    *  to identify it in later use. If you forget to name it, there will be a
    *  standard name given.
    * 
    *  This mechanism had already been used in Processor, World,
    *  SimulationController and KeeperManaged, so that you could look at
    *  an working example.
    */
   class Logger
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      Logger();
      ///
      virtual ~Logger();
      ///@}

#ifdef ENABLE_LOGGING
      ///@name main logging functionality
      ///@{
      /** This method returns a logger object demanding on which logging
       * library has been enabled.
       * \return used logger object
       */
      __LOGGER_OBJECT& logger( void ) throw();
      /** This method returns a logger object demanding on which logging
       * library has been enabled.
       * \return used logger object
       */
      __LOGGER_OBJECT& logger( void ) const throw();
      /** Set the used name of the logger by which it is accessed from logging
       *  libraries configuration files.
       *  \param name new logger name
       */
      void set_logger_name( const std::string& name ) throw();
      /**  \return logger name
       */
      const std::string& logger_name( void ) const throw();
      ///@}
#endif

   private:

#ifdef ENABLE_LOGGING
      std::string logger_name_;
      mutable __LOGGER_OBJECT logger_;
#endif
      
   };

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logger.h,v $
 * Version $Revision: 525 $
 * Date    $Date: 2011-04-11 15:36:47 +0200 (Mon, 11 Apr 2011) $
 *-----------------------------------------------------------------------
 * $Log: logger.h,v $
 *-----------------------------------------------------------------------*/
