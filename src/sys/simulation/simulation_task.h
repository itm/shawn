/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_SIMULATION_SIMULATION_TASK_H
#define __SHAWN_SYS_SIMULATION_SIMULATION_TASK_H


#include "shawn_config.h"
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"

#include <ostream>
#include <stdexcept>
#include <string>
#include <set>

namespace shawn
{
   class SimulationController;

   /**
     *
	 */
   class SimulationTaskResult
   {
   public:
	    virtual ~SimulationTaskResult() {}
		virtual std::string name() = 0;
		virtual std::string description() = 0;
		virtual void send_to(std::ostream&) = 0;
   };

   DECLARE_HANDLES(SimulationTask);
   /**
     *
	 */
   class SimulationTask
      : public KeeperManaged
   {
   public:
	  typedef std::set<SimulationTaskResult*> ResultSet;
	  typedef ResultSet::iterator ResultSetIt;

      virtual ~SimulationTask();

	  ///
      virtual void run( SimulationController& ) throw( std::runtime_error ) = 0;

	  ///
	  virtual ResultSetIt begin_results();
	  ///
	  virtual ResultSetIt end_results();
   protected:
	   ///
	  ResultSet results_;

      virtual void require_world( const SimulationController& ) const throw( std::runtime_error );
	  ///
	  virtual void add_result(SimulationTaskResult*);
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/simulation_task.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: simulation_task.h,v $
 *-----------------------------------------------------------------------*/
