/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_WISEML_COMMUNICATION_WISEML_COMM_MODEL_FACTORY_H
#define __SHAWN_APPS_WISEML_COMMUNICATION_WISEML_COMM_MODEL_FACTORY_H

#include "apps/wiseml/communication/wiseml_comm_model.h"
#ifdef ENABLE_WISEML

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/comm_models/communication_model_factory.h"

#include <string>
#include <vector>

using namespace shawn;

//class shawn::CommunicationModel;
//class shawn::SimulationController;

namespace wiseml
{
	//----------------------------------------------------------------------------
	/**
	  * A factory to create a WiseML communication model.
	  */
	class WiseMlCommModelFactory
      : public shawn::CommunicationModelFactory
	{
	public:
		virtual ~WiseMlCommModelFactory();
		virtual CommunicationModel* create( const SimulationController& ) const throw();
		virtual std::string name(void)  const throw();
		virtual std::string description(void) const throw();
	};

}

#endif
#endif
