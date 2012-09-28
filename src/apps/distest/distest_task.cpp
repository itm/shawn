/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include "sys/simulation/simulation_environment.h"
#include "sys/node_distance_estimate.h"
#include "sys/misc/number_formatter.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "apps/distest/distest_task.h"
#include "apps/distest/histogram.h"
#include "sys/communication_model.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "sys/comm_models/communication_model_factory.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/node.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <cstring>

using namespace shawn;
using namespace std;

//-----------------------

struct Daten             
{                       
	int numberOfNeighbors_;        // Number of Neighbors
	double distanceError_;      // DistanceError between two Nodes (source_node -> target_node) Error with sign
	double absDistanceError_;   // DistanceError between two Nodes (source_node -> target_node) Error without sign
	int node1_ID;           // node1 Identifier 
	int node2_ID;           // node2 Identifier
	double node1_posX;      // X Co-ordinate of source-node
	double node1_posY;      // Y Co-ordinate of source-node
	double node2_posX;      // X Co-ordinate of target-node
	double node2_posY;      // Y Co-ordinate of target-node
	string direction_mode_;  // Direction Mode
};

typedef struct {
	int neighborhoodSize;
	int count;
	double relErrorUni;
	double absErrorUni;
	double relErrorAvg;
	double absErrorAvg;
	double relErrorMin;
	double absErrorMin;
	double relErrorMax;
	double absErrorMax;
} dat;

//----------------------- 

namespace distest
{ 

//	double neighborhoodInfo::container_size_ = 1.0;

	DistanceEstimateTask::
		DistanceEstimateTask()
	{
		initialized = false;
		fhc_ = new FileHandleCache("multiset");
				for (int i = 0; i<200;i++) {
					could[i]=0;
					couldnot[i]=0;
				}
	}

	void
		DistanceEstimateTask::
		init(shawn::SimulationController& sc)
	{
		cout << "init called" << endl;

		plus_ = minus_ = 0;

		perfect = sc.distance_estimate_keeper().find( "perfect_estimate" );
		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();

		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;

//		ContainerSize = sc.environment().required_double_param("container_size");
//		neighborhoodInfo::container_size_ = ContainerSize;

//		cout << "Containersize: " << ContainerSize << " count: " << 2/ContainerSize+1 << endl;

		initialized = true;
	}

	void 
		DistanceEstimateTask::
		GetMoments(const std::vector<double>& vContainer, const std::vector<double>& vData,double& avg, double& bias, double& sdev) {
			avg=bias=sdev = 0;
			for(int j = 0; j < int(vContainer.size()); j++) {
				avg+=fabs(vData[j]*vContainer[j]);
				bias+=vData[j]*vContainer[j];
			}

			for(int j = 0; j < int(vContainer.size()); j++) {
				sdev+=(vContainer[j]-bias)*(vContainer[j]-bias)*vData[j]*vData[j];
			}
			sdev = sqrt(sdev);
			if (!((avg>=-1.0)&&(avg<=1.0))) avg=0.0;
			if (!((bias>=-1.0)&&(bias<=1.0))) bias=0.0;
			if (!((sdev>=-1.0)&&(sdev<=1.0))) sdev=0.0;
		}


		// ----------------------------------------------------------------------
		/*DistanceEstimateTask::
		~DistanceEstimateTask()
		{
		}*/

