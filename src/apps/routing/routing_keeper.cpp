#include "routing_keeper.h"
#ifdef ENABLE_ROUTING

#include "sys/simulation/simulation_controller.h"

#include "apps/routing/routing.h"
#include "apps/routing/routing_keeper.h"
#include "apps/routing/routing_factory.h"

namespace routing
{
	// RoutingKeeper:

	const std::string RoutingKeeper::KEEPER_NAME( "routing" );
	// ----------------------------------------------------------------------
	RoutingKeeper::
		RoutingKeeper() : shawn::HandleKeeper<RoutingBase>( KEEPER_NAME, "Routing protocols" )
	{}
	// ----------------------------------------------------------------------
	RoutingKeeper::
		~RoutingKeeper()
	{}
	// ----------------------------------------------------------------------
	const RoutingKeeper&
		routing_keeper(const shawn::SimulationController& sc)
		throw()
	{
		const RoutingKeeper* rk = sc.keeper_by_name<RoutingKeeper>(RoutingKeeper::KEEPER_NAME);
		assert( rk );
		return *rk;
	}
	// ----------------------------------------------------------------------
	RoutingKeeper&
		routing_keeper_w(shawn::SimulationController& sc)
		throw()
	{
		RoutingKeeper* rk = sc.keeper_by_name_w<RoutingKeeper>(RoutingKeeper::KEEPER_NAME);
		assert( rk );
		return *rk;
	}

	// RoutingFactoryKeeper:

	const std::string RoutingFactoryKeeper::KEEPER_NAME( "routing_factory" );
	// ----------------------------------------------------------------------
	RoutingFactoryKeeper::
		RoutingFactoryKeeper() : shawn::HandleKeeper<RoutingFactory>( KEEPER_NAME, "Routing factories" )
	{}
	// ----------------------------------------------------------------------
	RoutingFactoryKeeper::
		~RoutingFactoryKeeper()
	{}
	// ----------------------------------------------------------------------
	const RoutingFactoryKeeper&
		routing_factory_keeper(const shawn::SimulationController& sc)
		throw()
	{
		const RoutingFactoryKeeper* rfk = sc.keeper_by_name<RoutingFactoryKeeper>(RoutingFactoryKeeper::KEEPER_NAME);
		assert( rfk );
		return *rfk;
	}
	// ----------------------------------------------------------------------
	RoutingFactoryKeeper&
		routing_factory_keeper_w(shawn::SimulationController& sc)
		throw()
	{
		RoutingFactoryKeeper* rfk = sc.keeper_by_name_w<RoutingFactoryKeeper>(RoutingFactoryKeeper::KEEPER_NAME);
		assert( rfk );
		return *rfk;
	}

}

#endif
