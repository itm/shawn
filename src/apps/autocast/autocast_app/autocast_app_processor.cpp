/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 ** \author Torsten Teubler <teubler@itm.uni-luebeck.de>			   **
 **                                                                    **
 ************************************************************************/

#include "autocast_app_processor.h"
#ifdef ENABLE_AUTOCAST

#include <iostream>
#include <cfloat>
#include <cassert>
#include <cmath>
#include "sys/node.h"
#include "apps/autocast/autocast_app/autocast_app_processor.h"
#include "apps/autocast/autocast/autocast_processor.h"
#include "apps/autocast/autocast_app/autocast_app_message.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/vec.h"
#include "sys/world.h"
#include "test_dataunit.h"
#include "distribution_area_circle.h"

using namespace std;
using namespace shawn;

int autocast::AutoCastAppProcessor::dataUnit_id_counter_ = 0;

namespace autocast
{
   AutoCastAppProcessor::
	   AutoCastAppProcessor() : AutoCastApplication(),
								sending_point_x_(0),
								sending_point_y_(0),
								disseminate_every_n_round_(1)

   {}
   // ----------------------------------------------------------------------
   AutoCastAppProcessor::
   ~AutoCastAppProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   AutoCastAppProcessor::
   boot( void )
      throw()
   {
	  sending_point_x_ = owner().world().simulation_controller().environment().required_double_param("sending_point_x");
	  sending_point_y_ = owner().world().simulation_controller().environment().required_double_param("sending_point_y");
   }
   // ----------------------------------------------------------------------
   void
   AutoCastAppProcessor::
   special_boot( void )
      throw()
   {
	   disseminate_every_n_round_ = owner().world().simulation_controller().environment().optional_int_param("disseminate_every_n_round",1);
	   assert( disseminate_every_n_round_ > 0 );
   }
   // ----------------------------------------------------------------------
   bool
   AutoCastAppProcessor::
   process_message( const ConstMessageHandle& mh )
      throw()
   {
      return Processor::process_message( mh );
   }
   // ----------------------------------------------------------------------
   void
   AutoCastAppProcessor::
   work( void )
      throw()
   {
	   // Fake a sending position
	   if(owner().is_special_node()){
		   if ( (owner().simulation_round() - 30) % disseminate_every_n_round_ == 0 && disseminate_every_n_round_ != 999999 ){
			   double min_distance = DBL_MAX;
			   int min_node = 0;
			   for(shawn::World::node_iterator it = owner_w().world_w().begin_nodes_w();
				   it != owner_w().world_w().end_nodes_w(); it++){
					   double distance = /*sqrt(*/
						   pow((sending_point_x_ - it->real_position().x()),2)+
						   pow((sending_point_y_ - it->real_position().y()),2)/*)*/;
						   if( distance < min_distance ){
								min_distance = distance;
								min_node = it->id();
						   }
			   }
			   shawn::Node * node = owner_w().world_w().find_node_by_id_w(min_node);
			   assert(node);
					AutoCastAppProcessor * ac_appp = node->get_processor_of_type_w<AutoCastAppProcessor>();
			   assert(ac_appp);
					std::cout << "***************************************************" << std::endl;
					std::cout << "The min node is: " << node->id() << std::endl;
					std::cout << "The min node position is: (" << node->real_position().x() << ", " << node->real_position().y() << ")" << std::endl;
					std::cout << "The min node velocity is: " << node->movement().velocity().euclidean_norm() << std::endl;
					std::cout << "***************************************************" << std::endl;
					ac_appp->disseminate_DataUnit();
		   }
	   }
   }
   // ----------------------------------------------------------------------
   bool
	   AutoCastAppProcessor::
	   receive_DataUnit(const autocast::ConstDataUnitHandle& duh)
	   throw()
   {
	   bool dispatch_DataUnit = owner().world().simulation_controller().environment().optional_bool_param("AutoCastApp_dispatch_DataUnit",false);
	   return dispatch_DataUnit;
   }
   // ----------------------------------------------------------------------
   const autocast::ConstDataUnitHandle&
	   AutoCastAppProcessor::
	   refresh(const autocast::ConstDataUnitHandle& duh)
	   throw()
   {
		return duh;
   }
   // ----------------------------------------------------------------------
   void
	   AutoCastAppProcessor::
	   disseminate_DataUnit()
	   throw()
   {
	   double max_life_time = owner().world().simulation_controller().environment().optional_double_param("TestDataUnit_max_life_time",600);
	   int priority = owner().world().simulation_controller().environment().optional_int_param("TestDataUnit_priority",0);
	   double dist_area_radius = owner().world().simulation_controller().environment().optional_int_param("TestDataUnit_DistributionAreaCircle_radius",26000);
	   int dataUnit_size = owner().world().simulation_controller().environment().optional_int_param("TestDataUnit_size",100);
	   dataUnit_id_counter_++;
	   DistributionArea * area = new DistributionAreaCircle(sending_point_x_,sending_point_y_,dist_area_radius);
	   const ConstDataUnitHandle tdu = new TestDataUnit(dataUnit_id_counter_,
														&owner(),
														/*owner().real_position().x(),
														owner().real_position().y(),
														owner().world().scheduler().current_time(),*/
														max_life_time,
														priority,
														/*application id: */"1",
														area,
														dataUnit_size);
	   AutoCastProcessor * acp = owner_w().get_processor_of_type_w<AutoCastProcessor>();
	   assert(acp != NULL);
			acp->send_to(tdu,NULL);
   }



}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