		// ----------------------------------------------------------------------
		void
			DistanceEstimateTask::
			writeStats(std::string filename)
		{

			cout << "Writing Stats" << endl;
			delete(fhc_);
			DoubleHisto histo;
			histo = DoubleHisto(101);			
			histo.SetMinSpectrum(-1.01);
			histo.SetMaxSpectrum(1.01);

			//filename=filename+"abc";
			cout << "Writing file: " << filename << "...";
			
			vector < double > vCenterPositions ( histo.GetCenterContainers() );
			std::string out_file_short;
			out_file_short=filename+".uni";
			std::ofstream out_uni((char*)(out_file_short.c_str()));

			out_file_short=filename+".bidiavg";
			std::ofstream out_bidiavg((char*)(out_file_short.c_str()));

			out_file_short=filename+".bidiavg2";
			std::ofstream out_bidiavg2((char*)(out_file_short.c_str()));

			out_file_short=filename+".bidimin";
			std::ofstream out_bidimin((char*)(out_file_short.c_str()));

			out_file_short=filename+".bidimax";
			std::ofstream out_bidimax((char*)(out_file_short.c_str()));

			std::ofstream gnu_out("gnu3d.txt");
			std::ofstream gnu_out2("gnu3d_avg2.txt");




					cout << "opening files" << endl;
			
			out_uni      << "          #crit;";
			out_bidiavg  << "          #crit;";
			out_bidiavg2  << "          #crit;";
			out_bidimin  << "          #crit;";
			out_bidimax  << "          #crit;";
			for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
				out_uni  << DoubleFormatter(vCenterPositions[j]) << ";";
				out_bidiavg  << DoubleFormatter(vCenterPositions[j]) << ";";
				out_bidiavg2  << DoubleFormatter(vCenterPositions[j]) << ";";
				out_bidimin  << DoubleFormatter(vCenterPositions[j]) << ";";
				out_bidimax  << DoubleFormatter(vCenterPositions[j]) << ";";
			}
			out_uni      << "         #nodes;            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;
			out_bidiavg  << "         #nodes;            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;
			out_bidiavg2  << "         #nodes;            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;
			out_bidimin  << "         #nodes;            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;
			out_bidimax  << "         #nodes;            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;


			//Look for the highest key value
			int max = -1;
			for(vcMap::iterator it = uniErr.begin(), end = uniErr.end(); it!=end; ++it)
				if( it->first > max )
					max = it->first;

			//cout << "max="<< max << endl;
					//cout << "Before outer for" << endl;
			for(int i = 0; i <= max; i++){
			//cout << "i="<< i << endl;

				double crit;
				crit = i;
				if (criteria=="distance") crit = double(i) / 100;

						out_uni  << DoubleFormatter(crit) << ";";
						out_bidiavg  << DoubleFormatter(crit) << ";";
						out_bidiavg2  << DoubleFormatter(crit) << ";";
						out_bidimin  << DoubleFormatter(crit) << ";";
						out_bidimax  << DoubleFormatter(crit) << ";";

			

				int cur;
				double sum, abssum;
				double bias, mean, min, max;
				double median;
				double lower25;
				double upper25;
				double lower95;
				double upper95;
				int size;
				multiset<float>* s;
				vector < double > hist;

				//---------uni----------------------------------------


				if( uniErr.find(i) == uniErr.end() ) 
				{
					//cout << "No entry for "<<i<<" in uniErr" << endl << flush;
					/*for(int j = 0; j < vCenterPositions.size() + 8; j++) 	
					{
						out_uni  << DoubleFormatter(0) << ";";
						out_bidiavg  << DoubleFormatter(0) << ";";
						out_bidimin  << DoubleFormatter(0) << ";";
						out_bidimax  << DoubleFormatter(0) << ";";

					}*/
					out_uni   << endl << flush;
				} else {
					//cout << "uni i=" << i << endl << flush;
					histo = DoubleHisto(101);			
					histo.SetMinSpectrum(-1.01);
					histo.SetMaxSpectrum(1.01);
					//uniErr[i].close();                  //-------------------------
					//cout << "after uniErr["<<i<<"].close" << endl << flush;
					s = uniErr[i].getVals(i, "uni");             //-----------------
					assert( s!=NULL);
					//cout << "after getting the values" << endl << flush;
					size = s->size();


					//cout << "after uniErr["<<i<<"].getVals size="<<size << endl << flush;

					cur = 0;
					sum = abssum = 0.0;
					for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
						if ( cur == 0 ) {
							min = *it;
							lower25 = *it;
							lower95 = *it;
						}
						if ( cur == (size-1) ) max = *it;
	
						histo.Update(*it);
						if( (int)floor(size*0.05)==(cur+1)) lower95=*it;
						if( (int)floor(size*0.95)==(cur+1)) upper95=*it;
						if( (int)ceil(size*0.5)==(cur+1)) median=*it;
						if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
						if( (int)ceil(size*0.5)==(cur+1)) median=*it;
						if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
						cur++;
						sum+=(*it);
						abssum+=fabs(*it);
					}
					uniErr[i].clear();  //---------------------
					bias = sum / size;
					mean = abssum / size;
					//cout << "calculated uni data" << endl << flush;
					hist = vector< double >( histo.GetHistogramD() );
					for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
						double u;
						hist[j] = hist[j] / size;//dists[i].distCount;
						u = hist[j];
						if (!((u>=0.0)&&(u<=1.0))) u=0.0;
						out_uni  << DoubleFormatter(u) << ";";   //-------------------------
					}
	
