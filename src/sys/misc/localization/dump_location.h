/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_LOCALIZATION_DUMP_LOCATION_H
#define __SHAWN_SYS_MISC_LOCALIZATION_DUMP_LOCATION_H

#include "sys/simulation/simulation_task.h"

#include <string>
#include <iostream>

namespace shawn
{
   class Node;
   class SimulationController;

   class SimulationTaskDumpLocation
      : public SimulationTask
   {

   public:
      SimulationTaskDumpLocation();
      virtual ~SimulationTaskDumpLocation();
      virtual void run( SimulationController& ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual void print_label( std::ostream&, const Node& ) throw();
      virtual void print_pos( std::ostream&, const Node& ) throw();

   private:
      bool dump_estimates_;
      bool skip_unpositioned_;
      std::string unpositioned_mark_;
      std::string colsep_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/localization/dump_location.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: dump_location.h,v $
 *-----------------------------------------------------------------------*/
