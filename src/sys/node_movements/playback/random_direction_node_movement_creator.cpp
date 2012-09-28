/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include <cfloat>
#include <cmath>

#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"

#include "sys/node_movements/playback/random_direction_node_movement_creator.h"

#ifdef RANDOMDIRECTION_STARTNOW
#undef RANDOMDIRECTION_STARTNOW
#endif
#define RANDOMDIRECTION_STARTNOW 0.0
#ifdef PI
#undef PI
#endif
#define PI 3.14159265

using namespace std;

namespace shawn{

   RandomDirectionNodeMovementCreator::RandomDirectionNodeMovementCreator(SimulationController& sc) : 
	sc_(sc),
	reset_(false)
   {

	  width_ = sc.environment_w().required_double_param("width");
      height_ = sc.environment_w().required_double_param("height");

      urv_speed_.set_lower_bound(sc.environment_w().optional_double_param("random_direction_v_min", 0.001));
      urv_speed_.set_upper_bound(sc.environment_w().optional_double_param("random_direction_v_max", 1.0));
      urv_speed_.set_lower_bound_inclusive(true);
      urv_speed_.set_upper_bound_inclusive(true);
      urv_speed_.init();

      urv_direction_.set_lower_bound(0.0);
      urv_direction_.set_upper_bound(2*PI);
      urv_direction_.set_lower_bound_inclusive(true);
      urv_direction_.set_upper_bound_inclusive(false);
      urv_direction_.init();

	  urv_t_move_.set_lower_bound(sc.environment_w().optional_double_param("random_direction_t_move_min", DBL_MAX));
	  urv_t_move_.set_upper_bound(sc.environment_w().optional_double_param("random_direction_t_move_max", DBL_MAX));
	  urv_t_move_.set_lower_bound_inclusive(true);
	  urv_t_move_.set_upper_bound_inclusive(true);
	  urv_t_move_.init();

	  urv_t_stop_.set_lower_bound(sc.environment_w().optional_double_param("random_direction_t_stop_min", 0));
	  urv_t_stop_.set_upper_bound(sc.environment_w().optional_double_param("random_direction_t_stop_max", 0));
	  urv_t_stop_.set_lower_bound_inclusive(true);
	  urv_t_stop_.set_upper_bound_inclusive(true);
	  urv_t_stop_.init();
   }

   RandomDirectionNodeMovementCreator::~RandomDirectionNodeMovementCreator(void)
   {
	   if(!reset_) reset();
   }


   MovementInfo* RandomDirectionNodeMovementCreator::next_movement()
      throw( std::runtime_error )
   {
      if (static_cast<int>(next_movement_times_.size()) < sc_.world().node_count())
      {
         // Generate new movements for remaining nodes
         for (World::node_iterator it=sc_.world_w().begin_nodes_w(); it!=sc_.world_w().end_nodes_w(); ++it)
         {
            NodeMovement &nm = it->movement_w();
			if (nm.name() == "NoMovement")
			{
				return generate_new_movement(*it, RANDOMDIRECTION_STARTNOW);
			}
            LinearMovement *lm = dynamic_cast<LinearMovement*>(&nm);
            if ( ( lm != NULL ) && ( lm->position() == lm->destination() ) )
            {
               return generate_new_movement(*it, RANDOMDIRECTION_STARTNOW);
            }
         }
      }
      // Otherwise get the next scheduled movement
      if (next_movement_times_.size() > 0)
      {
         double start_time = next_movement_times_.begin()->first;
         const Node *n = next_movement_times_.begin()->second;
         assert (n != NULL);
         next_movement_times_.erase(next_movement_times_.begin());
         return generate_new_movement(*n, start_time);
      }

      return NULL;

   }

