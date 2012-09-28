/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/external_application/external_application.h"
#ifdef ENABLE_EXTERNAL_APPLICATION

#include <iostream>
#include "sys/simulation/simulation_controller.h"

extern void init_external_app(shawn::SimulationController&);

extern "C" void init_external_application( shawn::SimulationController& sc )
{
   std::cout << "Initialising External_Application  Shawn module" << std::endl;
   init_external_app( sc );
}

#endif
