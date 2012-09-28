/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_LOGGING_TASK_RECONFIGURE_H
#define __SHAWN_SYS_LOGGING_TASK_RECONFIGURE_H

#include "sys/world.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"


namespace shawn
{
   /// Task reconfiguring logging libraries at runtime
   /** This task allows reconfiguring logging libraries at runtime.
    *
    *  Just type one of the following exemplary lines to reconfigure logging.
    *  \code
    *    logging_reconfigure action=set_level logger=MyLoggerName level=[debug,info,...,fatal]
    *    logging_reconfigure action=set_additivity logger=MyLoggerName additivity=[true|false]
    *  \endcode
    *
    *  Moreover, by using log4cxx, there is the chance a add/remove different
    *  appenders (up to now \c rm_appenders removes <b>all</b> appenders.).
    *  \code
    *    logging_reconfigure action=rm_appender logger=MyLoggerName
    *    logging_reconfigure action=add_appender \
    *       logger=MyLoggerName                  \
    *       appender=Appender1,Appender2,... (whereby AppenderX=[ConsoleAppender|FileAppender]) \
    *       layout=[SimpleLayout|TTCCLayout|StdLayout|StdUserLayout|XMLLayout]     \
    *       append=[true|false] logfile=Filename (these ones are just used by FileAppender)
    *  \endcode
    *
    *  <b>Attention</b>: Reconfiguration just works with the external logging
    *  libraries log4cxx and log4cplus. If you use internal logging functions
    *  reconfiguring will not have any effect. The only way to influence
    *  internal logging is defining \c DISABLE_LOGLEVEL_DEBUG or
    *  \c DISABLE_LOGLEVEL_INFO.
    */
   class SimulationTaskLoggingReconfigure
      : public SimulationTask
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      SimulationTaskLoggingReconfigure();
      virtual ~SimulationTaskLoggingReconfigure();
      ///@}

      ///@name basic task methods
      ///@{
      /** Main function which is called when starting this task. This case,
       *  methods reconfiguring logging libraries are called.
       *  \param sc SimulationController to access the whole simulation
       *  \sa SimulationTask::run()
       */
      virtual void run( SimulationController& sc) throw( std::runtime_error );
      /** \return Name of task. This case <em>logging_reconfigure</em>
       *  \sa KeeperManaged::name()
       */
      virtual std::string name( void ) const throw();
      /** \return Description of task.
       *  \sa KeeperManaged::description()
       */
      virtual std::string description( void ) const throw();
      ///@}


   private:

      ///@name reconfiguring methods
      ///@{
      /** Method which sets the debug level of a given logger. It reads
       *  simulation parameters <em>logger</em> and <em>level</em>.
       *  First, <em>logger</em>, just describes the logger which is going to
       *  be configured. Second argument <em>level</em> is the level to be
       *  set, namely <em>debug, info, warn, error</em> or <em>fatal</em>.
       *  \param SimulationController enables access to whole simulation
       */
      void set_debug_level( SimulationController& ) throw( std::runtime_error );
      /** Method which sets the additivity of a logger. It reads
       *  simulation parameters <em>logger</em> and <em>additivity</em>.
       *  First, <em>logger</em>, just describes the logger which is going to
       *  be configured. Second argument <em>additivity</em> should be set to
       *  \c true or \c false.
       *  \param SimulationController enables access to whole simulation
       */
      void set_additivity( SimulationController& ) throw( std::runtime_error );
      /** Method which removes appender(s) of a logger.
       *  \param SimulationController enables access to whole simulation
       */
      void rm_appender( SimulationController& ) throw( std::runtime_error );
      /** Method which adds appender(s) to a logger, e.g. ConsoleAppender or
       *  FileAppender, with each given a special layout.
       *  \param SimulationController enables access to whole simulation
       */
      void add_appender( SimulationController& ) throw( std::runtime_error );
#if defined ( LOGGER_LOG4CXX )
      /**
       */
      log4cxx::LayoutPtr layout( SimulationController&, const std::string& ) throw();
#endif
      ///@}

   };

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging_task_reconfigure.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: logging_task_reconfigure.h,v $
 *-----------------------------------------------------------------------*/