   MovementInfo* RandomDirectionNodeMovementCreator::generate_new_movement(const Node &node, double start_time)
   {
      // Generate a new movement
      MovementInfo *mi = new MovementInfo();
	  mi->set_node(const_cast<shawn::Node*>(&node));
      if (start_time <= 0.0) 
      {
         mi->set_urgency(MovementInfo::Immediately);
      } 
	  else 
	  {
         mi->set_urgency(MovementInfo::Delayed);
         mi->set_dispatch_time(start_time);
      }

	  shawn::Vec destination = get_destination(node);

	  LinearMovement *lm = new LinearMovement();

	  // Get movement vector
	  double direction = 0.0;
	  double velocity = 0.0;
	  double t_move = 0.0;
	  double t_stop = 0.0;
	  double vector_length = 0.0;
	  std::map<const shawn::Node*,RDNMCInfo*>::iterator it = rdnmc_infos_.find(&node);
	  if (it == rdnmc_infos_.end() || EQDOUBLE(it->second->t_move(),0.0))
	  {
		  direction = urv_direction_;
		  velocity = urv_speed_;
		  t_move = urv_t_move_;
		  t_stop = urv_t_stop_;
		  vector_length = (velocity * t_move);
		  if(it != rdnmc_infos_.end())
		  {
			  delete it->second;
			  rdnmc_infos_.erase(it);
		  }
	  }
	  else
	  {
		  direction = it->second->direction();
		  velocity = it->second->velocity();
		  t_move = it->second->t_move();
		  t_stop = it->second->t_stop();
		  vector_length = (velocity * t_move);
		  delete it->second;
		  rdnmc_infos_.erase(it);
	  }
      Vec vector(cos(direction) * vector_length, sin(direction) * vector_length, 0.0);

	  if (is_inside(destination + vector))
	  {
		  next_movement_times_.insert(std::make_pair(start_time + t_move + t_stop,&node));
		  lm->set_parameters(velocity,(destination + vector),sc_.world_w());
		  mi->set_node_movement(lm);
	  }
	  else
	  {
		  double intersection = get_intersection(node,vector);
		  // time_until_hit the border
		  double time_until_hit = ((vector * intersection).euclidean_norm()/velocity);
		  next_movement_times_.insert(std::make_pair(start_time + time_until_hit,&node));
		  shawn::Vec w = (destination + vector * intersection);
		  RDNMCInfo *rdnmci = new RDNMCInfo(new_direction(direction,w),
					                        velocity,
						                    (t_move - time_until_hit),
					                        t_stop);
		  rdnmc_infos_.insert(std::make_pair(&node,rdnmci));
		  lm->set_parameters(velocity,w,sc_.world_w());
		  mi->set_node_movement(lm);
	  }

      return mi;
   }

   void RandomDirectionNodeMovementCreator::reset()
   {
      next_movement_times_.clear();
	  for(std::map<const Node*, RDNMCInfo*>::iterator it = rdnmc_infos_.begin(); it != rdnmc_infos_.end(); ++it)
	  {
		  delete it->second;
	  }
	  rdnmc_infos_.clear();
	  reset_ = true;
   }

   double 
	   RandomDirectionNodeMovementCreator::
	   get_intersection(const shawn::Node& node, const shawn::Vec& vector) 
	   const
   {
		shawn::Vec destination = get_destination(node);
		
		double intersection = DBL_MAX;
		// Get x intersection
		if (vector.x() < 0.0)
		{
			double t = (0.0 - destination.x()) / vector.x();
			if (t < intersection) intersection = t;
		} 
		else if (vector.x() > 0.0)
		{
			double t = (width_ - destination.x()) / vector.x();
			if (t < intersection) intersection = t;
		}
		// Get y intersection
		if (vector.y() < 0.0)
		{
			double t = (0.0 - destination.y()) / vector.y();
			if (t < intersection) intersection = t;
		} 
		else if (vector.y() > 0.0)
		{
			double t = (height_ - destination.y()) / vector.y();
			if (t < intersection) intersection = t;
		}
		return intersection;
   }

   double 
	   RandomDirectionNodeMovementCreator::
	   new_direction(double alpha, const shawn::Vec& vec) 
	   const
   {
	   Borders border = get_border(vec);
       
	   if (border == right && alpha >= 0.0 && alpha < PI/2){
			return (PI - alpha);
	   }
	   if (border == top && alpha >= 0.0 && alpha < PI/2){
		   return EQDOUBLE(alpha,0.0) ? 0.0 : (2*PI - alpha);
	   }
	   if (border == top && alpha >= PI/2 && alpha < PI){
			return (2*PI - alpha);
	   }
	   if (border == left && alpha >= PI/2 && alpha < PI){
			return (PI - alpha);
	   }
	   if (border == left && alpha >= PI && alpha < 3*PI/2){
		   return EQDOUBLE(alpha,PI) ? 0.0 : (3*PI - alpha);
	   }
	   if (border == bottom && alpha >= PI && alpha < 3*PI/2){
			return (2*PI - alpha);
	   }
	   if (border == bottom && alpha >= 3*PI/2 && alpha < 2*PI){
		   return (2*PI - alpha);
	   }
	   if (border == right && alpha >= 3*PI/2 && alpha < 2*PI){
			return (3*PI - alpha);
	   }
	   
	   if (border == lowerleftcorner && alpha >= PI && alpha <= 3*PI/2){
			return (alpha - PI);
	   }
	   if (border == lowerleftcorner && alpha > PI/2 && alpha < PI){
			return (PI - alpha);
	   }
	   if (border == lowerleftcorner && alpha > 3*PI/2 && alpha < 2*PI){
			return (2*PI - alpha);
	   }

	   if (border == lowerrightcorner && alpha >= 3*PI/2 && alpha < 2*PI){
			return (alpha - PI);
	   }
	   if (border == lowerrightcorner && alpha > PI && alpha < 3*PI/2){
			return (2*PI - alpha);
	   }
	   if (border == lowerrightcorner && alpha >= 0.0 && alpha < PI/2){
			return (PI - alpha);
	   }

	   if (border == upperrightcorner && alpha >= 0.0 && alpha <= PI/2){
			return (PI + alpha);
	   }
	   if (border == upperrightcorner && alpha > PI/2 && alpha < PI){
			return (2*PI - alpha);
	   }
	   if (border == upperrightcorner && alpha > 3*PI/2 && alpha < 2*PI){
			return (3*PI - alpha);
	   }

	   if (border == upperleftcorner && alpha >= PI/2 && alpha <= PI){
		   return EQDOUBLE(alpha,PI) ? 0.0 : (PI + alpha);
	   }
	   if (border == upperleftcorner && alpha > PI && alpha < 3*PI/2){
			return (3*PI - alpha);
	   }
	   if (border == upperleftcorner && alpha > 0.0 && alpha < PI/2){
			return (2*PI - alpha);
	   }
#ifdef DEBUG
	   std::cerr << "ERROR in RandomDirectionNodeMovementCreator: Impossible direction set!" << std::endl;
	   std::cerr << "vec: " << vec << std::endl;
	   std::cerr << "border: " << border << " alpha: " << alpha << std::endl;
#endif
	   return 2*PI;
   }

