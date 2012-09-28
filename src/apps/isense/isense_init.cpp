/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "isense_init.h"
#ifdef ENABLE_ISENSE

#include "apps/isense/isense_init.h"
#include "sys/simulation/simulation_controller.h"

extern void isense_ext_application_init(shawn::SimulationController&);

extern "C" void init_isense( shawn::SimulationController& sc )
{
  std::cout << "isense init" << std::endl;
  isense_ext_application_init(sc);
}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/isense/isense_init.cpp,v $
 * Version $Revision: 67 $
 * Date    $Date: 2007-09-14 14:53:58 +0200 (Fr, 14 Sep 2007) $
 *-----------------------------------------------------------------------
 * $Log: isense_init.cpp,v $
 *-----------------------------------------------------------------------*/
