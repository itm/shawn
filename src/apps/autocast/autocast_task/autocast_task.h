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

#ifndef __SHAWN_LEGACYAPPS_AUTOCASTTASK_H
#define __SHAWN_LEGACYAPPS_AUTOCASTTASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "apps/autocast/autocast_app/dataunit.h"
#include "sys/node.h"
#include <vector>

namespace autocast
{
 /**
  * The AutoCastTask gathers statistical data from the AutoCastProcessor.
  *
  * @author Torsten Teubler
  */
   class AutoCastTask
      : public shawn::SimulationTask
   {
   public:
      AutoCastTask();
      virtual ~AutoCastTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

	  void process_latencies(const autocast::ConstDataUnitHandle&, const shawn::Node*, const shawn::SimulationController&) throw();

	  void process_sent_statistic(const shawn::Node* node, int packet_sent, int byte_sent , int dataUnit_sent, int dataUnit_byte_sent, int packet_received, int dataUnits_received ,int received_DataUnit_ids, double active_time, int msgCountBeacon, int msgCountAnswer, int msgCountAnswer_only, int msgCountFlood, int msgCountFlood_only, int msgCountRequest, int msgCountRequest_only) throw();

   private:
	   struct latencies_line{
         double simulation_time;
		    const shawn::Node* from_addr;
			const shawn::Node* node_addr;
			double time;
			double latency_s;
			double dist;
			double latency_s_per_km;
			double propagation_speed;
	   };
	   std::vector<latencies_line*> latencies_;

	   struct sent_statistics_line{
		    const shawn::Node* node_addr;
			int packet_sent;
			int byte_sent;
			int dataUnit_sent;
			int dataUnit_byte_sent;
			int packets_received;
			int dataUnits_received;
			int received_DataUnit_ids;
			double active_time;
         int msgCountBeacon;
         int msgCountAnswer;
         int msgCountAnswer_only;
         int msgCountFlood;
         int msgCountFlood_only;
         int msgCountRequest;
         int msgCountRequest_only;
         /*double neighbors_count_;
			double real_neighbors_count_;
			double velocity_;
			double update_time_;*/
	   };
	   std::vector<sent_statistics_line*> sent_statistics_;

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/


