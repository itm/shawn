/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_DISTEST

#include "apps/distest/multihop_distest_task.h"
#include "sys/node_distance_estimate.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/misc/number_formatter.h"
#include "apps/distest/distest_postscript_writer.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"
#include "apps/distest/value_collection.h"
/*
#include "sys/simulation/simulation_environment.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"

#include "apps/distest/histogram.h"
*/

#include "sys/node.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace shawn;
using namespace std;



#define UINT_INF 9999
#define FLOAT_INF 99999.0

namespace distest
{ 
	//double distanceInfo::container_size_ = 0;


	MultihopDistanceEstimateTask::
		MultihopDistanceEstimateTask()
		{

			initialized = false;
			psW = NULL;
			PS = 0;
			max=0;
			fhc_ = new FileHandleCache("multiset");
			v = std::vector<int>( 500 );
			for ( int i = 0; i<500; i++ ) v[i]=0;
			not_ok_counter = 0;
			ok_counter = 0;


		}

	MultihopDistanceEstimateTask::
		~MultihopDistanceEstimateTask()
		{
			if (psW != NULL) delete(psW);
			PS = 0;
		}


		// ----------------------------------------------------------------------
		std::string 
			MultihopDistanceEstimateTask::
			description( void )
			const throw()
		{
			return std::string("Evaluate multi hop distance estimates");
		}

	void
		MultihopDistanceEstimateTask::
		init(shawn::SimulationController& sc)
		{
			cout << "      Initializing...";

			//ContainerSize = sc.environment().required_double_param("container_size");
			PS = sc.environment().optional_int_param("ps", 0);
			//distanceInfo::container_size_ = ContainerSize;
			if (PS) {
				ofstream *psout_c = new ofstream("out.ps");
				psW = new DIPostscriptWriter(*psout_c, sc);
			}
			//cout << ContainerSize << endl;
			shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "perfect_estimate" );	
			if (nei.is_not_null()) 
				perfect_de_ = (shawn::PerfectDistanceEstimate*)nei.get();
			else
				cout << "ERROR: MultihopDistanceEstimateTask::init: Could not retrieve perfect distance estimate." << endl;
			//double r = sc.world().size_hint();
			rr = sc.world().upper_right() / 4;
			ll = sc.world().lower_left() + rr;
			ur = sc.world().upper_right() - rr;

