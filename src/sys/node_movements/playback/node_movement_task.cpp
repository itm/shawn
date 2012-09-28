/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movements/playback/node_movement_task.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/node_movements/playback/ns2_node_movement_creator.h"
#include "sys/node_movements/playback/tag_node_movement_creator.h"
#include "sys/node_movements/playback/random_direction_node_movement_creator.h"
#include "sys/event_scheduler.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "apps/traci_client/traci_node_movement_creator.h"
#include "apps/sumo/sumo_node_movement.h"
#include <string>

namespace shawn
{

    SimulationTaskNodeMovement::
        SimulationTaskNodeMovement()
	 {}
    // ----------------------------------------------------------------------
    SimulationTaskNodeMovement::
        ~SimulationTaskNodeMovement()
    {}
    // ----------------------------------------------------------------------
    void
        SimulationTaskNodeMovement::
        run( SimulationController& sc )
        throw(std::runtime_error )
	 {
		 
		if (&(sc.world_w().movement_controller_w()) != NULL) 
		{
	   if (sc.environment_w().optional_string_param("mode", "")=="ns2_parsefile")
		{
		    NS2NodeMovementCreator * ns2nmc = new NS2NodeMovementCreator(sc);
		    sc.world_w().movement_controller_w().set_node_movement_creator(ns2nmc);
		     sc.world_w().movement_controller_w().start();
		}
		else if(sc.environment_w().optional_string_param("mode","")=="tag_movement")
		{
			
			TagNodeMovementCreator * tagnmc =new TagNodeMovementCreator(sc);
			 sc.world_w().movement_controller_w().set_node_movement_creator(tagnmc);
		     sc.world_w().movement_controller_w().start();
		}
      else if(sc.environment_w().optional_string_param("mode","")=="random_direction")
      {
         RandomDirectionNodeMovementCreator * rdnmc =new RandomDirectionNodeMovementCreator(sc);
         sc.world_w().movement_controller_w().set_node_movement_creator(rdnmc);
         sc.world_w().movement_controller_w().start();
      }
	  else if(sc.environment_w().optional_string_param("mode","")=="TraCI")
	  {
#ifdef ENABLE_TRACICLIENT
		  //shawn::KeeperManagedHandle kmh = sc.simulation_task_keeper_w().find_managed_w("TraCI");
		  //traci::TraCINodeMovementCreator * tnmc = dynamic_cast<traci::TraCINodeMovementCreator*>( kmh.get() );
		  sc.world_w().movement_controller_w().set_node_movement_creator( /*tnmc*/ traci::TraCIClient::instance() );
		  traci::TraCIClient::instance()->run(sc);
		  sc.world_w().movement_controller_w().start();
#else
		  std::cerr << "TraCI node movement not built!" << std::endl;
		  abort();
#endif
	  }
          else if(sc.environment_w().optional_string_param("mode","")=="sumo")
          {
#ifdef ENABLE_SUMO
                  std::cout << "Start node movement from SUMO dump file" << std::endl;
                  sumo::SumoNodeMovementCreator * sumonmc = new sumo::SumoNodeMovementCreator(sc);
                  sc.world_w().movement_controller_w().set_node_movement_creator(sumonmc);
                  sc.world_w().movement_controller_w().start();
#else
                  std::cerr << "SUMO app not built!" << std::endl;
#endif
          }
		else
		{
        WARN(sc.logger(), "No NodeMovementCreator Set");
      }
	}
   }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskNodeMovement::
        name( void )
        const throw()
    {
        return std::string("node_movement");
    }
    // ----------------------------------------------------------------------
    std::string
        SimulationTaskNodeMovement::
        description( void )
        const throw()
    {
        return std::string("Enables Nodes to be moved");
    }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/node_movements/playback/node_movement_task.cpp,v $
 * Version $Revision: 493 $
 * Date    $Date: 2011-01-14 17:02:19 +0100 (Fri, 14 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: node_movement_task.cpp,v $
 *-----------------------------------------------------------------------*/
