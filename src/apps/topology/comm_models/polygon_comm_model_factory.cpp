
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"

#include "apps/topology/comm_models/polygon_comm_model_factory.h"
#include "apps/topology/comm_models/polygon_comm_model.h"
#include "apps/topology/topology/polygon_topology.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "apps/topology/topology_keepers.h"
#include <limits>
#include <iostream>
using namespace std;
using namespace shawn;
namespace topology
{
PolygonTopologyCommunicationModelFactory::
	PolygonTopologyCommunicationModelFactory(shawn::SimulationController* sc)
	:sc_(sc)
{}
	// ----------------------------------------------------------------------
	PolygonTopologyCommunicationModelFactory::
		~PolygonTopologyCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		PolygonTopologyCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "polygon_topology";
	}

	// ----------------------------------------------------------------------
	std::string 	
		PolygonTopologyCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "polygon_topology_communication_model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		PolygonTopologyCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
			string topology_name = sc.environment().required_string_param("topology");
		double lower = sc.environment().required_double_param("lower_bound");
		double upper = sc.environment().required_double_param("upper_bound");
		string fname = sc.environment().optional_string_param("file_name","");
		string rssi_dist = sc.environment().optional_string_param("rssi_dist","");
//		bool attenuation = sc.environment().optional_bool_param("with_attenuation", false);

		//Retrieve the topology by the supplied name
		reading::ConstReadingHandle h = topology::topology_keeper(sc).find(topology_name);
		assert( h.is_not_null() );
		const PolygonTopology* pt = dynamic_cast<const PolygonTopology*>( h.get() );

		if( pt == NULL )
		{
			cerr << "The supplied topology (" << topology_name << "( does not exist or is not of type PolygonTopology" << endl;
			exit(1);
		}

		//Parameterize the newly created model
		PolygonTopologyCommunicationModel* cm = new PolygonTopologyCommunicationModel(*pt,upper,lower,fname,rssi_dist/*,attenuation*/);
		cm->set_transmission_range(upper);
		return cm;

	}


}



#endif
