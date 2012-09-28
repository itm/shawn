/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/disk_graph_model.h"
#include "sys/comm_models/disk_graph_communication_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <limits>

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	DiskGraphCommunicationModelFactory::
		~DiskGraphCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		DiskGraphCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "disk_graph";
	}

	// ----------------------------------------------------------------------
	std::string 	
		DiskGraphCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "Disk graph Communication Model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		DiskGraphCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		double range = sc.environment().optional_double_param("range", std::numeric_limits<int>::max() );
		DiskGraphModel* dgm = new DiskGraphModel;

		if( range != std::numeric_limits<int>::max() )
			dgm->set_transmission_range(range);

		return dgm;
	}


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/disk_graph_communication_model_factory.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: disk_graph_communication_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
