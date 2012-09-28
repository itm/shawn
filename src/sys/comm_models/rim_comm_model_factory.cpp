/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/rim_comm_model.h"
#include "sys/comm_models/rim_comm_model_factory.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	RimCommModelFactory::
		~RimCommModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		RimCommModelFactory::
		name(void) 
		const throw()
	{
		return "rim";
	}

	// ----------------------------------------------------------------------
	std::string 	
		RimCommModelFactory::
		description(void) 
		const throw()
	{
		return std::string("Models the Radio Irregularity Model described in http://www.cs.virginia.edu/~th7c/Research/Publications/Papers/RAM_V11.pdf");
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		RimCommModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		const SimulationEnvironment& se = sc.environment();

		double alpha = se.optional_double_param("alpha", 3);
		double beta = se.optional_double_param("beta", 5);
		double mean = se.optional_double_param("mean", 0);
		double variance = se.optional_double_param("variance", 1);
		double range = se.optional_double_param("range", 10);
		double min_range = se.optional_double_param("min_range", 0.5);
		double max_range = se.optional_double_param("max_range", 1.5);
		double doi = se.optional_double_param("doi", 0.01);
		int granularity = se.optional_int_param("granularity", 1);
		double vsp = se.optional_double_param("vsp", 0);
		double epsilon = se.optional_double_param("epsilon", 0.05);
		int rotation = se.optional_int_param("rotation", 0);
		int model_count = se.optional_int_param("model_count", 1);

		cout << "range=" << range << " min_r=" << min_range << " max_r=" << max_range << " doi=" << doi << " epsilon=" << epsilon << " rotation=" << rotation << " granularity=" << granularity << endl;

		//TODO: Read Ki sequence from config value of file
		RimCommModel* r = new RimCommModel(alpha, beta, mean, variance, range, min_range, max_range, doi, granularity, vsp, epsilon, rotation, model_count);
		//cout << "factory running" << endl;
		if (!r->load_from_tag(se)) {
			//cout << "should now generate" << endl;
			r->generate_sequences();
//			r->print();
//			r->save_to_tag(se);
			r->save_to_file();
		}
		return r;
	}


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/rim_comm_model_factory.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: rim_comm_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