   bool 
	   RandomDirectionNodeMovementCreator::
	   is_inside(const shawn::Vec &vector) 
	   const
   {
		if(vector.x() >= width_) return false;
		if(vector.x() <= 0) return false;
		if(vector.y() >= height_) return false;
		if(vector.y() <= 0) return false;
		return true;
   }

   const shawn::Vec 
	   RandomDirectionNodeMovementCreator::
	   get_destination(const shawn::Node& node)
	   const
   {
		shawn::Vec destination = node.real_position();
		if(node.movement().name() == "LinearMovement"){
			try{
				const shawn::LinearMovement& lm = dynamic_cast<const shawn::LinearMovement&>(node.movement());
				destination = lm.destination();
			}catch(bad_cast e){
				std::cerr << "Error in RandomNodeMovementCreator::get_intersection(): " << e.what() << std::endl;
			}
		}
		return destination;
   }

   shawn::RandomDirectionNodeMovementCreator::Borders 
	   RandomDirectionNodeMovementCreator::
	   get_border(const shawn::Vec& vec)
	   const
   {
		if(EQDOUBLE(vec.x(),0.0) && !EQDOUBLE(vec.y(),0.0) && !EQDOUBLE(vec.y(),height_)) return left;
		if(EQDOUBLE(vec.y(),0.0) && !EQDOUBLE(vec.x(),0.0) && !EQDOUBLE(vec.x(),width_)) return bottom;
		if(EQDOUBLE(vec.x(),width_) && !EQDOUBLE(vec.y(),0.0) && !EQDOUBLE(vec.y(),height_)) return right;
		if(EQDOUBLE(vec.y(),height_) && !EQDOUBLE(vec.x(),0.0) && !EQDOUBLE(vec.x(),width_)) return top;

		if(EQDOUBLE(vec.x(),0.0) && EQDOUBLE(vec.y(),0.0)) return lowerleftcorner;
		if(EQDOUBLE(vec.x(),width_) && EQDOUBLE(vec.y(),0.0)) return lowerrightcorner;
		if(EQDOUBLE(vec.x(),width_) && EQDOUBLE(vec.y(),height_)) return upperrightcorner;
		if(EQDOUBLE(vec.x(),0.0) && EQDOUBLE(vec.y(),height_)) return upperleftcorner;

		return noborder;
   }

   //---RDNMCInfo---

   RandomDirectionNodeMovementCreator::
	   RDNMCInfo::
	   RDNMCInfo(double d, double v, double m, double s) : 
		direction_(d),
		velocity_(v),
	    t_move_(m),
		t_stop_(s)
		{
			assert(direction_ >= 0.0);
			assert(direction_ < 2*PI);
			assert(velocity_ >= 0.0);
			assert(t_move_ >= 0.0);
			assert(t_stop_ >= 0.0);
		}

	RandomDirectionNodeMovementCreator::
		RDNMCInfo::
		~RDNMCInfo()
	{}

	double 
		RandomDirectionNodeMovementCreator::
		RDNMCInfo::
		direction()
		const
	{
			return direction_;
	}

	double 
		RandomDirectionNodeMovementCreator::
		RDNMCInfo::
		velocity()
		const
	{
		return velocity_;
	}

	double 
		RandomDirectionNodeMovementCreator::
		RDNMCInfo::
		t_move()
		const 
	{
		return t_move_;
	}

	double 
		RandomDirectionNodeMovementCreator::
		RDNMCInfo::
		t_stop() 
		const
	{
		return t_stop_;
	}	
}
