/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_LOGGING_TASK_LOAD_CFG_H
#define __SHAWN_SYS_LOGGING_TASK_LOAD_CFG_H

#include "sys/world.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_controller.h"


namespace shawn 
{
   /// Task that loads a logging library configuration file
   /** This task is able to load configuration files of both used logging
    *  libraries log4cxx and log4cplus. There is a working example in
    *  <em>shawn/legacyapps/logging_demo/</em>
    *
    *  Just type the following exemplary line to load a configuration file:
    *  \code
    *    logging_load_cfg log_cfg_file=MyCfgFile
    *  \endcode
    */
   class SimulationTaskLoggingLoadCfg
      : public SimulationTask
   {

   public:

      ///@name construction / destruction
      ///@{
      ///
      SimulationTaskLoggingLoadCfg();
      virtual ~SimulationTaskLoggingLoadCfg();
      ///@}

      ///@name basic task methods
      ///@{
      /** Main function which is called when starting this task. This case,
       *  configuration file with name <em>log_cfg_file</em> is loaded.
       *  \param sc SimulationController to access the whole simulation
       *  \sa SimulationTask::run()
       */
      virtual void run( SimulationController& sc) throw( std::runtime_error );
      /** \return Name of task. This case <em>logging_load_cfg</em>
       *  \sa KeeperManaged::name()
       */
      virtual std::string name( void ) const throw();
      /** \return Description of task.
       *  \sa KeeperManaged::description()
       */
      virtual std::string description( void ) const throw();
      ///@}

   };

}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/logging/logging_task_load_cfg.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: logging_task_load_cfg.h,v $
 *-----------------------------------------------------------------------*/
