/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/simulation/shawn_simulation_controller.h"
#include "sys/world.h"
#include "sys/simulation/simulation_task.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/processors/processor_keeper.h"
#include "sys/node_distance_estimate.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/edge_models/edge_model_keeper.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/taggings/tag_factory_keeper.h"
using namespace std;

namespace shawn
{
	const std::string ShawnSimulationController::WHITE_SPACE = " \t\n\r";

	ShawnSimulationController::
   ShawnSimulationController()
		: SimulationController(),
        prefix_line_("")
	{}
   // ----------------------------------------------------------------------
	ShawnSimulationController::
   ~ShawnSimulationController()
	{}
   // ----------------------------------------------------------------------
	void 
	ShawnSimulationController::
	check_line_token( const std::string& t, 
                     ShawnSimulationController::CommandLineInfo& li )
    const throw( std::runtime_error )
	{
		std::string::size_type eqpos = t.find('=');

		if( eqpos==std::string::npos )
		{
			if( li.has_command_ )
               throw std::runtime_error(std::string("Two commands in command line: '")+
                                        li.command_ +
                                        std::string("' and '") +
                                        t +
                                        std::string("'.") );
            else
               {
                  li.has_command_ = true;
                  li.command_ = t;
               }
		}
		else
        {
            assert( li.parms_ != NULL );
            li.parms_->add( t.substr(0,eqpos), t.substr(eqpos+1) );
        }
	}
	// ----------------------------------------------------------------------
   bool
	ShawnSimulationController::
	extract_line_token( const std::string& b,
                       std::string::size_type& start,
                       ShawnSimulationController::CommandLineInfo& li )
		const throw( std::runtime_error )
	{
      std::string::size_type pos1, pos2;
      std::string::size_type pos_eq, pos_beg_str, pos_end_str;

      // Find a string that is represented by
      //    varname="string that ought to be stored as a parameter".
      // It is essential that the string starts and ends with a '"', and that
      // the first '"' is exactly written after the '='. If so, all spaces or
      // other separators given by ShawnSimulationController::WHITE_SPACE are
      // not considered.
      pos_eq = b.find_first_of( "=", start );
      pos_beg_str = b.find_first_of( "\"", start );
      pos_end_str = std::string::npos;
      if ( pos_beg_str != std::string::npos )
         pos_end_str = b.find_first_of( "\"", pos_beg_str + 1 );
      if ( pos_eq != std::string::npos && pos_end_str != std::string::npos &&
            pos_beg_str == pos_eq + 1 )
      {
         // If a string has been found (e.g.: param1="str 01"  param02=5), the
         // cursor (pos2) has to be set to the next parameter (in the above
         // example, ->p<-aram02 ).
         pos2 = b.find_first_not_of(ShawnSimulationController::WHITE_SPACE,pos_end_str + 1);
         // Add the string by suppressing the leading and ending '"'
         li.parms_->add( b.substr( start, pos_eq - start ),
                         b.substr( pos_beg_str + 1, pos_end_str - pos_beg_str - 1 ) );
      }
      else
      {
         // If a string has not been found, look for the standard cases
         pos1 = b.find_first_not_of(ShawnSimulationController::WHITE_SPACE,start);
         if( pos1 == std::string::npos ) return false;
         pos2 = b.find_first_of(ShawnSimulationController::WHITE_SPACE,pos1);
         
         if( pos2 == std::string::npos )
            check_line_token( b.substr(pos1), li );
         else
            check_line_token( b.substr(pos1,pos2-pos1), li );
      }      

      start=pos2;
      return pos2 != std::string::npos;
	}
	// ----------------------------------------------------------------------
	void
	ShawnSimulationController::
	execute_command_line( const std::string& bpost )
		throw( std::runtime_error )
	{
      if( bpost.empty() ) return;

      if( bpost[bpost.size()-1]=='\\' )
         { 
            prefix_line_ += bpost.substr(0,bpost.size()-1);
            return;
         }

      std::string b = prefix_line_ + bpost;
      prefix_line_ = "";

      CommandLineInfo li;
      li.has_command_=false;
      li.parms_=new SimulationEnvironment::ParameterSet;

      std::string::size_type hashm = b.find('#');

      std::string line;
      if( hashm == std::string::npos )
         line=b;
      else
         line=b.substr(0,hashm);

      std::string::size_type curpos = 0;
      while( extract_line_token(line,curpos,li) );

      if( li.has_command_ )
         {
            environment_w().push_parameters( li.parms_ );

            SimulationTaskHandle sth = simulation_task_keeper_w().find_w( li.command_ );
            if( sth != NULL )
               {
                  execute_task(sth);
                  environment_w().pop_parameters();
               }
            else
               {
                  environment_w().pop_parameters();
                  throw std::runtime_error(std::string("Unknown task '")+
                                           li.command_ +
                                           std::string("'.") );
               }
         }
      else
         environment_w().add_parameters_low_prio( li.parms_ );
	}
	// ----------------------------------------------------------------------
	void 
   ShawnSimulationController::
   execute_task(SimulationTaskHandle th) 
		throw( std::runtime_error )
	{
		cout << "Simulation: Running task '" << th->name() << "'" << endl;
		th->run(*this);
		cout << "Simulation: Task done '" << th->name() << "'" << endl;
	}

}
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/simulation/shawn_simulation_controller.cpp,v $
 * Version $Revision: 114 $
 * Date    $Date: 2007-12-12 14:49:16 +0100 (Wed, 12 Dec 2007) $
 *-----------------------------------------------------------------------
 * $Log: shawn_simulation_controller.cpp,v $
 *-----------------------------------------------------------------------*/
