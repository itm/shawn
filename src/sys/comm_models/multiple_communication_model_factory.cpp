/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/communication_model.h"
#include "sys/comm_models/communication_model_factory.h"
#include "sys/comm_models/multiple_communication_model_factory.h"
#include "sys/comm_models/multiple_communication_model.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/util/defutils.h"
#include "sys/misc/tokenize.h"

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	MultipleCommunicationModelFactory::
		~MultipleCommunicationModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string 
		MultipleCommunicationModelFactory::
		name(void) 
		const throw()
	{
		return "multiple";
	}

	// ----------------------------------------------------------------------
	std::string 	
		MultipleCommunicationModelFactory::
		description(void) 
		const throw()
	{
		return "Multiple Communication Model. "
			"Parameters: "
			"$comm_models to specify a comma seperated list of comm models. "
			"$comm_default_model to specify the defaul model to use, if the others in front of "
			"it can not supply a communication relation. "
			"Example: comm_models=manual,someother_manual comm_default_model=disk_graph";
	}

	// ----------------------------------------------------------------------
	CommunicationModel* 
		MultipleCommunicationModelFactory::
		create( const SimulationController& sc) 
		const throw()
	{
		const SimulationEnvironment& se = sc.environment();
		const CommunicationModelKeeper& cmk = sc.communication_model_keeper();
		MultipleCommunicationModel* m = new MultipleCommunicationModel();

		std::string model_names = se.required_string_param("comm_models");
		std::string default_name = se.optional_string_param("comm_default_model", "");

		//Add all comm models to the multiple comm model
        StrTok tok(model_names, ", ");
        for(StrTok::iterator it = tok.begin(), end = tok.end(); it!=end; ++it)
		{
			ConstCommunicationModelFactoryHandle factory = cmk.find(*it);
            m->add_comm_model( *(factory.get()->create(sc)) );
		}

		//Set the default comm model
		if( default_name != "" )
		{
			ConstCommunicationModelFactoryHandle factory = cmk.find(default_name);
            m->set_default_comm_model( *(factory.get()->create(sc)) );
		}

		return m;
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/multiple_communication_model_factory.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: multiple_communication_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
