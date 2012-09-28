/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_COMM_MODELS_RIM_COMM_MODEL_FACTORY_H
#define __SHAWN_SYS_COMM_MODELS_RIM_COMM_MODEL_FACTORY_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"
#include "sys/comm_models/communication_model_factory.h"

#include <string>
#include <vector>

namespace shawn
{
   class CommunicationModel;
   class SimulationController;

	//----------------------------------------------------------------------------
	/**
	  *
	  */
	class RimCommModelFactory
		: public CommunicationModelFactory
	{
	public:
		virtual ~RimCommModelFactory();
		virtual CommunicationModel* create( const SimulationController& ) const throw();
		virtual std::string name(void)  const throw();
		virtual std::string description(void) const throw();
	};

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/comm_models/rim_comm_model_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: rim_comm_model_factory.h,v $
 *-----------------------------------------------------------------------*/
