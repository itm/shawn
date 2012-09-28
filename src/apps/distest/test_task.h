/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_DISTEST_TESTTASK_H
#define __SHAWN_APPS_DISTEST_TESTTASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include <map>
#include <vector>
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include "apps/distest/histogram.h"
#include "sys/comm_models/rim_comm_model.h"
#include "apps/distest/distest_postscript_writer.h"
#include "apps/distest/value_collection.h"

#include "sys/node_distance_estimate.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"

namespace shawn {class Node;}

namespace distest
{
typedef std::map<int, valueCollection> vcMap;
typedef std::vector< std::vector<int> > INTMatrix;
   class TestTask
      : public shawn::SimulationTask
   {
   public:
      TestTask();
      virtual ~TestTask();
	  virtual void init(shawn::SimulationController& sc);

	   virtual std::string name( void )
         const throw();
      virtual std::string description( void )
         const throw();
	  virtual void paint_rim(DIPostscriptWriter& psc, shawn::RimCommModel& rim, Node& n)
			throw( std::runtime_error );
    virtual void paint_scene(DIPostscriptWriter& psc, Node& n1, Node& n2 ) throw( std::runtime_error );
    virtual void paint_nodes(DIPostscriptWriter& psc ) throw( std::runtime_error );
	virtual void paint_neighbors(DIPostscriptWriter& psc, Node& n, shawn::EdgeModel::CommunicationDirection d ) throw( std::runtime_error );

	  virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
	virtual std::string getIdString( const shawn::SimulationController& sc ) const throw();
        bool out_of_bounds(shawn::Vec& ll, shawn::Vec& ur, const shawn::Node& node) throw();
		void paint_scenario( shawn::SimulationController& sc, Node& n1, Node& n2, double frac );
	void calc_communication_probability_function( shawn::SimulationController& sc ) throw( std::runtime_error );


		DIPostscriptWriter *psc;	
		bool initialized;
		int could[200];
		int couldnot[200];
		std::ofstream *twoDfile_;

		std::vector<std::ofstream*> gp_file;

		vcMap uniErr;
		FileHandleCache *fhc_;
		double simus;
		double multi_sim_avg_dens;
		shawn::SimulationController* sc_;
	   shawn::ConstNodeDistanceEstimateHandle perfect;
	   shawn::NeighborhoodIntersectionDistanceEstimate* nptr;
	   std::vector<int> density_histogram_;
	   std::map<int,INTMatrix> results;
	   int source_bigger, target_bigger;

   };

}

#endif
#endif
