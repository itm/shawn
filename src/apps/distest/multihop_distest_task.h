/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_MULTIHOP_DISTEST_TASK_H
#define __SHAWN_APPS_MULTIHOP_DISTEST_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include "apps/distest/histogram.h"
#include "sys/node_distance_estimate.h"
#include "apps/distest/distest_postscript_writer.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "apps/distest/value_collection.h"
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>

using namespace std;

namespace shawn {class Node;}

typedef unsigned int uint;
typedef vector< vector<double> > FloatMatrix;
typedef vector< vector<int> > IntMatrix;
typedef vector< vector<uint> > UINTMatrix;


namespace distest
{

	typedef THistogram<double,double> DoubleHisto;
/*
	class valueCollection {


	public:
		valueCollection() {
			file = true;
			err = NULL;
			fh = NULL;
		}

		std::ofstream* newFile(int key) {
			ostringstream os;
			os << "multiset" << key << ".ms";
			return new std::ofstream(os.str().c_str() );

		}
		
		void insert( int key, float val ) {
			if (file) {
				if (fh == NULL) fh = newFile(key);
				(*fh) << val;
				(*fh) << " ";

			} else {
				if (err==NULL) err = new multiset<float>();
				err->insert(val);

			}

		}

		void close() {
			if (fh != NULL) {
				fh->flush();
				fh->close();
			}
		}

		void clear() {
			if (file) {
				if (fh!=NULL) delete(fh); fh=NULL;
			} else 
				if (err!=NULL) delete(err); err = NULL;
		}

		multiset<float>* getVals(int key) {
			if (file) {
				ostringstream os;
				os << "multiset" << key << ".ms";
				std::ifstream f(os.str().c_str() );
				err = new multiset<float>();
				float val;
				char c;
				while ( f >> val) { err->insert(val); f.ignore();}

				return err;
			} else {
				return err;
			}
		}


		~valueCollection() {
			if (file) {
				if (fh!=NULL) delete(fh);
			} else 
				if (err!=NULL) delete(err);
		}
	protected:
		friend class MultihopDistanceEstimateTask;
		//friend class DistanceEstimateTask;

	private:
		bool file;
		multiset<float>* err;
		std::ofstream* fh;

	};


*/

   class MultihopDistanceEstimateTask
      : public shawn::SimulationTask
   {
   public:

	   typedef std::map<int, valueCollection> DistMap;
	   DistMap dists;





	   double ContainerSize;
	   bool initialized;
	   int density;
	   std::vector<int> v;

			MultihopDistanceEstimateTask();
			~MultihopDistanceEstimateTask();
			virtual void init( shawn::SimulationController& sc );
			virtual void paint( shawn::SimulationController& sc, DIPostscriptWriter *psc, int nodeId );
			virtual void paint2( shawn::SimulationController& sc, DIPostscriptWriter *psc, const shawn::Node& n1, const shawn::Node& n2 );
			void GetMoments(const std::vector<double>& vContainer, const std::vector<double>& vData,double& avg, double& bias, double& sDev);
			std::list<const Node*> getPath( shawn::SimulationController& sc, const shawn::Node& n1, const shawn::Node & n2 );
			void writeStats(std::string filename, shawn::SimulationController& sc);
			virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );
			virtual std::string description( void )	const throw();
			virtual std::string name( void ) const throw();
			IntMatrix getDistances( shawn::NodeDistanceEstimate& nde, shawn::SimulationController& sc ) throw();
			bool out_of_bounds(const shawn::Node& node) throw();
			virtual std::string getIdString( const shawn::SimulationController& sc ) const throw();


   private	:
			IntMatrix dist;
			UINTMatrix pred;
			DIPostscriptWriter *psW;
			int ok_counter, not_ok_counter;
			int max;
			int PS;
			shawn::PerfectDistanceEstimate* perfect_de_;
			shawn::Vec rr, ll, ur;
			FileHandleCache *fhc_;

   };

}

#endif
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/multihop_distest_task.h,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: multihop_distest_task.h,v $
 *-----------------------------------------------------------------------*/
