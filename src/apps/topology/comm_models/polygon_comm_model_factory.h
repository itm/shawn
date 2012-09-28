
#ifndef __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_FACTORY_H
#define __SHAWN_APPS_TOPOLOGY_COMM_MODELS_TOPOLOGY_COMM_MODEL_FACTORY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"

#include "sys/simulation/simulation_controller.h"
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/communication_model.h"
#include "sys/comm_models/communication_model_factory.h"
#include <string>
#include <vector>

namespace shawn
{
   class CommunicationModel;
   class SimulationController;
}
	//----------------------------------------------------------------------------
	/***
	  * This Factory provides PolygonTopologyCommunicationModelFactory
	  *
	  */
namespace topology{
	class PolygonTopologyCommunicationModelFactory
		: public shawn::CommunicationModelFactory
	{
	public:
		PolygonTopologyCommunicationModelFactory(shawn::SimulationController*);
		virtual ~PolygonTopologyCommunicationModelFactory();
		virtual shawn::CommunicationModel* create( const shawn::SimulationController& ) const throw();
		virtual std::string name(void)  const throw();
		virtual std::string description(void) const throw();
	private:
		shawn::SimulationController* sc_;

	};

}


#endif
#endif