					out_uni  << DoubleFormatter(nodeCount[i]) <<";"<< DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
					//------------------------------
					histo.Clear();	
				}


				if( bidiAvgErr.find(i) == bidiAvgErr.end() ) 
				{
					//cout << "No entry for "<<i<<" in BidiAvgErr" << endl << flush;
					/*for(int j = 0; j < vCenterPositions.size() + 8; j++) 	
					{
						out_uni  << DoubleFormatter(0) << ";";
						out_bidiavg  << DoubleFormatter(0) << ";";
						out_bidimin  << DoubleFormatter(0) << ";";
						out_bidimax  << DoubleFormatter(0) << ";";

					}*/

					out_bidiavg << endl << flush;
					out_bidiavg2 << endl << flush;
					out_bidimin  << endl << flush;
					out_bidimax  << endl << flush;
					continue;
				}

				//---------avg----------------------------------------
				//cout << "avg i=" << i << endl << flush;
				histo = DoubleHisto(101);			
				histo.SetMinSpectrum(-1.01);
				histo.SetMaxSpectrum(1.01);
				//bidiAvgErr[i].close();                  //-------------------------
				//cout << "after bidiAvgErr["<<i<<"].close" << endl << flush;
				s = bidiAvgErr[i].getVals(i, "avg");             //-----------------
				assert( s!=NULL);
				//cout << "after getting the values" << endl << flush;

				size = s->size();
				//cout << "after bidiAvgErr["<<i<<"].getVals size="<<size << endl << flush;
				cur = 0;
				sum = abssum = 0.0;
				for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
					histo.Update(*it);
						if ( cur == 0 ) {
							min = *it;
							lower25 = *it;
							lower95 = *it;
						}
					if ( cur == (size-1) ) max = *it;
					if( (int)floor(size*0.05)==(cur+1)) lower95=*it;
					if( (int)floor(size*0.95)==(cur+1)) upper95=*it;
					if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
					if( (int)ceil(size*0.5)==(cur+1)) median=*it;
					if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
					cur++;
					sum+=(*it);
					abssum+=fabs(*it);
				}
				bidiAvgErr[i].clear();  //---------------------
				bias = sum / size;
				mean = abssum / size;
				//cout << "calculated avg data" << endl << flush;

				hist  = vector< double >( histo.GetHistogramD() );
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					double u;
					hist[j] = hist[j] / size;//dists[i].distCount;
					u = hist[j];
					if (!((u>=0.0)&&(u<=1.0))) u=0.0;
					out_bidiavg  << DoubleFormatter(u) << ";";   //-------------------------
					gnu_out << 	crit << "\t" << vCenterPositions[j] << "\t" << u << endl;

				}

				out_bidiavg  << DoubleFormatter(nodeCount[i]) <<";"<< DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
				//------------------------------
				histo.Clear();	

				gnu_out << endl;


				//---------avg2----------------------------------------
				//cout << "avg i=" << i << endl << flush;
				histo = DoubleHisto(101);			
				histo.SetMinSpectrum(-1.01);
				histo.SetMaxSpectrum(1.01);
				//bidiAvgErr[i].close();                  //-------------------------
				//cout << "after bidiAvg2Err["<<i<<"].close" << endl << flush;
				s = bidiAvg2Err[i].getVals(i, "avg2");             //-----------------
				assert( s!=NULL);
				//cout << "after getting the values" << endl << flush;

				size = s->size();
				//cout << "after bidiAvg2Err["<<i<<"].getVals size="<<size << endl << flush;
				cur = 0;
				sum = abssum = 0.0;
				for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
					histo.Update(*it);
						if ( cur == 0 ) {
							min = *it;
							lower25 = *it;
							lower95 = *it;
						}
					if ( cur == (size-1) ) max = *it;
					if( (int)floor(size*0.05)==(cur+1)) lower95=*it;
					if( (int)floor(size*0.95)==(cur+1)) upper95=*it;
					if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
					if( (int)ceil(size*0.5)==(cur+1)) median=*it;
					if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
					cur++;
					sum+=(*it);
					abssum+=fabs(*it);
				}
				bidiAvg2Err[i].clear();  //---------------------
				bias = sum / size;
				mean = abssum / size;
				//cout << "calculated avg data" << endl << flush;

				hist  = vector< double >( histo.GetHistogramD() );
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					double u;
					hist[j] = hist[j] / size;//dists[i].distCount;
					u = hist[j];
					if (!((u>=0.0)&&(u<=1.0))) u=0.0;
					out_bidiavg2  << DoubleFormatter(u) << ";";   //-------------------------
					gnu_out2 << 	crit << "\t" << vCenterPositions[j] << "\t" << u << endl;

				}

				out_bidiavg2  << DoubleFormatter(nodeCount[i]) <<";"<< DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
				//------------------------------
				histo.Clear();	

				gnu_out2 << endl;


				//---------min----------------------------------------
				//cout << "min i=" << i << endl << flush;
				histo = DoubleHisto(101);			
				histo.SetMinSpectrum(-1.01);
				histo.SetMaxSpectrum(1.01);
				//bidiMinErr[i].close();                  //-------------------------
				//cout << "after bidiMinErr["<<i<<"].close" << endl << flush;
				s = bidiMinErr[i].getVals(i, "min");             //-----------------
								assert( s!=NULL);
				//cout << "after getting the values" << endl << flush;

				size = s->size();
				//cout << "after bidiMinErr["<<i<<"].getVals size="<<size << endl << flush;

				cur = 0;
				sum = abssum = 0.0;
				for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
					histo.Update(*it);
						if ( cur == 0 ) {
							min = *it;
							lower25 = *it;
							lower95 = *it;
						}
					if ( cur == (size-1) ) max = *it;
					if( (int)floor(size*0.05)==(cur+1)) lower95=*it;
					if( (int)floor(size*0.95)==(cur+1)) upper95=*it;
					if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
					if( (int)ceil(size*0.5)==(cur+1)) median=*it;
					if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
					cur++;
					sum+=(*it);
					abssum+=fabs(*it);
				}
				bidiMinErr[i].clear();  //---------------------
				bias = sum / size;
				mean = abssum / size;
				//cout << "calculated min data" << endl << flush;

				hist  = vector< double >( histo.GetHistogramD() );
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					double u;
					hist[j] = hist[j] / size;//dists[i].distCount;
					u = hist[j];
					if (!((u>=0.0)&&(u<=1.0))) u=0.0;
					out_bidimin  << DoubleFormatter(u) << ";";   //-------------------------
				}

				out_bidimin  << DoubleFormatter(nodeCount[i]) <<";"<< DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
				//------------------------------
				histo.Clear();	



				//---------max----------------------------------------
				//cout << "max i=" << i << endl << flush;				
				histo = DoubleHisto(101);			
				histo.SetMinSpectrum(-1.01);
				histo.SetMaxSpectrum(1.01);
				//bidiMaxErr[i].close();                  //-------------------------
				//cout << "after bidiMaxErr["<<i<<"].close" << endl << flush;
				s = bidiMaxErr[i].getVals(i, "max");             //-----------------
								assert( s!=NULL);
				//cout << "after getting the values" << endl << flush;

				size = s->size();
				//cout << "after bidiMaxErr["<<i<<"].getVals size="<<size << endl << flush;
				cur = 0;
				sum = abssum = 0.0;
				for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
					histo.Update(*it);
						if ( cur == 0 ) {
							min = *it;
							lower25 = *it;
							lower95 = *it;
						}
					if ( cur == (size-1) ) max = *it;
					if( (int)floor(size*0.05)==(cur+1)) lower95=*it;
					if( (int)floor(size*0.95)==(cur+1)) upper95=*it;
					if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
					if( (int)ceil(size*0.5)==(cur+1)) median=*it;
					if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
					cur++;
					sum+=(*it);
					abssum+=fabs(*it);
				}
				bidiMaxErr[i].clear();  //---------------------
				bias = sum / size;
				mean = abssum / size;
				//cout << "calculated max data" << endl << flush;

				hist  = vector< double >( histo.GetHistogramD() );
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					double u;
					hist[j] = hist[j] / size;//dists[i].distCount;
					u = hist[j];
					if (!((u>=0.0)&&(u<=1.0))) u=0.0;
					out_bidimax  << DoubleFormatter(u) << ";";   //-------------------------
				}

				out_bidimax  << DoubleFormatter(nodeCount[i]) <<";"<< DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
				//------------------------------
				histo.Clear();	
				//out_bidimax.flush;//----------------

			}
				out_uni.close();//----------------
			out_bidimin.close();//----------------

				out_bidimax.close();//----------------
				out_bidiavg.close();//----------------
				out_bidiavg2.close();//----------------
				gnu_out.close();
				gnu_out2.close();


	//	fhc_ = new FileHandleCache("multiset");
				cout << "done" << endl << flush;


