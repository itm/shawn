/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_SIMULATION_SIMULATION_SHAWN_SIMULATION_CONTROLLER_H
#define __SHAWN_SYS_SIMULATION_SIMULATION_SHAWN_SIMULATION_CONTROLLER_H

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"

namespace shawn
{
	class SimulationTaskKeeper;

	class ShawnSimulationController : public SimulationController
	{	
	public:
		ShawnSimulationController();
		~ShawnSimulationController();

		static const std::string WHITE_SPACE;

		struct CommandLineInfo
		{
			std::string command_;
			bool has_command_;
			SimulationEnvironment::ParameterSetHandle parms_;
		};

		void execute_command_line( const std::string& ) throw( std::runtime_error );

		virtual void execute_task(SimulationTaskHandle) throw( std::runtime_error );

	private:
		void 
		check_line_token( const std::string& t, 
                        ShawnSimulationController::CommandLineInfo& li )
        const throw( std::runtime_error );

		bool
		extract_line_token( const std::string& b,
						std::string::size_type& start,
						ShawnSimulationController::CommandLineInfo& li )
        const throw( std::runtime_error );

		inline ShawnSimulationController( const ShawnSimulationController& ) { abort(); }
      std::string prefix_line_;
	};
}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/shawn_simulation_controller.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: shawn_simulation_controller.h,v $
 *-----------------------------------------------------------------------*/
