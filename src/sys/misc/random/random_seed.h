/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_TASKS_RANDOM_SEED_H
#define __SHAWN_SYS_MISC_TASKS_RANDOM_SEED_H

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include <fstream>
#include <ctime>


namespace shawn {

  DECLARE_HANDLES(SimulationTaskRandomSeed);

  class SimulationTaskRandomSeed 
    : public SimulationTask

  {
  public:

    SimulationTaskRandomSeed();
    virtual ~SimulationTaskRandomSeed();
    virtual void run( SimulationController& ) throw(std::runtime_error);
    virtual std::string name( void ) const throw();
    virtual std::string description( void ) const throw();

    virtual long int seed( void ) const throw();

  private:

    long int seed_;
  };

} // namespace shawn


#endif
 

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_seed.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: random_seed.h,v $
 *-----------------------------------------------------------------------*/
