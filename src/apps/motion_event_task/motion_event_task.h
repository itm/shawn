/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_H
#define __SHAWN_APPS_MOTION_EVENT_TASK_MOTION_EVENT_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_MOTIONEVENTTASK

#include "sys/simulation/simulation_task.h"
#include "sys/misc/random/random_variable_keeper.h"
#include "sys/misc/random/uniform_random_variable.h"
#include "sys/misc/random/normal_random_variable.h"
#include "sys/node.h"
#include <list>

namespace shawn 
{
	class Vec;
	class SimulationController;
}

namespace motion_event
{
	
	struct point 
	{
		float x;
		float y;
	};

	struct line
	{
		point p;
		point a;
	};
	

	/** \brief Simulates moving objects in the world
	*
	* Objects begin to move at the specified time and move with a certain velocity 
	* according to the given positions without stop. The positions are specified 
	* through vectors (x-, y- and z-coordinate). 
	* 
	* Each node in the world has the same detection range (approximated by a square). 
	* When an object crosses the detection range of one node, the task adds a 
	* "MotionEventTag" to this node witch contains the time of the crossing multiplied
	* with an error.
	* 
	* The given standard deviation (standard_deviation) and maximum error (max_time_error) influence this error.
	* 
	* There are two types in this task with the following functions:
	* type = object: 
	* Adds all MotionEventTags to the world caused by this object. 
	* Parameters like start_time and velocity must be set.
	* 
	* type = errorDetections: 
	* Adds MotionEventTags to the world caused by error detections (false positives).
	* The parameter 'errorDetectionsPerTimeRange' specifies the absolute number of these error detections, 
	* the time range is set to one day per default. 
	* The parameter 'TimeRange' must be greater than or equal to the number of simulations rounds or
	* you have to implement the other case! If a simulation has less rounds than this
	* value, a uniform distributed random variable decides how much error detections are at least
	* during the simulation.
	*/
	class MotionEventTask
		: public shawn::SimulationTask
	{
	public:
		MotionEventTask();
		virtual ~MotionEventTask();
   
		/** The name is motion_event_task */
		virtual std::string name( void ) const throw();
   
      /** A generator for motion events in the world
       */
      virtual std::string description( void ) const throw();

      // Main method
      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

	private:
		// Private functions for the computation of the times of detection
		void single_line(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos, std::string sensor_model);
	
		void single_line_disc_range(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos);
	
		bool between(const shawn::Vec, const shawn::Vec, const shawn::Vec);
	
		bool in_range_of_node(shawn::Node*);
	
		void set_in_range_of_node(shawn::Node*, bool);

		point v_minus (point a, point b);
		
		point v_plus (point a, point b);

		float determinante (point a, point b);

		point sv_mult (float s, point v);

		point schnitt (line g1, line g2);

		bool kollinear (point a, point b);

		bool parallel (line g1, line g2);

		bool checkSensor(line weg,line sensorr,line rp,line pq,line qsensor,point sensor,point r,point p,point q);


		/** \brief Object id
		*/
		int object_id_;

		/** \brief Velocity of the object
		*
		* The velcoity of one person does not change in one simulation. 1 correspond to 1 meter/simulation round.
		*/
		double velocity_;

		/** \brief Detection range of each nodes in the world.
		*
		* The range of the nodes is approximated by a square lieing on edge in the world. 
		* The parameter is shortest distance to the four edges.  
		*/
		double detection_range_;

		/** \brief Start time of the object.
		*/
		double start_time_;

		/** \brief Standard deviation for the error added to (or subtracted from) the time of detection
		*
		* The default value is 0. So a nodes detects an object when this is at the nearest point of 
		* the node. If the standard deviation is greater than 0, the absolute value of a 
		* normal distributed random error is added 
		* to this time of detection.
		*/
		double standard_deviation_;

		/** \brief Maximum of the error added to the time of detection. If the absolute random value is bigger
		* than this variable, a new value is generated until it is smaller.
		*/
		double max_time_error_;

		/** \brief Rate of not detected events. (Computed by a uniform distributed random variable.)  
		*/
		double missing_detection_rate_;
	   
		/** \brief Temporary saving the direction change of an object. 
		*/
		std::list<std::pair<shawn::Node*,bool> > in_range_of_nodes_; 

		shawn::NormalRandomVariable* nrv_;
		
		shawn::UniformRandomVariable* urv_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2007/01/31 17:24:17 $
 *-----------------------------------------------------------------------
 * $Log: motion_event_task.h,v $
 * Revision 1.2  2007/01/31 17:24:17  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2007/01/29 16:37:14  krueger
 * *** empty log message ***
 *
 * Revision 1.1  2005/03/05 06:43:10  tbaum
 * added motion_event_task
 *
*-----------------------------------------------------------------------*/
