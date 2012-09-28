/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/communication/wiseml_comm_model.h"
#ifdef ENABLE_WISEML

#include "apps/wiseml/communication/wiseml_comm_model_factory.h"
#include "apps/wiseml/communication/wiseml_comm_model.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

using namespace std;

namespace wiseml
{

	// ----------------------------------------------------------------------
	WiseMlCommModelFactory::
		~WiseMlCommModelFactory()
	{}

	// ----------------------------------------------------------------------
	std::string
		WiseMlCommModelFactory::
		name(void)
		const throw()
	{
		return "wiseml";
	}

	// ----------------------------------------------------------------------
	std::string
		WiseMlCommModelFactory::
		description(void)
		const throw()
	{
		return "WiseML Communication Model";
	}

	// ----------------------------------------------------------------------
	CommunicationModel*
		WiseMlCommModelFactory::
		create( const SimulationController& sc)
		const throw()
	{
		return new WiseMlCommModel;
    }


}

#endif
