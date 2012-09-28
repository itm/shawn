/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_FACTORY_INIT_H
#define __SHAWN_APPS_ROUTING_FACTORY_INIT_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/util/keeper_managed.h"

namespace shawn{ class SimulationController; }

namespace routing
{
	DECLARE_HANDLES(RoutingFactory);

	class RoutingBase;

	class RoutingFactory : public shawn::KeeperManaged
	{
	public:
		virtual ~RoutingFactory();
		virtual RoutingBase* create( const std::string& name, shawn::SimulationController& sc ) throw() = 0;
	};
}

#endif
#endif

