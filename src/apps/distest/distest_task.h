/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_DISTEST_TASK_H
#define __SHAWN_APPS_DISTEST_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include "apps/distest/histogram.h"
#include "apps/distest/value_collection.h"

#include "sys/node_distance_estimate.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"


#include <map>

namespace shawn {class Node;}

using namespace shawn;

namespace distest
{

	typedef THistogram<double,double> DoubleHisto;

/*
	class neighborhoodInfo {

		static double container_size_;

	public:

		neighborhoodInfo() {

			nodeCount=0;
			edgeCount=0;

			uniErr = DoubleHisto((unsigned int) (2/container_size_+1));
			uniErr.SetMinSpectrum(-1.0-container_size_/2);
			uniErr.SetMaxSpectrum(1.0+container_size_/2);

			bidiAvgErr = DoubleHisto((unsigned int) (2/container_size_+1));
			bidiAvgErr.SetMinSpectrum(-1.0-container_size_/2);
			bidiAvgErr.SetMaxSpectrum(1.0+container_size_/2);

			bidiMinErr = DoubleHisto((unsigned int) (2/container_size_+1));
			bidiMinErr.SetMinSpectrum(-1.0-container_size_/2);
			bidiMinErr.SetMaxSpectrum(1.0+container_size_/2);

			bidiMaxErr = DoubleHisto((unsigned int) (2/container_size_+1));
			bidiMaxErr.SetMinSpectrum(-1.0-container_size_/2);
			bidiMaxErr.SetMaxSpectrum(1.0+container_size_/2);
		}

	protected:
		int nodeCount;
		int edgeCount;
		DoubleHisto uniErr;
		DoubleHisto bidiAvgErr;
		DoubleHisto bidiMinErr;
		DoubleHisto bidiMaxErr;

		friend class DistanceEstimateTask;
	};

*/
   class DistanceEstimateTask
      : public shawn::SimulationTask
   {
   public:
	   double plus_, minus_;
//	   double ContainerSize;
	   bool initialized;
	   shawn::ConstNodeDistanceEstimateHandle perfect;
	   shawn::NeighborhoodIntersectionDistanceEstimate* nptr;
		std::map<int, int> nodeCount;
		typedef std::map<int, valueCollection> vcMap;
		vcMap uniErr; 
		vcMap bidiAvgErr; 
		vcMap bidiAvg2Err; 
		vcMap bidiMinErr; 
		vcMap bidiMaxErr; 
      DistanceEstimateTask();
      //virtual ~DistanceEstimateTask();


	  void GetMoments(const std::vector<double>& vContainer, const std::vector<double>& vData,double& avg, double& bias, double& sDev);
	  void writeStats(std::string filename);
      virtual std::string name( void )
         const throw();
      virtual std::string description( void )
         const throw();
      virtual void init( shawn::SimulationController& sc );

	  virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
	  virtual std::vector<double> readVector(std::string&)
		  throw( std::runtime_error );
        bool out_of_bounds(shawn::Vec& ll, shawn::Vec& ur, const shawn::Node& node) throw();
		FileHandleCache *fhc_;
		int could[200];
		int couldnot[200];
		string criteria;

    };

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/distest_task.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: distest_task.h,v $
 *-----------------------------------------------------------------------*/
