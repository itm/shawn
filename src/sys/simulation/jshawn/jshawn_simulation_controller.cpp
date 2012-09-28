/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "shawn_config.h"
#include "sys/simulation/jshawn/jshawn_simulation_controller.h"
#include "sys/simulation/simulation_task.h"

using namespace std;

namespace shawn
{
	// ----------------------------------------------------------------------
	void
		JShawnSimulationController::
		execute_task(SimulationTaskHandle st) 
		throw( std::runtime_error )
	{
		string jshawn_run_command = "jshawn_run_command";

		start_module(cout, jshawn_run_command );
		cout << "start:" << st->name() << endl;
		end_module(cout, jshawn_run_command);

		st->run(*this);

		for(SimulationTask::ResultSetIt it = st->begin_results(), end = st->end_results(); it!=end; ++it)
		{
			start_module(cout, (*it)->name());
			(*it)->send_to(cout);
			end_module(cout, (*it)->name());
		}

		start_module(cout, jshawn_run_command );
		cout << "end:" << st->name() << endl;
		end_module(cout, jshawn_run_command);
	}

	// ----------------------------------------------------------------------
	void
		JShawnSimulationController::
		start_module(ostream& o, std::string name)
	{
		o << endl << "jshawn_internal:start_module:" << name << endl;
	}

	// ----------------------------------------------------------------------
	void 
		JShawnSimulationController::
		end_module(ostream& o, std::string name)
	{
		o << endl << "jshawn_internal:end_module:" << name << endl;
	}


}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/simulation/jshawn/jshawn_simulation_controller.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: jshawn_simulation_controller.cpp,v $
 *-----------------------------------------------------------------------*/