			initialized = true;
			cout << "done." << endl;
		}


	void
		MultihopDistanceEstimateTask::
		paint(shawn::SimulationController& sc, DIPostscriptWriter *psc, int nodeId )
		{
			psc->calcOffset(sc.world());


			
			Vec v1, v2;
			//int node_count = sc.world().node_count();

			psc->setlinewidth2(0.1);			
			psc->setcolor2(0.8, 0.8, 0.8);
				for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            

				v1 = (*it).real_position();
				int id1 = (*it).id();
				//if (rim->can_communicate_uni(*n, (*it))) psc->setcolor2(0, 255, 0); else psc->setcolor2(255, 0, 0);

				psc->procircle3(v1, 0.5, true );
	        
				if (id1 == nodeId) psc->procircle3(v1, 10, false ); // TODO: aquire the actual communication range

				const shawn::Node& nb = *it;
		        for(shawn::Node::const_adjacency_iterator nit = nb.begin_adjacent_nodes(), nend = nb.end_adjacent_nodes(); nit!=nend; ++nit)
			    {
					v2 = (*nit).real_position();
					int id2 = (*nit).id();
					if (id1<id2) psc->proline2(v1, v2);
				}
			}


			psc->setcolor2(0, 0, 0);			
			v1 = sc.world().find_node_by_id(nodeId)->real_position();

			psc->procircle3(v1, 0.5, true );
			
			int i;
			for( shawn::World::const_node_iterator it1 = sc.world().begin_nodes(), end = sc.world().end_nodes(); it1 != end; ++it1) {

//			for ( int i = 0; i < node_count; i++) {
				i = it1->id();
				if ( (pred[i][nodeId] != UINT_INF)&&(!out_of_bounds(*it1))){
					int j = nodeId;
					while (int(pred[i][j]) != i) {
						int tmp = pred[i][j];
						v1=sc.world().find_node_by_id(j)->real_position();
						v2=sc.world().find_node_by_id(tmp)->real_position();
						psc->proline2(v1, v2);
						j = tmp;
					}
					v1=it1->real_position();
					v2=sc.world().find_node_by_id(j)->real_position();
					psc->proline2(v1, v2);
				}
			}
			psc->text( Vec(0, 0), getIdString(sc) );
			psc->mystroke();

			psc->newpage();
		}

	void
		MultihopDistanceEstimateTask::
		paint2(shawn::SimulationController& sc, DIPostscriptWriter *psc, const shawn::Node& n1, const shawn::Node& n2 )
		{
			psc->calcOffset(sc.world());

			Vec offset(1,1,0);
			Vec v1, v2;
			//int node_count = sc.world().node_count();

			psc->setlinewidth2(0.1);			
			psc->setcolor2(0.8, 0.8, 0.8);
			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            

				v1 = (*it).real_position();
				int id1 = (*it).id();
				//if (rim->can_communicate_uni(*n, (*it))) psc->setcolor2(0, 255, 0); else psc->setcolor2(255, 0, 0);

				psc->procircle3(v1, 0.5, true );
	        
				if (id1 == n1.id()) psc->procircle3(v1, 10, false ); // TODO: aquire the actual communication range

				const shawn::Node& nb = *it;
		        for(shawn::Node::const_adjacency_iterator nit = nb.begin_adjacent_nodes(), nend = nb.end_adjacent_nodes(); nit!=nend; ++nit)
			    {
					v2 = (*nit).real_position();
					int id2 = (*nit).id();
					if (id1<id2) psc->proline2(v1, v2);
				}
			}


			psc->setcolor2(1.5, 0, 0);			
			v1 = n1.real_position();

			psc->procircle3(v1, 1.0, false );
			psc->setcolor2(0, 0, 0);			

			list<const Node*> l = getPath(sc, n1, n2);
			for( std::list<const shawn::Node*>::iterator pit = l.begin(), pend = l.end(); pit!=pend; ++pit) {
				v1 = (*pit)->real_position();
				psc->procircle3(v1, 0.5, true );
								ostringstream os;
				os << (*pit)->id();
				
				psc->text( v1+offset, os.str() );
				psc->mystroke();
				if (pit!=l.begin()) 
					psc->proline2(v1, v2);

				v2 = v1;
				
			}
			
			
			v2 = n2.real_position();
			psc->procircle3(v2, 1.0, false );
			
			/*			
			int i;
			for( shawn::World::const_node_iterator it1 = sc.world().begin_nodes(), end = sc.world().end_nodes(); it1 != end; ++it1) {

				i = it1->id();
				if ( (pred[i][nodeId] != UINT_INF)&&(!out_of_bounds(*it1))){
					int j = nodeId;
					while (pred[i][j] != i) {
						int tmp = pred[i][j];
						v1=sc.world().find_node_by_id(j)->real_position();
						v2=sc.world().find_node_by_id(tmp)->real_position();
						psc->proline2(v1, v2);
						j = tmp;
					}
					v1=it1->real_position();
					v2=sc.world().find_node_by_id(j)->real_position();
					psc->proline2(v1, v2);
				}
			}*/
			psc->text( Vec(0, 0), getIdString(sc) );
			psc->mystroke();

			psc->newpage();
		}


		// ----------------------------------------------------------------------
		std::list<const Node*> 
			MultihopDistanceEstimateTask::
			getPath( shawn::SimulationController& sc, const shawn::Node& n1, const shawn::Node& n2 ) {
				std::list< const Node*> nl;
				unsigned int sourceId = n1.id();
				unsigned int destId = n2.id();

				if ( pred[sourceId][destId] != UINT_INF) {
					int j = destId;
					while (pred[sourceId][j] != sourceId) {
						int tmp = pred[sourceId][j];
						nl.push_back(n1.world().find_node_by_id(j));
						j = tmp;
					}
					nl.push_back(n1.world().find_node_by_id(j));
					nl.push_back(n1.world().find_node_by_id(sourceId));
				}
				nl.reverse();
				return nl;
		}

		// ----------------------------------------------------------------------
			void
				MultihopDistanceEstimateTask::
				writeStats(std::string filename, shawn::SimulationController& sc)
			{

				delete(fhc_);
				double histogram_max = sc.environment().required_double_param("histogram_max");
				double histogram_min = sc.environment().required_double_param("histogram_min");
				double container_size = ( 1 + histogram_max - histogram_min ) / 200;

				DoubleHisto histo; //DoubleHisto((unsigned int) (max/container_size_+1));

				//double histogram_max = sc.environment().required_double_param("histogram_max");
				//			container_size_ = (histogram_max +20 )/ 200;

				histo = DoubleHisto(201);			
				histo.SetMinSpectrum(histogram_min-container_size/2);
				histo.SetMaxSpectrum(histogram_max+container_size/2);

				vector < double > vCenterPositions ( histo.GetCenterContainers() );
				std::string out_file_short;
				out_file_short="myhisto.txt";
				std::ofstream out_uni((char*)(out_file_short.c_str()));

				out_uni      << "            dist;";
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					out_uni  << DoubleFormatter(vCenterPositions[j]) << ";";
				}
				out_uni      << "         #dists;            avg;          median;         lower25;        upper25;        rel avg;      rel median;         lower25;        upper25" << endl;
				//Look for the highest key value
				int max2 = -1;
				for(DistMap::iterator it = dists.begin(), end = dists.end(); it!=end; ++it)
					if( it->first > max2 )
						max2 = it->first;
				for(int i = 0; i < max2; i++){

					out_uni  << DoubleFormatter(i) << ";";

					if( dists.find(i) == dists.end() ) 
					{/*
					 for(int j = 0; j < int(vCenterPositions.size()) + 5; j++) 	
					 {
					 out_uni  << DoubleFormatter(0) << ";";
					 }
					 out_uni  << DoubleFormatter(0) << ";" << endl;
					 continue;*/
						cout << "nix drin in " << i << endl;
						out_uni   << endl << flush;
					} else {

						multiset<float>* s = dists[i].getVals(i, "");
						int size = s->size();

						if (size==0) {
						cout << "0 drin in " << i << endl;
						out_uni   << endl << flush;
						} 
						else{
							histo = DoubleHisto(201);			
							histo.SetMinSpectrum(histogram_min-container_size/2);
							histo.SetMaxSpectrum(histogram_max+container_size/2);


							double lower25;
							double upper25;
							//dists[i].close();

							int cur = 0;
							double sum = 0;
							double mean;
							double median;

							for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
								histo.Update(*it);
								if( (int)floor(size*0.25)==(cur+1)) lower25=*it;
								if( (int)ceil(size*0.5)==(cur+1)) median=*it;
								if( (int)ceil(size*0.75)==(cur+1)) upper25=*it;
								cur++;
								sum+=(*it);
							}

							dists[i].clear();
							mean = sum / size;
							vector < double > hist ( histo.GetHistogramD() );
							for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
								double u;
								hist[j] = hist[j] / size;//dists[i].distCount;
								u = hist[j];
								if (!((u>=0.0)&&(u<=1.0))) u=0.0;
								out_uni  << DoubleFormatter(u) << ";";
							}

							histo.Clear();
							//double bias;

							//double std_dev;
							//double u, l, shit;


							//				GetMoments(vCenterPositions, hist, shit, bias, std_dev);
							out_uni  << DoubleFormatter(size) <<";"<< DoubleFormatter(mean)<<";"<< DoubleFormatter(median)//<<";"<< DoubleFormatter(std_dev)
								<<";"<< DoubleFormatter(lower25)<<";"<< DoubleFormatter(upper25) << endl;
							//					 <<";"<< DoubleFormatter((mean+0.5+i)/(i+0.5)) <<";"<< DoubleFormatter((median+0.5+i)/(i+0.5)) <<";"<< DoubleFormatter((lower25+0.5+i)/(i+0.5))<<";"<< DoubleFormatter((upper25+0.5+i)/(i+0.5)) << endl;
							out_uni.flush();

						}
					}
				}
				dists.clear();
				delete(fhc_);
			}
