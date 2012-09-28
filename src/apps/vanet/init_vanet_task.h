/**
 * @file init_vanet_task.h
 * Defines the class InitVanetTask that initialises pre and post step tasks
 * of the VANET app.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/init_vanet_task.h $
 * $Id: init_vanet_task.h 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef VANET_INIT_TASK_H_
#define VANET_INIT_TASK_H_
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/simulation/simulation_task.h"

namespace vanet {

class InitVanetTask: public shawn::SimulationTask {
public:
	InitVanetTask();
	virtual ~InitVanetTask();

	   virtual std::string
	   name( void ) const throw();
	   virtual std::string
	   description( void ) const throw();

	   virtual void
	   run( shawn::SimulationController& sc ) throw( std::runtime_error );

};

}

#endif /* ENABLE_VANET */
#endif /* VANET_INIT_TASK_H_ */

