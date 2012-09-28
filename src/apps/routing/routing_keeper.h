/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_ROUTING_ROUTING_KEEPER_H
#define __SHAWN_APPS_ROUTING_ROUTING_KEEPER_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/util/handle_keeper.h"

#include "apps/routing/routing_factory.h"

namespace shawn{ class SimulationController; }

namespace routing
{
	// Keeper for instances
	class RoutingKeeper : public shawn::HandleKeeper<RoutingBase>
	{
	public:
		static const std::string KEEPER_NAME;
		RoutingKeeper();
		virtual ~RoutingKeeper();
	};

	// global getters
	const RoutingKeeper& routing_keeper( const shawn::SimulationController& ) throw();
	RoutingKeeper& routing_keeper_w( shawn::SimulationController& ) throw();

	// Keeper for factories
	class RoutingFactoryKeeper : public shawn::HandleKeeper<RoutingFactory>
	{
	public:
		static const std::string KEEPER_NAME;
		RoutingFactoryKeeper();
		virtual ~RoutingFactoryKeeper();
	};

	// global getters
	const RoutingFactoryKeeper& routing_factory_keeper( const shawn::SimulationController& ) throw();
	RoutingFactoryKeeper& routing_factory_keeper_w( shawn::SimulationController& ) throw();

}

#endif
#endif
