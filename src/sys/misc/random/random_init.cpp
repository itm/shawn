/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/


#include "sys/misc/random/random_seed.h"
#include "sys/misc/random/random_init.h"
#include "sys/misc/random/task_create_uniform.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/misc/random/normal_random_variable.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/misc/random/task_create_normal.h"


namespace shawn
{

   void
   init_default_random_variables( SimulationController& sc )
      throw()
   {
      UniformRandomVariable* urv;

      // uni[0;1)
      urv=new UniformRandomVariable;
      urv->set_lower_bound( 0.0 );
      urv->set_upper_bound( 1.0 );
      urv->set_lower_bound_inclusive( true );
      urv->set_upper_bound_inclusive( false );
      urv->init();
      sc.random_variable_keeper_w().add(urv);

      // uni[0;1]
      urv=new UniformRandomVariable;
      urv->set_lower_bound( 0.0 );
      urv->set_upper_bound( 1.0 );
      urv->set_lower_bound_inclusive( true );
      urv->set_upper_bound_inclusive( true );
      urv->init();
      sc.random_variable_keeper_w().add(urv);

      // uni[-1,1]
      urv=new UniformRandomVariable;
      urv->set_lower_bound( -1.0 );
      urv->set_upper_bound( 1.0 );
      urv->set_lower_bound_inclusive( true );
      urv->set_upper_bound_inclusive( true );
      urv->init();
      sc.random_variable_keeper_w().add(urv);      

      NormalRandomVariable* nrv;
      nrv=new NormalRandomVariable;
      nrv->set_mean(0.0);
      nrv->set_variance(1.0);
      nrv->init();
      sc.random_variable_keeper_w().add(nrv);
   }
   // ----------------------------------------------------------------------
   void
   init_misc_random( SimulationController& sc )
      throw()
   {
      sc.simulation_task_keeper_w().add( new SimulationTaskRandomSeed );
      sc.simulation_task_keeper_w().add( new SimulationTaskCreateUniform );
      sc.simulation_task_keeper_w().add( new SimulationTaskCreateNormal );

      init_default_random_variables(sc);
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/random_init.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: random_init.cpp,v $
 *-----------------------------------------------------------------------*/
