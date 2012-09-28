/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_MOTIONEVENTTASK

#include "apps/motion_event_task/motion_event_task.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/event_scheduler.h"
#include "sys/vec.h"
#include "apps/topology/polygon/polygon.h"
#include "apps/topology/polygon/jarvis_march.h"

#include "sys/taggings/basic_tags.h"
#include "sys/taggings/map_tags.h"
#include "sys/misc/tokenize.h"
#include "sys/util/string_conv.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;
using namespace shawn;

#define MAX_ERROR 10000000.0
#define PI 3.14159265

namespace motion_event
{

	// ----------------------------------------------------------------------
	MotionEventTask::
		MotionEventTask()
	{
		object_id_ = 0;
		velocity_ = 1.0;
		detection_range_ = 1.0;
		start_time_ = 0.0;
		standard_deviation_ = 0.0;
		max_time_error_ = MAX_ERROR;
		missing_detection_rate_ = 0.0;
		
		in_range_of_nodes_.clear();
		
		urv_ = new UniformRandomVariable();
		urv_->set_upper_bound(1.0);
		urv_->init();
		nrv_ = new NormalRandomVariable();
		nrv_->init();
	}
	
	// ----------------------------------------------------------------------
	MotionEventTask::
		~MotionEventTask()
	{
		in_range_of_nodes_.clear();
		
		delete urv_;
		urv_ = NULL;
		
		delete nrv_;
		nrv_ = NULL;
	}
	// ----------------------------------------------------------------------
	std::string
		MotionEventTask::
		name( void )
		const throw()
	{
		return "motion_event_task";
	}
	
	// ----------------------------------------------------------------------
	std::string
		MotionEventTask::
		description( void )
		const throw()
	{
		return "A generator for motion events in the world";
	}
	
