/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/stochastic_comm_model.h"
#include "sys/comm_models/stochastic_comm_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <limits>
#include <iostream>
#include <cassert>

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	StochasticCommunicationModelFactory::
		~StochasticCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		StochasticCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "stochastic";
	}

	// ----------------------------------------------------------------------
	std::string 	
		StochasticCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "Stochastic Communication Model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		StochasticCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		double max_probability = sc.environment().optional_double_param("max_probability", 1.0 );
		double range = sc.environment().optional_double_param("range", std::numeric_limits<int>::max() );
		double sf = sc.environment().optional_double_param("smooth_factor", 0.25 );

		assert(sf > 0);
		assert (sf <= 1.0);
		assert(max_probability > 0);
		assert (max_probability <= 1.0);
		
		StochasticCommunicationModel* scm = new StochasticCommunicationModel( max_probability, sf );;

		if( range != std::numeric_limits<int>::max() )
			scm->set_transmission_range(range);
		/*
		for (int i = 0; i < 2 * range; i++)
		{
			std::cout << "d=" << i << " p=" << scm->communication_probability(i) << std::endl;
		}
		*/
		return scm;
	}



}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/stochastic_comm_model_factory.cpp,v $
* Version $Revision: 298 $
* Date    $Date: 2008-12-04 17:53:45 +0100 (Thu, 04 Dec 2008) $
*-----------------------------------------------------------------------
* $Log: stochastic_comm_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
