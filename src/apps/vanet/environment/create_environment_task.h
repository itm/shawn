/**
 * @file create_environment_task.h
 * Defines the task vanet::CreateEnvironmentTask.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/environment/create_environment_task.h $
 * $Id: create_environment_task.h 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_ENVIRONMENT_CREATE_ENVIRONMENT_TASK_H
#define __SHAWN_APPS_VANET_ENVIRONMENT_CREATE_ENVIRONMENT_TASK_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/simulation/simulation_task.h"

namespace vanet
{

class CreateEnvironmentTask : public shawn::SimulationTask
{
   class ReadingsFileReader;

public:
   CreateEnvironmentTask();

   virtual
   ~CreateEnvironmentTask();

   virtual std::string
   name( void ) const throw();
   virtual std::string
   description( void ) const throw();

   virtual void
   run( shawn::SimulationController& sc ) throw( std::runtime_error );
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_ENVIRONMENT_CREATE_ENVIRONMENT_TASK_H */