	// ----------------------------------------------------------------------
	void
		MotionEventTask::
		run( shawn::SimulationController& sc )
		throw( std::runtime_error )
	{
		require_world( sc );
		
		string type  = sc.environment().optional_string_param("type", "object");
		
		if (type == "object")
		{
			string sensor_model = sc.environment().optional_string_param("sensor_model", "rectangle");
		
			object_id_ = sc.environment().required_int_param("object_id");
			velocity_ = sc.environment().required_double_param("velocity");
			detection_range_ = sc.environment().required_double_param("detection_range");
			start_time_ = sc.environment().required_double_param("start_time");
			standard_deviation_ = sc.environment().optional_double_param("standard_deviation", 0.0);
			max_time_error_ = sc.environment().optional_double_param("max_time_error", MAX_ERROR);
			missing_detection_rate_ = sc.environment().optional_double_param("missingDetRate", 0.0);

			{//Deprecated
				bool disc_range = sc.environment().optional_double_param("disc_instead_of_rect_range", false);
				if( disc_range ) 
				{
					sensor_model = "disc";
					cerr << "MotionEventTask: Parameter disc_instead_of_rect_range is DEPRECATED, use sensor_model=disc instead" << endl;
				}
			}
			
			in_range_of_nodes_.clear();
			
			if (urv_->upper_bound() != 1.0)
			{
				delete urv_;
				urv_ = new UniformRandomVariable();
				urv_->set_upper_bound(1.0);
				urv_->init();
			}
			
			for (shawn::World::node_iterator iter = sc.world_w().begin_nodes_w(); iter != sc.world_w().end_nodes_w(); ++iter) 
			{
				pair<shawn::Node*, bool> p;
				p.first = &(*iter);
				p.second = false;
				in_range_of_nodes_.push_back(p);
			}
	
			std::string path = sc.environment().required_string_param("object");
			
			// Reading of positions (vectors) of the object
			StrTok tok(path, ",;() ");
			StrTok::iterator it = tok.begin();
			
			for(int i = 0; i < tok.size() - 3; i += 3)
			{
				double x_pos = conv_string_to_double( *it );
				it++;
				double y_pos = conv_string_to_double( *it );
				it++;
				double z_pos = conv_string_to_double( *it );
				it++;		
				double x_dest = conv_string_to_double( *it );
				it++;
				double y_dest = conv_string_to_double( *it );
				it++;
				double z_dest = conv_string_to_double( *it );
				it-=2;	
	
				Vec position = Vec( x_pos, y_pos, z_pos );
				Vec destination = Vec( x_dest, y_dest, z_dest );
			
				if( sensor_model == "disc" ) 
					single_line_disc_range(sc, position, destination);
				else if( sensor_model == "rectangle" || sensor_model == "cone" ) 
					single_line(sc, position, destination, sensor_model);
				else 
				{
					cerr << "MotionEventTask: Unknown parameter sensor_model="<< sensor_model << endl;
					throw std::runtime_error("MotionEventTask: Unknown parameter for sensor_model");
				}
				
			}
		}
		else if (type == "errorDetections")
		{
			double iterations = sc.environment().required_double_param("duration");
			double errorDet = sc.environment().required_double_param("errorDetectionsPerTimeRange");
			double timeRange = sc.environment().optional_double_param("errorDetectionTimeRange", 86400.0);
			
			if (errorDet > 0)
			{
				if (urv_->upper_bound() != timeRange)
				{
					delete urv_;
					urv_ = new UniformRandomVariable();
					urv_->set_upper_bound(timeRange);
					urv_->init();
				}
				
				double random_variable = 0;
					
				for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it )
				{
					for (int i=0; i<(int)errorDet; ++i) 
					{
						random_variable = *urv_;
					
						//cout << "node=" << it->id() << " i=" << i << " urv=" << random_variable << endl;
						if (random_variable < iterations)
						{
							shawn::Node& node = *it;
							
							// Saving event information in a tag
							TagHandle th = node.find_tag_w("MotionEventTag");
							shawn::IntegerDoubleMapTag* mt;
							if ( th.is_not_null() )
								mt = dynamic_cast<IntegerDoubleMapTag*>(th.get());
							else 
								mt = new shawn::IntegerDoubleMapTag("MotionEventTag");

							std::ostringstream ossint, ossdouble;
							ossint << mt->value().size();
							ossdouble << random_variable;
							mt->add_indexed_entry( ossint.str(), ossdouble.str());
							mt->set_persistency(true);
							node.add_tag(mt);
						}
					}		
				}
			}
		}
	}
	
	// ----------------------------------------------------------------------
	void 
		MotionEventTask::
		single_line(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos, string sensor_model) 
	{
		/*
		Vec lower_left = Vec(min(start_pos.x(), dest_pos.x()), min(start_pos.y(), dest_pos.y()), min(start_pos.z(), dest_pos.z()) );
		Vec upper_right = Vec(max(start_pos.x(), dest_pos.x()), max(start_pos.y(), dest_pos.y()), max(start_pos.z(), dest_pos.z()) );
		lower_left -= Vec(detection_range_, detection_range_, 0);
		upper_right += Vec(detection_range_, detection_range_, 0);
		Box outer_box = Box(lower_left, upper_right);
		*/
		Vec det_range = Vec(detection_range_, detection_range_, 0);
		vector<Vec> start_dest_boxes;
		start_dest_boxes.push_back(start_pos + det_range);
		start_dest_boxes.push_back(start_pos + Vec(-1, 1, 0)*det_range);
		start_dest_boxes.push_back(start_pos + Vec(1, -1, 0)*det_range);
		start_dest_boxes.push_back(start_pos + Vec(-1, -1, 0)*det_range);
		start_dest_boxes.push_back(dest_pos + det_range);
		start_dest_boxes.push_back(dest_pos + Vec(-1, 1, 0)*det_range);
		start_dest_boxes.push_back(dest_pos + Vec(1, -1, 0)*det_range);
		start_dest_boxes.push_back(dest_pos + Vec(-1, -1, 0)*det_range);
		
		polygon::JarvisMarch jm = polygon::JarvisMarch();
		polygon::Polygon convex_hull = polygon::Polygon(jm.compute_convex_hull(start_dest_boxes));
		
		Box dest_box = Box(dest_pos - det_range, dest_pos + det_range);
		Vec direction = dest_pos - start_pos;
		double dir_norm = direction.euclidean_norm();
		bool sensor_check = false;
		
		for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it )
		{
			shawn::Node& node = *it;
			//cout << "Node: " << it->id() << endl;
			Vec n = node.real_position();
			//cout << "Sensor x: " << n.x()  << " y: "<< n.y() << " s " << start_pos.x() << " " <<  start_pos.y()<< " " << dest_pos.x() << " " << dest_pos.y() << endl;

			if( sensor_model == "rectangle" )
			{
				sensor_check = convex_hull.bounded_side(n) || convex_hull.on_boundary(n);
			}
			else if( sensor_model == "cone" )
			{
				//IDEE : Berechne Schnittpunkt zw. start - ende + plygon-kanten des sensorbereichs
				point tmp;

				point start_punkt;
				start_punkt.x = (float) start_pos.x();
				start_punkt.y = (float) start_pos.y();
				
				point end_punkt;
				end_punkt.x = (float) dest_pos.x();
				end_punkt.y = (float) dest_pos.y();

				line weg;
				weg.p=start_punkt;
				tmp.x = start_punkt.x - end_punkt.x;
				tmp.y = start_punkt.y - end_punkt.y;
				weg.a=tmp;

				//cout << "Weg x: " << weg.p.x  << " y: "<< weg.p.y << " x: " << weg.a.x << " y: " << weg.a.y << endl;

				point sensor;	
				sensor.x = (float) n.x();
				sensor.y = (float) n.y();

				point p;	
				p.x = (float) n.x() + (float)detection_range_;
				p.y = (float) n.y();

				point r;	
				r.x = (float)n.x()+((float)detection_range_*0.5);
				r.y = (float)n.y()+(-1*(sin(60*PI/180)*detection_range_));

				//cout << "Punt R: " << r.x << " y: " << r.y  <<  endl;

				point q;	
				q.x = (float)n.x()+((float)detection_range_*0.5);
				q.y = (float)n.y()+(1*(sin(60*PI/180)*detection_range_));

				line sensorr;
				sensorr.p = sensor;
				tmp.x = sensor.x - r.x ;
				tmp.y = sensor.y - r.y;
				sensorr.a = tmp;

				line rp;
				rp.p = r;
				rp.a = p;

				line pq;
				pq.p = p;
				pq.a = q;

				line qsensor;
				qsensor.p = q;
				qsensor.a = sensor;
				
				sensor_check = (convex_hull.bounded_side(n) || convex_hull.on_boundary(n) ) && checkSensor(weg, sensorr, rp, pq, qsensor, sensor, r, p, q);
			}

			
			if( sensor_check )
			{
				//Minimum distance between node and moving event
				double distance = ((cross_product((n - start_pos), direction)).euclidean_norm())/dir_norm;
	
				//computation of node projection on the line of the moving event 
				double lambda = ( (n - start_pos) * direction ) / (dir_norm * dir_norm) ;
				Vec intercept = start_pos + (direction * lambda);

				//If object was not in the detection range of node before
				if ( !in_range_of_node(&node) ) 
				{
					//compute the time, at which the node notices the motion event
					if (nrv_->standard_deviation() != standard_deviation_)
					{
						delete nrv_;
						nrv_ = new NormalRandomVariable();
						nrv_->set_standard_deviation(standard_deviation_);
						nrv_->init();
					}
					double random_variable = *nrv_;
					
					while (random_variable < ((-1.0)*max_time_error_/velocity_) || random_variable > (max_time_error_/velocity_))
						random_variable = *nrv_;
					
					double intercept_time = start_time_ + random_variable + euclidean_distance(start_pos, intercept)/velocity_;
					//cout << "intercept= " << start_time_ + euclidean_distance(start_pos, intercept)/velocity_ << endl;
					
					double urv = *urv_;
					//cout << "miss rate=" << missing_detection_rate_ << " urv=" << urv << endl; 
					if (urv >= missing_detection_rate_) 
					{
						//cout << "Object " << object_id_ << " hits node " << node.id() << " (" << node.real_position() << ") at position " << intercept << " at " << intercept_time << endl;
						//Saving event information in a tag
						TagHandle th = node.find_tag_w("MotionEventTag");
						shawn::IntegerDoubleMapTag* mt;
						if ( th.is_not_null() )
							mt = dynamic_cast<IntegerDoubleMapTag*>(th.get());
						else
							mt = new shawn::IntegerDoubleMapTag("MotionEventTag");
						
						std::ostringstream ossint, ossdouble;
						ossint << mt->value().size();
						ossdouble << intercept_time;
						
						mt->add_indexed_entry( ossint.str(), ossdouble.str());
						mt->set_persistency(true);
						node.add_tag(mt);
						
					}
					//else cout << "node " << node.id() << " misses event" << endl;
				}
			}
			
			// If person does not leave the detection range of the node set in_range_of_node = true
			if ( dest_box.contains(n) ) 
				set_in_range_of_node(&node, true);
			else 
				set_in_range_of_node(&node, false);
			
		}
		
		start_time_ += euclidean_distance(start_pos, dest_pos) / velocity_;
	}
	
	// ----------------------------------------------------------------------
	void 
		MotionEventTask::
		single_line_disc_range(shawn::SimulationController& sc, const shawn::Vec start_pos, const shawn::Vec dest_pos) 
	{
		Vec direction = dest_pos - start_pos;
		double dir_norm = direction.euclidean_norm();
		for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it )
		{
			shawn::Node& node = *it;
			//cout << "Node: " << it->id() << endl;
			Vec n = node.real_position();

			//Minimum distance between node and moving event
			double distance = ((cross_product((n - start_pos), direction)).euclidean_norm())/dir_norm;

			//cout << "Distance: " << distance << endl;
			if (distance <= detection_range_) 
			{
				//computation of node projection on the line of the moving event 
				double lambda = ( (n - start_pos) * direction ) / (dir_norm * dir_norm) ;
				Vec intercept = start_pos + (direction * lambda);

				//cout << "Nod_pos: " << n.x() << "," << n.y() << "," << n.z() << endl;
				//cout << "Dest: " << dest_pos.x() << "," << dest_pos.y() << "," << dest_pos.z() << endl;
				//cout << "Distance: " << euclidean_distance(n, dest_pos) << " <= " << detection_range_ << endl;
				
				//If (the projection is between pos_ and dest_ or if the person is near enough) and the person enters the detection_range
				//the node remarks the event
				if ( ( between(intercept, start_pos, dest_pos) || 
				(euclidean_distance(n, start_pos) <= detection_range_) || 
				(euclidean_distance(n, dest_pos) <= detection_range_) ) && 
				!in_range_of_node(&node) ) 
				{
					// If person does not leave the detection range of the node set in_range_of_node = true
					if ( euclidean_distance(n, dest_pos) <= detection_range_ ) 
						set_in_range_of_node(&node, true);
					else set_in_range_of_node(&node, false);

					//compute the time, at which the node remarks the motion event
					if (nrv_->standard_deviation() != standard_deviation_)
					{
						delete nrv_;
						nrv_ = new NormalRandomVariable();
						nrv_->set_standard_deviation(standard_deviation_);
						nrv_->init();
					}
					double random_variable = *nrv_;
					while (random_variable < (-1.0)*max_time_error_ || random_variable > max_time_error_)
						random_variable = *nrv_;
					double intercept_time = start_time_ + random_variable + euclidean_distance(start_pos, intercept)/velocity_;
				
					double urv = *urv_ + 0.00000001;
					//cout << "miss rate=" << missing_detection_rate_ << " urv=" << urv << endl;
					
					if (urv >= missing_detection_rate_) 
					{
						//cout << "Object " << object_id_ << " hits node " << node.id() << " (" << node.real_position() << ") at position " << intercept << " at " << intercept_time << endl;
						// Saving event information in a tag
						TagHandle th = node.find_tag_w("MotionEventTag");
						shawn::IntegerDoubleMapTag* mt;
						if ( th.is_not_null() )
							mt = dynamic_cast<IntegerDoubleMapTag*>(th.get());
						else
							mt = new shawn::IntegerDoubleMapTag("MotionEventTag");
						
						std::ostringstream ossint, ossdouble;
						ossint << mt->value().size();
						ossdouble << intercept_time;
						
						mt->add_indexed_entry( ossint.str(), ossdouble.str());
						mt->set_persistency(true);
						node.add_tag(mt);
						
					}
				}
				else set_in_range_of_node(&node, false);
			}
		}
		start_time_ += euclidean_distance(start_pos, dest_pos)/velocity_;
	}
	
	// ----------------------------------------------------------------------
	bool
		MotionEventTask::
		between(const Vec r, Vec p, Vec q) 
	{
		bool b = true;
		if (!((p.x() <= r.x() && r.x() <= q.x()) || (q.x() <= r.x() && r.x() <= p.x()))
			|| !((p.y() <= r.y() && r.y() <= q.y()) || (q.y() <= r.y() && r.y() <= p.y()))
			|| !((p.z() <= r.z() && r.z() <= q.z()) || (q.z() <= r.z() && r.z() <= p.z())))
			b = false;
		return b;
	}
	
	// ----------------------------------------------------------------------
	bool
		MotionEventTask::
		in_range_of_node(shawn::Node* n)
	{
		for (std::list<std::pair<shawn::Node*,bool> >::iterator it = in_range_of_nodes_.begin(); it != in_range_of_nodes_.end(); it++)
		{
			if ((*it).first == n)
				return (*it).second;
		}
		
		return false;
	}
	
	// ----------------------------------------------------------------------
	// better: use bitset
	void
		MotionEventTask::
		set_in_range_of_node(shawn::Node* n, bool b)
	{
		for (std::list<std::pair<shawn::Node*,bool> >::iterator it = in_range_of_nodes_.begin(); it != in_range_of_nodes_.end(); it++)
			if ((*it).first == n)
			{
				(*it).second = b;
				break;
			}
	}
	
	// ----------------------------------------------------------------------
	point 
		MotionEventTask::
		v_minus (point a, point b) 
	{
		point res;
		res.x = a.x - b.x;
		res.y = a.y - b.y;
		return res;
	}

	// ----------------------------------------------------------------------
	point 
		MotionEventTask::
		v_plus (point a, point b) 
	{
		point res;
		res.x = a.x + b.x;
		res.y = a.y + b.y;
		return res;
 	}

	// ----------------------------------------------------------------------
 	float 
		MotionEventTask::
		determinante(point a, point b) 
	{
    	return a.x * b.y - a.y * b.x;
 	}

	// ----------------------------------------------------------------------
	point 
		MotionEventTask::
		sv_mult (float s, point v) 
	{
		point res;
		res.x = s*v.x;
		res.y = s*v.y;
		return res;
	  }

	// ----------------------------------------------------------------------
	point 
	  	MotionEventTask::
	  	schnitt (line g1, line g2) 
	{
		float D_s = determinante (v_minus (g2.p, g1.p), sv_mult (-1, g2.a));
		float D   = determinante (g1.a, sv_mult (-1, g2.a));
		float s   = D_s / D;
		
		point res;
		res = v_plus (g1.p, sv_mult (s, g1.a));

		return res;
	}

	// ----------------------------------------------------------------------
	bool 
		MotionEventTask::
		kollinear (point a, point b) 
	{
		return (a.x * b.y - a.y * b.x) == 0;
	}
	
	// ----------------------------------------------------------------------
  	bool 
		MotionEventTask::
		parallel (line g1, line g2) 
	{
    	return kollinear (g1.a, g2.a);
  	}

	// ----------------------------------------------------------------------
	bool 
		MotionEventTask::
		checkSensor(line weg,line sensorr,line rp,line pq,line qsensor,point sensor,point r,point p,point q)
	{
		if (!parallel(weg,sensorr))
		{
			//cout << "SensorR x: " << sensorr.p.x  << " y: "<< sensorr.p.y << " x: " << sensorr.a.x << " y: " << sensorr.a.y << endl;
			point s = schnitt (weg,sensorr);
			//cout << "Schnittpunkt x: " << s.x  << " y: "<< s.y  << " "<< sensor.x << " "  << r.x<< endl;
			if ( (sensor.x <= s.x) & (r.x >= s.x) ) 
			{
				//cout << "Schnittpunkt innerhalb" << endl;
				return true;
			}
		}

		if (!parallel(weg,rp))
		{
			point s = schnitt (weg,rp);
			//cout << "Schnittpunkt x: " << s.x  << " y: "<< s.y  << " "<< r.x << " "  << p.x<< endl;
			if ( (r.x <= s.x) & (p.x >= s.x) ) 
			{
				//cout << "Schnittpunkt innerhalb" << endl;
				return true;
			}
		}

		if (!parallel(weg,pq))
		{
			point s = schnitt (weg,pq);
			//cout << "Schnittpunkt x: " << s.x  << " y: "<< s.y  << " "<< p.x << " "  << q.x<< endl;
			if ( (q.x <= s.x) & (p.x >= s.x) ) 
			{
				//cout << "Schnittpunkt innerhalb" << endl;
				return true;
			}
		}

		if (!parallel(weg,qsensor))
		{
			point s = schnitt (weg,qsensor);
			//cout << "Schnittpunkt x: " << s.x  << " y: "<< s.y  << " "<< q.x << " "  << sensor.x<< endl;
			if ( (sensor.x <= s.x) & (q.x >= s.x) ) 
			{
				//cout << "Schnittpunkt innerhalb" << endl;
				return true;
			}
		}

		return false;
	}

}

#endif //ENABLE_MOTIONEVENTTASK

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/tubsapps/motion_event_task/motion_event_task.cpp,v $
* Version $Revision: 1.2 $
* Date    $Date: 2007/01/31 17:24:16 $
*-----------------------------------------------------------------------
* $Log: motion_event_task.cpp,v $
* Revision 1.2  2007/01/31 17:24:16  krueger
* *** empty log message ***
*
* Revision 1.1  2007/01/29 16:37:14  krueger
* *** empty log message ***
*
* Revision 1.1  2005/03/05 06:43:10  krueger
* added motion_event_task
*
*-----------------------------------------------------------------------*/

