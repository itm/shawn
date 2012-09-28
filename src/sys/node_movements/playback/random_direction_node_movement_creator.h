/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_RANDOM_DIRECTION_NODE_MOVEMENT_CREATOR
#define __SHAWN_SYS_NODE_MOVEMENTS_PLAYBACK_RANDOM_DIRECTION_NODE_MOVEMENT_CREATOR

#include "sys/event_scheduler.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/node_movement.h"
//#include "sys/taggings/group_tag.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/node.h"
//#include <string>
#include <map>


namespace shawn
{
	class RandomDirectionNodeMovementCreator:
		public NodeMovementCreator
	{
		public:
      ///@name construction / destruction
      ///@{
      ///
      RandomDirectionNodeMovementCreator(SimulationController& sc);

	  virtual ~RandomDirectionNodeMovementCreator();
		      ///@}


      ///@name methods inherited from NodeMovementCreator
      ///@{
      /** This method scans all Nodes regarding the next Movement from the xml tags belonging 
		 *  to distinct nodes.
		 *
       */
		virtual shawn::MovementInfo *next_movement(void) throw( std::runtime_error );
	
		/** This method causes the NodeMovementCreator to start movements
       *  from the beginning.
	   *
       */
		virtual void reset();

	private:
	
      MovementInfo* generate_new_movement(const Node &node, double startTime);

	  double get_intersection(const shawn::Node&, const shawn::Vec&) const;

	  bool is_inside(const shawn::Vec&) const;

	  double new_direction(double, const shawn::Vec&) const;

	  const shawn::Vec get_destination(const shawn::Node&) const;

	  enum Borders{noborder, left, bottom, right, top, lowerleftcorner, lowerrightcorner, upperrightcorner, upperleftcorner};

	  Borders get_border(const shawn::Vec&) const;

      SimulationController &sc_;
      
      // Map to save times, when nodes will fall from the border of the world
      std::multimap<double, const Node*> next_movement_times_;

	  struct RDNMCInfo{
		RDNMCInfo(double d, double v, double m, double s);
		virtual ~RDNMCInfo();
		double direction() const;
		double velocity() const;
	    double t_move() const;
		double t_stop() const;
	  private:
		double direction_;
		double velocity_;
        double t_move_;
		double t_stop_;
	  };
	  //
	  std::map<const Node*, RDNMCInfo*> rdnmc_infos_;

      // Random Variable for getting speeds
      shawn::UniformRandomVariable urv_speed_;
      // Random Variable for getting directions
      shawn::UniformRandomVariable urv_direction_;
	  //
	  shawn::UniformRandomVariable urv_t_move_;
	  //
	  shawn::UniformRandomVariable urv_t_stop_;
      
      // The world does not know its maximum dimension, so get it from the SimulationController
      double width_;
      double height_;
	  //
	  bool reset_;
   };
}
#endif