//			cout << "minus_=" << minus_ << " plus_=" << plus_ << endl;

		}




		// ----------------------------------------------------------------------
		std::string 
			DistanceEstimateTask::
			name( void )
			const throw()
		{
			return std::string("distest");
		}

		// ----------------------------------------------------------------------
		std::string 
			DistanceEstimateTask::
			description( void )
			const throw()
		{
			return std::string("Compares the results of two distance estimates");
		}

		// ----------------------------------------------------------------------
		namespace {
			struct SortByLabel
			{
				bool operator() ( const Node* u,
					const Node* v ) const
				{ return u->label() < v->label(); }
			};

			typedef std::set<const Node*,SortByLabel> NodeSetByLabel;
		}

		// ----------------------------------------------------------------------
		void
			DistanceEstimateTask::
			run( shawn::SimulationController& sc )
			throw( std::runtime_error )
		{
			int neighborCount;
			int nodeID1 = 0;
			int nodeID2 = 0;

			unsigned long int densities = 0;
			int min_dens = 999999;
			int max_dens = 0;
			float all_count = 0.0;

			string cmd = sc.environment().optional_string_param("cmd", "distest");
				if (!initialized) init(sc);

			if (cmd == "commest") {
				cout << "---- Estimation Communication Model Function --- " << endl;

				shawn::PerfectDistanceEstimate* perfect_de_;
				shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "perfect_estimate" );	
				if (nei.is_not_null()) 
					perfect_de_ = (shawn::PerfectDistanceEstimate*)nei.get();
				else
					cout << "ERROR: TestTask::init: Could not retrieve perfect distance estimate." << endl;
				double r = sc.world().size_hint();
				double result;



				shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());

				for( shawn::World::const_node_iterator it1 = sc.world().begin_nodes(); it1 != sc.world().end_nodes(); ++it1) {            


					for( shawn::World::const_node_iterator it2 = sc.world().begin_nodes(); it2 != sc.world().end_nodes(); ++it2) {            

						perfect_de_->estimate_distance(*(it1), *(it2), result );
						result/=r;

						if (result <= 2.0 ) {
							int index = (int)(floor(result*100.0));
							//cout << index << endl;
							if (com->can_communicate_uni(*(it1), *(it2))) 
								could[index]++; else couldnot[index]++;
						}

					}
				}
			}
			if (cmd == "commset") {
				cout << "---- Setting Communication Model Function --- " << endl << flush;
				string cmf_type = sc.environment().required_string_param("cmf");
				CommunicationModelFunction* cmf;
				
				if (cmf_type == "circle") cmf = new CircleCommunicationModelFunction(); else

				if (cmf_type == "rim") {
				
					cmf = new RimModelFunction( *( dynamic_cast<shawn::RimCommModel*>(&(sc.world_w().communication_model_w()))), 0);
				}else
				
				if (cmf_type == "vector") {
					std::string fname = sc.environment().optional_string_param("file","");
					if(strcmp(fname.c_str(),"")==0){
					vector<double> v(200);
					for (int i = 0; i<200;i++) {
						if (could[i]+couldnot[i]>0)
							v[i] = (double)(could[i])/(double)(could[i]+couldnot[i]);
						else
							v[i]=0.0;
					}
					cmf = new VectorCommunicationModelFunction( v );
					for (int i = 0; i<200;i++) {
						could[i]=0;
						couldnot[i]=0;
					} 
					}
					else
					{ 
				      //std::vector<double> v=readVector(fname);
					  cmf = new VectorCommunicationModelFunction(readVector(fname));
					}
				} else
				if (cmf_type == "linear") {
					double sm =  sc.environment().optional_double_param("smooth_factor", 0.25);
					cmf = new LinearCommunicationModelFunction(sm); 
				} else { 
					cout << "ERROR: no valid CommunicationModelFunction type specified" << endl << flush;
					assert ( false );
				}
				
				nptr->setCommunicationModelFunction( cmf );
				if(sc.environment().optional_bool_param("print_info",true)){
				nptr->gnuplotEstimationTable();
				cmf->gnuplot3D( "gnuplot3D.txt", false, 0.0 );
				cmf->gnuplot3D( "gnuplot3D-mult-d_0.txt", true, 0.0 );
				cmf->gnuplot3D( "gnuplot3D-mult-d_1.txt", true, 1.0 );
				cmf->gnuplot2D( "gnuplot2D.txt" );
				}

			}

			if (cmd == "distest") {
				cout << "---- Estimating Distances --- " << endl;
				//int density = sc.environment().required_int_param("density");
				//preconditions ok?
				require_world(sc);
				assert( perfect != NULL && nptr != NULL );
				const World& w = sc.world();
				assert( w.has_size_hint());

				//int this run...
				double r = w.size_hint();
				criteria = sc.environment().required_string_param("criteria");
				//int density = sc.environment().required_int_param("density");
				cout << "DistEstTask.r=" << r << endl;
				bool perfdistok = false, unidistok = false, bidiavgdistok = false, bidiavg2distok = false, bidimindistok = false, bidimaxdistok = false;
				double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0, bidiavg2dist = 0.0;
				shawn::Vec rr(r, r, 0);
				shawn::Vec ll = w.lower_left() + rr;
				shawn::Vec ur = w.upper_right() - rr;

				bool exclude_border = (  sc.environment().optional_int_param("exclude_border", 1) == 1);

				cout << "Running over all nodes..." << flush;
				for( shawn::World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes();it != endit; ++it) {
					if( out_of_bounds(ll, ur, *it ) && exclude_border) //TODO: Zum initialen Testen muss nachher raus
						continue;


					neighborCount = (*it).degree(EdgeModel::CD_IN);
					all_count+= 1.0;
					densities += neighborCount;
					if (neighborCount > max_dens) max_dens = neighborCount;
					if (neighborCount < min_dens) min_dens = neighborCount;

				}

				double avg_dens = (double)(densities / all_count);
				cout << "DENSITY: average=" << avg_dens << " min=" << min_dens << " max=" << max_dens << endl << flush;


				for( shawn::World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes();it != endit; ++it) 
				{            
					nodeID1 = (*it).id();

					//cout << nodeID1 << endl << flush;

					if( out_of_bounds(ll, ur, *it ) && exclude_border) //TODO: Zum initialen Testen muss nachher raus
						continue;

					neighborCount = (*it).degree(EdgeModel::CD_IN);
					/*
					all_count+= 1.0;
					densities += neighborCount;
					if (neighborCount > max_dens) max_dens = neighborCount;
					if (neighborCount < min_dens) min_dens = neighborCount;

					*/
					nodeCount[neighborCount]++;

					for(shawn::Node::const_adjacency_iterator itnb = (*it).begin_adjacent_nodes(), end = (*it).end_adjacent_nodes(); itnb != end; ++itnb) 
					{
						nodeID2 = (*itnb).id();

						if( out_of_bounds(ll, ur, *itnb )&& exclude_border ) //TODO: Zum initialen Testen muss nachher raus
							continue;

						if(nodeID1 == nodeID2)
						{
							continue;
						}	

						// 		cout << "vor den Distestdingern" << endl << flush;

						perfdistok = perfect->estimate_distance( (*it), *itnb, perfdist );
						nptr->set_mode(M_UNI);
						unidistok = nptr->estimate_distance( (*it), *itnb, unidist );
						nptr->set_mode(M_BIDI_AVG);
						bidiavgdistok = nptr->estimate_distance( (*it), *itnb, bidiavgdist );
						nptr->set_mode(M_BIDI_AVG2);
						bidiavg2distok = nptr->estimate_distance( (*it), *itnb, bidiavg2dist );
						nptr->set_mode(M_BIDI_MIN);
						bidimindistok = nptr->estimate_distance( (*it), *itnb, bidimindist );
						nptr->set_mode(M_BIDI_MAX);
						bidimaxdistok = nptr->estimate_distance( (*it), *itnb, bidimaxdist );

						//if (bidiavgdist != bidiavg2dist) cout << "avg=" << bidiavgdist << " avg2=" <<  bidiavg2dist << endl;

						{
							if( perfdistok && unidistok && bidiavgdistok && bidimindistok && bidimaxdistok)
							{
								//							if ((perfdist-unidist) <0) minus_++; else plus_++;

								int container;
								if (criteria=="density") container = (int)ceil(avg_dens); else 
									if (criteria=="neighbors") container = neighborCount; else 
									if (criteria=="avg_neighbors") container =(it->degree()+itnb->degree())/2; else 
										if (criteria=="distance") container = (int)floor(perfdist *10.0); else {
											cout << "No correct distinction criteria selected, must be \"density\", \"neighbors\" or \"distance\" but is " << criteria << endl;
											throw std::runtime_error("No correct distinction criteria selected, must be \"neighbors\" or \"distance\"");
										}

										//cout << "container=" << container << " criteria=" << criteria << endl;

										uniErr[container].set_file_handle_cache(fhc_);
										uniErr[container].insert(container, "uni",float((unidist-perfdist)/r));
										
										bidiAvgErr[container].set_file_handle_cache(fhc_);
										bidiAvgErr[container].insert(container, "avg", float((bidiavgdist-perfdist)/r));
										
										bidiAvg2Err[container].set_file_handle_cache(fhc_);
										bidiAvg2Err[container].insert(container, "avg2", float((bidiavg2dist-perfdist)/r));
										
										bidiMinErr[container].set_file_handle_cache(fhc_);
										bidiMinErr[container].insert(container, "min", float((bidimindist-perfdist)/r));
										
										bidiMaxErr[container].set_file_handle_cache(fhc_);
										bidiMaxErr[container].insert(container, "max", float((bidimaxdist-perfdist)/r));
										//edgeCount[neighborCount]++;


							}
						}
					}

				}

				cout << "done" << endl;
/*				int maxEdges = 0;
				int ns;


				for(int i = 0; i<250;i++){ 
				if( neigh[i].edgeCount > maxEdges) {
				maxEdges = neigh[i].edgeCount;
				ns = i;
				}
				}
				cout << "Maximum edge count=" << maxEdges << " at i=" << ns << endl;
				*/

				std::string filename = sc.environment().required_string_param("file_name");
				if (filename != "") writeStats( filename );
			}
			
		}

		// ----------------------------------------------------------------------
		bool 
			DistanceEstimateTask::
			out_of_bounds(shawn::Vec& ll, shawn::Vec& ur, const shawn::Node& node)  
			throw()
		{

			if( node.real_position().x() > ur.x() || node.real_position().y() > ur.y() ||
				node.real_position().x() < ll.x() || node.real_position().y() < ll.y() )
				return true;

			return false;
		}
		std::vector<double>
			DistanceEstimateTask::
			readVector(std::string& fname) throw(std::runtime_error)
		{
			ifstream in(fname.c_str());
			if( !in )
				throw std::runtime_error(string("cannot load file ")+fname);
			std::vector<double> v;
			double value = 0;
			in >>value;
			while(in >>  value){
				v.push_back(value);
			}
		in.close();
		return v;
		}
}
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/distest_task.cpp,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: distest_task.cpp,v $
 *-----------------------------------------------------------------------*/