/*
		void
			MultihopDistanceEstimateTask::
			writeStats(std::string filename)
		{
			vector < double > vCenterPositions ( dists[0].err.GetCenterContainers() );
			std::string out_file_short;
			out_file_short="myhisto.txt";
			std::ofstream out_uni((char*)(out_file_short.c_str()));

			out_uni      << "            dist;";
			for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
				out_uni  << DoubleFormatter(vCenterPositions[j]) << ";";
			}
//			out_uni      << "         #dists;            avg;            bias;        std_dev;        lower25;        upper25" << endl;
			out_uni      << "         #dists;            avg;         lower25;        upper25" << endl;

			//Look for the highest key value
			int max = -1;
			for(DistMap::iterator it = dists.begin(), end = dists.end(); it!=end; ++it)
				if( it->first > max )
					max = it->first;

			for(int i = 0; i < max; i++){

						out_uni  << DoubleFormatter(i) << ";";

				if( dists.find(i) == dists.end() ) 
				{

					for(int j = 0; j < int(vCenterPositions.size()) + 5; j++) 	
					{
						out_uni  << DoubleFormatter(0) << ";" << endl;
					}
					continue;
				}

				vector < double > hist ( dists[i].err.GetHistogramD() );
				for(int j = 0; j < int(vCenterPositions.size()); j++) 	{
					double u;
					hist[j] = hist[j] / dists[i].distCount;
					u = hist[j];
					if (!((u>=0.0)&&(u<=1.0))) u=0.0;
					out_uni  << DoubleFormatter(u) << ";";
				}
				double bias;
				double mean;
				double std_dev;
				double lower25;
				double upper25;
				GetMoments(vCenterPositions, hist, mean, bias, std_dev, lower25, upper25);
				out_uni  << DoubleFormatter(dists[i].distCount) <<";"<< DoubleFormatter(mean) //<<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(std_dev)
					<<";"<< DoubleFormatter(lower25)<<";"<< DoubleFormatter(upper25) << endl;
				out_uni.flush();
			}
			dists.clear();
		}

*/
		// ----------------------------------------------------------------------
		void 
		MultihopDistanceEstimateTask::
		GetMoments(const std::vector<double>& vContainer, const std::vector<double>& vData,double& avg, double& bias, double& sdev) {
			avg=bias=sdev = 0;
			//double lowersum=0;
			//double uppersum=0;
			for(int j = 0; j < int(vContainer.size()); j++) {
				avg+=fabs(vData[j]*vContainer[j]);
				bias+=vData[j]*vContainer[j];
			}
/*

			j=0;
			while (	(lowersum+=vData[j])<0.25) j++;
			lower25=vContainer[j];
			j=vContainer.size()-1;
			while (	(uppersum+=vData[j])<0.25) j--;
			upper25=vContainer[j];
*/
			for(int j = 0; j < int(vContainer.size()); j++) {
				sdev+=(vContainer[j]-bias)*(vContainer[j]-bias)*vData[j]*vData[j];
			}
			sdev = sqrt(sdev);
		}



		// ----------------------------------------------------------------------
		std::string 
			MultihopDistanceEstimateTask::
			name( void )
			const throw()
		{
			return std::string("multihopdistest");
		}

			
		std::string 
			MultihopDistanceEstimateTask::
			getIdString( const shawn::SimulationController& sc )
			const throw()
		{
			int density = sc.environment().required_int_param("density");
			int iteration = sc.environment().required_int_param("iteration");
			int iterations = sc.environment().required_int_param("iterations");
			int node_count = sc.world().node_count();
			string estimate = sc.environment().required_string_param("distance_estimate");

			ostringstream os;
			os << "d=" << density << " i=" << iteration << "/" << iterations << " n=" << node_count << " e=" << estimate;
			return os.str();

		}

		// ----------------------------------------------------------------------
		IntMatrix 
			MultihopDistanceEstimateTask::
			getDistances( shawn::NodeDistanceEstimate& nde, shawn::SimulationController& sc  )
			throw()
		{


			require_world( sc );
			shawn::World& w = sc.world_w();
			
			int node_count = sc.world().node_count();
			dist.resize( node_count );
			pred.resize( node_count );
			//Floyd Warshall Test

			// Initialization
			for(int i = 0; i < node_count; ++i) {
				dist[i].resize(node_count);
				pred[i].resize(node_count);
				for(int j = 0; j < node_count; ++j) {
					pred[i][j] = UINT_INF;
					dist[i][j] = (i == j) ? 0 : UINT_INF;
				}
			}

			//Import graph --> Create adjacency matrix
			//double result;
			for( shawn::World::const_node_iterator it = w.begin_nodes(), end = w.end_nodes(); it != end; ++it)
			{
				const shawn::Node& nb = *it;
				for(shawn::Node::const_adjacency_iterator nit = nb.begin_adjacent_nodes(EdgeModel::CD_BIDI), nend = nb.end_adjacent_nodes(); nit!=nend; ++nit)
				{
//					if( nde.estimate_distance(nb, *nit, result ) ) {
//					if( perfect_de_->estimate_distance(nb, *nit, result ) && result < 5 ) {
						dist[nb.id()][nit->id()] = 1;
						pred[nb.id()][nit->id()] = nb.id();
//					}
				}
			}

			//Output the adjacency matrix
/*			ofstream adjfile("adjacency.csv", ios::trunc);
			for(int i = 0; i < node_count; ++i) {
				for(int j = 0; j < node_count; ++j) {
					adjfile <<  DoubleFormatter(dist[i][j]);
					if( j < node_count -1 )
						adjfile << ";";
					else
						adjfile << endl;
				}
			}
			adjfile.close();
*///			ofstream adjfile("adjacency.csv", ios::trunc);
			for(int i = 0; i < node_count; ++i) {
				for(int j = 0; j < node_count; ++j) {
					if ((dist[i][j]==1) && (dist[j][i]!= 1)) cout << "link from " << i << " to " << j << "is not bidi!" << endl;
				}
			}
			//adjfile.close();

			//Algorithm main loop
			for(int  k = 0; k < node_count; ++k) {
				for(int  i = 0; i < node_count; ++i) {
					for(int  j = 0; j < node_count; ++j) {
						if(dist[i][k] == UINT_INF || dist[k][j] == UINT_INF) 
							continue;
						if(dist[i][j] > dist[i][k] + dist[k][j]) {
							dist[i][j] = dist[i][k] + dist[k][j];
							pred[i][j] = pred[k][j];
						}
					}
				}
			}


			return dist;
		}


		// ----------------------------------------------------------------------
		void
			MultihopDistanceEstimateTask::
			run( shawn::SimulationController& sc )
			throw( std::runtime_error )
		{

			if (!initialized) init(sc);
			std::string	mode = sc.environment().required_string_param("mode");
			std::string	unit = sc.environment().required_string_param("unit");

			int hop_number = 0;
			double overall_distance = 0.0;
			double overall_error = 0.0;


			cout << "      Retrieving models...";
			shawn::NodeDistanceEstimate* de_;
			bool is_symetric;
			string distance_estimate = sc.environment().required_string_param("distance_estimate");
			if (distance_estimate == "perfect_estimate") {
				de_ = perfect_de_; 
				is_symetric = true;
			} else
			if (distance_estimate == "neighborhood_intersection_estimate") {
				is_symetric = false;
				shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );	
				if (nei.is_not_null()) {
					CommunicationModelFunction* cmf;
					 shawn::NeighborhoodIntersectionDistanceEstimate* nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();
					de_ = nptr;
					cmf = new RimModelFunction( *( dynamic_cast<shawn::RimCommModel*>(&(sc.world_w().communication_model_w()))), 0);
					nptr->setCommunicationModelFunction( cmf );
					cout << "de_ set to nides" << endl;
				} else
					cout << "ERROR: MultihopDistanceEstimateTask::run: Could not retrieve neighborhood intersection distance estimate." << endl;
			} else
				cout << "ERROR: MultihopDistanceEstimateTask::run: Unknown distance estimate: " << distance_estimate << endl;
			cout << "done." << endl<< "      Calculating Floyd-Warshall distances...";
			cout.flush();
			IntMatrix dist = getDistances( *de_, sc );
			//assert( false );
			cout << "done." << endl<< "      Comparing distances...";
			cout.flush();
			//int node_count = sc.world().node_count();

			double r = 10.0;
			int i, j;
			int firstnode=-1;
			int paintcount = 0;
			for( shawn::World::const_node_iterator it1 = sc.world().begin_nodes(), end = sc.world().end_nodes(); it1 != end; ++it1)
				for( shawn::World::const_node_iterator it2 = sc.world().begin_nodes(), end = sc.world().end_nodes(); it2 != end; ++it2){
					i = it1->id();
					j = it2->id();
					if (((i<j)||((!is_symetric)&&(i!=j)))&&(dist[i][j]!=FLOAT_INF)&&((!out_of_bounds(*it1))&&(!out_of_bounds(*it2)))){
						if (firstnode == -1) { 
							firstnode = i;
						}
						double result;
						if (PS)
						if (++paintcount < 50 ) {
							paint2( sc, psW, (*it1), (*it2) );
							cout << "n1=" << i << " n2=" << j << endl;
						}
						if ( mode == "los" ) {						
							perfect_de_->estimate_distance(*(it1), *(it2), result );
							int d = (int)ceil(dist[i][j]-result);
							if (d>max) max=d;
							d = (int)floor(result);
							
//								dists[d].err.insert(dist[i][j]/result);
								dists[d].set_file_handle_cache(fhc_);

								dists[d].insert(d, "", (float)(dist[i][j]-result));
						} else {
							double perfdistance = 0.0;
							double dedistance = 0.0;
							double perfresult;
							double deresult;
							bool ok = true;
							const Node *n1, *n2;
							list<const Node*> l = getPath(sc, (*it1), (*it2));
							cout << "dist[i,j]="<< dist[i][j] << " l.length= " << l.size() << endl;
							for( std::list<const shawn::Node*>::iterator lit = l.begin(), pend = l.end(); lit!=pend; ++lit) {
								n2 = (*lit);
								//if (!ok) cout << "ok = false" << endl;
								if ((lit!=l.begin())&&(ok)) {
									//bool de_dist_ok = de_->estimate_distance(*(n1), *(n2), deresult );
									//bool perf_dist_ok = perfect_de_->estimate_distance(*(n1), *(n2), perfresult );
									bool de_dist_ok = de_->estimate_distance(*(n1), *(n2), deresult );
									bool perf_dist_ok = perfect_de_->estimate_distance(*(n1), *(n2), perfresult );

//									cout << "from:"<< it1->id() << " to:" << it2->id() << " n1=" << n1->id()<<" n2="<< n2->id()<< " perf="<< perfresult << " dists["<<n1->id()<<"]["<<n2->id()<<"]="<<dist[n1->id()][n2->id()]<<endl<<flush;
									cout << "from:"<< it1->id() << " to:" << it2->id() << " n1=" << n1->id()<<" n2="<< n2->id()<< " perf="<< perfresult<<" de="<< deresult << " dists["<<n1->id()<<"]["<<n2->id()<<"]="<<dist[n1->id()][n2->id()]<<endl<<flush;
									if (!(de_dist_ok && perf_dist_ok)) {
										if (!de_dist_ok) cout << "could not measure neighborhood distance" << endl;
										if (!perf_dist_ok) cout << "could not measure perfect distance" << endl;
										ok = false;
										continue;
										lit=pend;
									} else {
//										assert(de_dist_ok);
//										assert(perf_dist_ok);
										dedistance += deresult;
										perfdistance += perfresult;
										overall_distance += perfresult;
										hop_number++;
										overall_error += (float)((dedistance-perfdistance)/r);
									}
								}// else cout << "-----" << endl;
								n1 = n2;
							}
							if (!ok) { 
								not_ok_counter++; 
								cout << "path could not be measured" << endl;
							} else {
								ok_counter++;
								int container;
								if (unit == "hops") container = l.size() -1; else container = (int)floor(perfdistance);
								//cout << "container=" << container << endl;
								dists[container].set_file_handle_cache(fhc_);
//								dists[container].insert(container, "", (float)(distance/dist[i][j]));
								dists[container].insert(container, "", (float)((dedistance-perfdistance)/r));
								//if (container<500) v[container]++;
								//if (container<5) cout << "container=" << container << endl;
//								cout << "container=" << container<< " distance/dist[i][j]: " << distance << "/" << dist[i][j] <<"="<<distance/dist[i][j] << endl;
								cout << "container=" << container<< " dedistance/perfdistance: " << dedistance << "/" << perfdistance <<"="<<dedistance/perfdistance << endl;
							}

						}
					}
				}
			cout << "done." << endl;
					
			std::string filename = sc.environment().required_string_param("file_name");

			if (filename != "") {
				cout << "      Writing statistics...";
				cout << "considered " << hop_number << " links with an average length of " << overall_distance / double(hop_number) << endl;
				cout << "mean error: " << overall_error / double(hop_number) << endl;
				//for ( int i = 0; i<500; i++ ) cout << "v["<<i<<"]="<<v[i]<<endl;
				writeStats( filename, sc );
				cout << "done." << endl;
				cout << "ok_counter=" << ok_counter << " not_ok_counter=" << not_ok_counter << " fraction= " << double(not_ok_counter)/double(not_ok_counter+ok_counter) << endl;
			}


/*
			ofstream distfile("distance.csv", ios::trunc);
			for(int i = 0; i < node_count; ++i)
			{
				for(int j = 0; j < node_count; ++j)
	            {
		            distfile <<  DoubleFormatter(dist[i][j]);
			        if( j < node_count -1 )
				        distfile << ";";
					else
						distfile << endl;
	            }
		    }
			distfile.close();
*/

//			std::string ps_file_name = sc.environment().required_string_param("ps_file_name");
			if (PS) {
				
	
				//for(int j = 0; j < node_count; ++j)
				cout << "      Writing postscript...";
				paint(sc, psW, firstnode);
				cout << "done." << endl;
			}

		}

		// ----------------------------------------------------------------------
		bool 
			MultihopDistanceEstimateTask::
			out_of_bounds(const shawn::Node& node)  
			throw()
		{

			if( node.real_position().x() > ur.x() || node.real_position().y() > ur.y() ||
				node.real_position().x() < ll.x() || node.real_position().y() < ll.y() )
				return true;

			return false;


		}
}
#endif

