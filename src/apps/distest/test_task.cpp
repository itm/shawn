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
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/node_distance_estimate.h"
#include "sys/misc/number_formatter.h"
#include "sys/misc/node_finder.h"
#include "apps/distest/neighbor_intersection_distance_estimate.h"
#include "apps/distest/test_task.h"
#include "apps/distest/histogram.h"
#include "apps/spyglass/spyglass.h"
#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/misc/box.h"


#include "sys/node.h"
#include "sys/edge_model.h"
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include "apps/distest/distest_postscript_writer.h"
#include "sys/comm_models/rim_comm_model.h"
#include "sys/communication_model.h"
#include "sys/comm_models/communication_model_keeper.h"
#include "sys/comm_models/communication_model_factory.h"
#include "sys/misc/random/basic_random.h"
#include <map>
#include "sys/util/set_count.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



#define DELETE(_td_) if((_td_) != NULL) delete (_td_);

using namespace shawn;
using namespace std;

//-----------------------


namespace distest
{ 
	
	typedef THistogram<double,double> DoubleHisto;


	// ----------------------------------------------------------------------
    TestTask::
        TestTask( void )
    {
        initialized = false;
		psc = NULL;
		sc_ = NULL;
		fhc_ = new FileHandleCache("multiset");
		density_histogram_ = std::vector<int>(100);
		for (int i = 0; i < 100; i++) density_histogram_[i]=0;



    }
	// ----------------------------------------------------------------------
    TestTask::
        ~TestTask( void )
    {
		delete psc;

    }
	// ----------------------------------------------------------------------
    void 
        TestTask::
        init(shawn::SimulationController& sc)

    {
   		sc_ = &sc;
		perfect = sc.distance_estimate_keeper().find( "perfect_estimate" );
		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();
		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;

		ofstream *psout_c = new ofstream("out.ps");
		psc= new DIPostscriptWriter(*psout_c, sc);
		initialized = true;
		for (int i = 0; i<200;i++) {
			could[i]=0;
			couldnot[i]=0;
		}
		simus = 0.0;
		multi_sim_avg_dens = 0.0;
		std::string out_file_short="twodfile.txt";
		twoDfile_ = new ofstream((char*)(out_file_short.c_str()));

		results = std::map<int, INTMatrix>();

			for(int nei = 0; nei < 50; ++nei) {
				results[nei].resize(200);
				for(int dist = 0; dist < 200; ++dist) {
					results[nei][dist].resize(100);
					for(int frac = 0; frac < 100; ++frac) {
						results[nei][dist][frac] = 0;
					}
				}
			}

		gp_file = std::vector<std::ofstream*>(50);
		for (int i=0;i<50; i++) {
			ostringstream os;
			os << "gnuplot/"<<i << "neighbors.txt";
			gp_file[i] = new std::ofstream( os.str().c_str() );
		}

		source_bigger=0;
		target_bigger=0;


    }


	// ----------------------------------------------------------------------
    std::string 
        TestTask::
        name( void )
        const throw()
    {
        return std::string("test");
    }

    // ----------------------------------------------------------------------
    std::string 
        TestTask::
        description( void )
        const throw()
    {
        return std::string("Task for testing this and that");
    }


	// ----------------------------------------------------------------------
    void
        TestTask::
		paint_rim(DIPostscriptWriter& psc, shawn::RimCommModel& rim, Node& n)
        throw( std::runtime_error )
    {
			std::vector<double> ki = rim.get_Ki(n.id());
			int granu = rim.get_granularity();
			double range = rim.get_range();

//			int offset = 0;
//			int rotation = se.optional_int_param("rotation", 0);
//			if (rotation > 0) offset = n.id() % 360;

			Vec v = n.real_position();



			psc.setcolor2(0.0, 0.0, 0.0);
			//psc.procircle3(v, 0.1, false );

			//psc.setcolor2(0.5, 0.5, 0.5);
			//psc.procircle3(v, 05.0, false );
			//psc.procircle3(v, 10.0, false );
			//psc.procircle3(v, 15.0, false );
			//psc.setcolor2(0.5, 0.5, 0.5);
			psc.setlinewidth2(0.1);
			//cout << ki.size() << endl;
			//cout << granu << endl;
/*			for ( int i = 0; i < ki.size(); i++) {

				double l1 = ki[i] * range;
				Vec p1( v.x() + cos(double(i*granu/180.0*M_PI))*l1, 
					    v.y() + sin(double(i*granu/180.0*M_PI))*l1, 
						0); 
				double l2 = ki[(i+1)%ki.size()] * range;
				Vec p2(v.x() + cos(double(((i+1)%ki.size())*granu/180.0*M_PI))*l2, 
					   v.y() + sin(double(((i+1)%ki.size())*granu/180.0*M_PI))*l2, 
					   0);
				psc.proline2(p1, p2);

			}*/
				//psc.mystroke();

				
			for ( int i = 0; i < 360; i++) {


				double l1 = rim.interpolate_Ki(i, n.id()) * range;
//				cout << i << ": " << l1 << endl;
				Vec p1( v.x() + cos(double(i/180.0*M_PI))*l1, 
					    v.y() + sin(double(i/180.0*M_PI))*l1, 
						0); 
				double l2 = rim.interpolate_Ki(i+1, n.id()) * range;
				Vec p2(v.x() + cos(double((i+1)/180.0*M_PI))*l2, 
					   v.y() + sin(double((i+1)/180.0*M_PI))*l2, 
					   0);
				psc.proline2(p1, p2);

				//cout << "i:" << i << " l1:" << l1 << " l2:" << l2 <<" v:(" << v.x() <<","<< v.y() << ") p1:(" << p1.x() <<","<< p1.y() << ") p2:(" << p2.x() <<","<< p2.y() << ") " << endl;
				//psc.mystroke();
			}
			psc.mystroke();

	}

		std::string 
			TestTask::
			getIdString( const shawn::SimulationController& sc )
			const throw()
		{
			int density = sc.environment().required_int_param("density");
			//int iteration = sc.environment().required_int_param("iteration");
			//int iterations = sc.environment().required_int_param("iterations");
			int node_count = sc.world().node_count();
			//string estimate = sc.environment().required_string_param("distance_estimate");

			ostringstream os;
//			os << "d=" << density << " i=" << iteration << "/" << iterations << " n=" << node_count << " e=" << estimate;
			os << "d=" << density << "  n=" << node_count;
			return os.str();

		}

	// ----------------------------------------------------------------------
    void
        TestTask::
		paint_nodes(DIPostscriptWriter& psc )
        throw( std::runtime_error )
    {
			Vec v;
			psc.setlinewidth2(0.1);
			psc.setcolor2(0.9, 0.9, 0.9);
			for( shawn::World::const_node_iterator it = sc_->world().begin_nodes(); it != sc_->world().end_nodes(); ++it) 
		    {            
				v = (*it).real_position();
				psc.procircle3(v, 0.1, true );
			}

	}

	void
        TestTask::
		paint_neighbors(DIPostscriptWriter& psc, Node& n, EdgeModel::CommunicationDirection d )
        throw( std::runtime_error )
    {
			Vec v;
			psc.setlinewidth2(0.1);
			psc.setcolor2(0.9, 0.9, 0.9);
			for(shawn::Node::const_adjacency_iterator itnb = n.begin_adjacent_nodes(d); itnb != n.end_adjacent_nodes(); ++itnb) 
		    {            
				v = (*itnb).real_position();
				psc.procircle3(v, 0.1, true );
			}

	}

	// ----------------------------------------------------------------------
    void
        TestTask::
		paint_scene(DIPostscriptWriter& psc, Node& n1, Node&n2 )
        throw( std::runtime_error )
    {
		Vec v1 = n1.real_position();
		Vec v2 = n2.real_position();

        shawn::CommunicationModel* com = &(sc_->world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
//

		paint_nodes( psc);
		
//		psc->procircle3(v, 10, false );

		paint_neighbors( psc, n1, EdgeModel::CD_IN );
		paint_neighbors( psc, n2, EdgeModel::CD_IN );

		paint_rim (psc, *rim, n1 );
		paint_rim (psc, *rim, n2 );

		
		/*			psc->setcolor2( 0.0,0.0,0.0 );
			for(shawn::Node::adjacency_iterator itnb = n->begin_adjacent_nodes_w(BIDI), end = n->end_adjacent_nodes_w(); itnb != end; ++itnb) 
			{
				cout << "treating neighbor " << itnb->id() << endl;
				Vec v = itnb->real_position();
				psc->procircle3(v, 0.1, true );
							psc->mystroke();
			}
			paint_rim( *psc, rim, v);
			cout << "nach rim Painting" << endl;
			psc->text( Vec(0, 0), getIdString(sc) );
			psc->mystroke();
			cout << getIdString(sc) << endl;*/
		psc.newpage();
//		delete psc;

	}


    void
        TestTask::
        paint_scenario( shawn::SimulationController& sc, Node& n1, Node& n2, double frac )
    {


			psc->setlinewidth2(0.1);
				
		shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
		
//		Node* n1 = nf.findByPosition(Vec(50, 50, 0));
//		Node* n2 = nf.findByPosition(Vec(55, 50, 0));


		Vec v =  Vec(15, 15, 0 );

		Box b = Box ( n1.real_position() - v, n1.real_position() + v);

			psc->setcolor2(0.5, 0.5, 0.5);

			//paint all nodes

			for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it) 
		    {            
				v = (*it).real_position();
				if (b.contains( v) ) psc->procircle3(v, 0.2, true );
			}

			// paint the two nodes	
			psc->setcolor2( 0.0,0.0,0.0 );
			v = n1.real_position();
			psc->procircle3(v, 0.4, false );
			v = n2.real_position();
			psc->procircle3(v, 0.4, false );


			bool c1, c2;

			for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it) 
		    {            
				c1 = com->can_communicate_uni(*it, n1);
				c2 = com->can_communicate_uni(*it, n2);
				v = (*it).real_position();
			psc->setcolor2( 0.0,0.0,0.0 );

				if (c1&&c2) psc->procircle3(v, 0.2, true ); else
					if (c1||c2) {
						psc->setcolor2( 1.0,1.0,1.0 );
						psc->procircle3(v, 0.2, true ); 
						psc->setcolor2( 0.0,0.0,0.0 );
						psc->procircle3(v, 0.2, false ); 
					}

			}

			paint_rim( *psc, *rim, n1);
			paint_rim( *psc, *rim, n2);

			psc->mystroke();


			ostringstream os;
			os << "distace=" << (n1.real_position()-n2.real_position()).euclidean_norm() << "  fraction=" << frac;

			psc->text( Vec(0, 0), os.str() );
			psc->mystroke();
		psc->newpage();




			psc->setlinewidth2(0.1);
}

//--------------------------------------------------------------------------------
//Calculate the Connection Probability Distribution of the currently set Communication Model
//--------------------------------------------------------------------------------

    void
        TestTask::
        calc_communication_probability_function( shawn::SimulationController& sc )
        throw( std::runtime_error )

	{
	
		shawn::PerfectDistanceEstimate* perfect_de_;
		//cout << ContainerSize << endl;
		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "perfect_estimate" );	
		if (nei.is_not_null()) 
			perfect_de_ = (shawn::PerfectDistanceEstimate*)nei.get();
		else
			cout << "ERROR: TestTask::init: Could not retrieve perfect distance estimate." << endl;
		
		double r = sc.world().size_hint();
		double result;

		cout << "r=" << r << endl;
		shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		//shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);

		//CommunicationModel * udg = sc.communication_model_keeper_w().find_w("disk_graph")->create(sc);
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

			double frac[200];
			std::string filename = sc.environment().required_string_param("file_name");
			if ( filename != "") {
				std::string out_file_short;
				out_file_short=filename;
				std::ofstream out((char*)(out_file_short.c_str()));


				for (int i = 0; i<200;i++) {
					if (could[i]+couldnot[i]>0)
						frac[i] = (double)(could[i])/(double)(could[i]+couldnot[i]);
					else
						frac[i]=0.0;
					out << DoubleFormatter((double)(i)/100.0) << "; " <<DoubleFormatter(frac[i]) << ";" <<could[i]+couldnot[i] <<  endl;
				}
				out.close();
				for (int i = 0; i<200;i++) {
					could[i]=0;
					couldnot[i]=0;
				}

			}
		}



	// ----------------------------------------------------------------------
    void
        TestTask::
        run( shawn::SimulationController& sc )
        throw( std::runtime_error )
    {
				if (!initialized) init(sc);
	        calc_communication_probability_function( sc );


		
//--------------------------------------------------------------------------------
//  Writing gnuplot files of communication model functions
//--------------------------------------------------------------------------------

/*				
		LinearCommunicationModelFunction* lcmf = new LinearCommunicationModelFunction( 0.25 );
		lcmf->gnuplot3D( "gnuplot3D.txt", false, 0.0 );
		lcmf->gnuplot2D( "gnuplot2D.txt" );

*/
/*
		shawn::NeighborhoodIntersectionDistanceEstimate* nptr;

		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();
		}

		shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(&(sc.world_w().communication_model_w()));

//		double step = 0.1;
		nptr->setCommunicationModelFunction( new RimModelFunction( *( dynamic_cast<shawn::RimCommModel*>(&(sc.world_w().communication_model_w()))), 1 ));
		vector<double> v = nptr->getEstimationTable();
*/
		/*
		std::string out_file_short="interpolationtest.txt";
		std::ofstream out((char*)(out_file_short.c_str()));
		for (int i = 0; i<v.size(); i++) {
			out << DoubleFormatter(i*step) << "; " <<DoubleFormatter(v[i])<< "; " <<DoubleFormatter(v[i]) <<  endl;
			if (i+1<v.size()) out << DoubleFormatter((i+0.5)*step) << ";; " <<DoubleFormatter((v[i]+v[i+1])/2) <<  endl;
		}
		out.close();
*/

//--------------------------------------------------------------------------------
//  Writing a 2D file plotting occurences in an fraction vs distance diagramm
//--------------------------------------------------------------------------------
/*
		const World& w = sc.world();		
		int nodeID1 = 0;
		int nodeID2 = 0;
		double r = 10.0;
		double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
		shawn::Vec rr(r, r, 0);
		shawn::Vec ll = w.lower_left() + rr;
		shawn::Vec ur = w.upper_right() - rr;
		bool exclude_border = true;
						bool perfdistok = false, unidistok = false, bidiavgdistok = false, bidimindistok = false, bidimaxdistok = false;

		shawn::ConstNodeDistanceEstimateHandle perfect;
				perfect = sc.distance_estimate_keeper().find( "perfect_estimate" );

		shawn::NeighborhoodIntersectionDistanceEstimate* nptr;

		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();
		}

		//nptr->setCommunicationModelFunction(new LinearCommunicationModelFunction( 0.25 ));

			for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(), endit = sc.world_w().end_nodes_w();it != endit; ++it) 
			{            

				nodeID1 = (*it).id();


				if( out_of_bounds(ll, ur, *it ) && exclude_border) //TODO: Zum initialen Testen muss nachher raus
					continue;


				for(shawn::Node::adjacency_iterator itnb = (*it).begin_adjacent_nodes_w(IN), end = (*it).end_adjacent_nodes_w(); itnb != end; ++itnb) 
				{
					nodeID2 = (*itnb).id();

					if( out_of_bounds(ll, ur, *itnb )&& exclude_border ) //TODO: Zum initialen Testen muss nachher raus
						continue;
					double total, shared;

					perfdistok = perfect->estimate_distance( (*it), *itnb, perfdist );
					nptr->set_mode(M_BIDI_AVG);
					bidiavgdistok = nptr->getFraction( (*it), *itnb, shared, total );

					if(nodeID1 == nodeID2)
					{
						continue;
					}
					else
					{
						if (bidiavgdistok){
							double y = perfdist / 10.0;
							double x = shared/total;

							if ((x > 0.97)) {
								paint_scenario( sc, *it, *itnb, x ); 
								cout << "Writing to Ps-File..." << endl << flush;
							}

							(*twoDfile_) << x << "\t" << y << endl;

//							if ((x > 0.99) && (y > 0.8))cout << "x:" << x << " y:" << y << " id1:" << nodeID1 << " id2:" <<  nodeID2 << " shared:" << shared << " total:" << total << endl;
						}
					}
				}

			}
			cout << endl << flush;
			twoDfile_->flush();
			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") twoDfile_->close();


*/
//--------------------------------------------------------------------------------
//  Writing a 3D file plotting occurences in an fraction vs distance
//--------------------------------------------------------------------------------
/*
		const World& w = sc.world();		
		int nodeID1 = 0;
		int nodeID2 = 0;
		double r = 10.0;



		double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
		shawn::Vec rr(r, r, 0);
		shawn::Vec ll = w.lower_left() + rr;
		shawn::Vec ur = w.upper_right() - rr;
		bool exclude_border = true;
						bool perfdistok = false, unidistok = false, bidiavgdistok = false, bidimindistok = false, bidimaxdistok = false;

		shawn::ConstNodeDistanceEstimateHandle perfect;
				perfect = sc.distance_estimate_keeper().find( "perfect_estimate" );

		shawn::NeighborhoodIntersectionDistanceEstimate* nptr;

		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();
		}

		//nptr->setCommunicationModelFunction(new LinearCommunicationModelFunction( 0.25 ));

			for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(), endit = sc.world_w().end_nodes_w();it != endit; ++it) 
			{            

				nodeID1 = (*it).id();


				if( out_of_bounds(ll, ur, *it ) && exclude_border) //TODO: Zum initialen Testen muss nachher raus
					continue;
				int nei = (*it).degree(CD_IN);

				for(shawn::Node::adjacency_iterator itnb = (*it).begin_adjacent_nodes_w(CD_IN), end = (*it).end_adjacent_nodes_w(); itnb != end; ++itnb) 
				{
					nodeID2 = (*itnb).id();

					if( out_of_bounds(ll, ur, *itnb )&& exclude_border ) //TODO: Zum initialen Testen muss nachher raus
						continue;

					double offset;
					//if (nodeID1 > nodeID2) offset = 0.01; else offset=-0.01;
					double source, target, shared;

					perfdistok = perfect->estimate_distance( (*it), *itnb, perfdist );
					bidiavgdistok = nptr->getFraction( (*it), *itnb, shared, source, target );

					if(nodeID1 == nodeID2)
					{
						continue;
					}
					else
					{
						if (bidiavgdistok){

							int dist = int(perfdist *100 / 10.0);
							int frac =  int((shared/((source+target)/2.0))*100);
//							int frac =  int((shared/source)*100);

							offset = (source-target)/100;

							results[nei][dist][frac]++;
							nei = int((source+target)/2.0);

//							*(gp_file[nei]) << shared/((source+target)/2.0) << "\t" << perfdist / 10.0 << endl;
							*(gp_file[nei]) << shared/source << "\t" << perfdist / 10.0 - offset << "\t" << (shared/((source+target)/2.0) ) -(shared/source) << "\t" << offset << endl;
//							*(gp_file[nei]) << shared/source << "\t" << perfdist / 10.0 << endl;

//							if (dist!=0 && dist>50 && double(frac)/double(dist) > 0.75) {
							//if ( dist > 250 - 2.5 * frac ) {
							//	double r_source, r_dest, r_avg;
							//	if (source > target) source_bigger++;
							//	if (source < target) target_bigger++;
							//	nptr->set_mode(M_UNI);
							//	nptr->estimate_distance( (*it), *itnb, r_source );
							//	nptr->estimate_distance( *itnb, (*it), r_dest );
							//	nptr->set_mode(M_BIDI_AVG);
							//	nptr->estimate_distance( (*it), *itnb, r_avg );
							//	cout << "critt="<< 250 - 2.5 * double(frac)  <<" dist=" << perfdist << " shared=" << shared << " avg-est=" << r_avg << " n_s=" << source << " est_s=" << r_source<< " n_t=" << target << " est_t=" << r_dest << endl;
							//}


//							if ((x > 0.99) && (y > 0.8))cout << "x:" << x << " y:" << y << " id1:" << nodeID1 << " id2:" <<  nodeID2 << " shared:" << shared << " total:" << total << endl;
						}
					}
				}

			}
			cout << endl << flush;
			twoDfile_->flush();
			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") {
				
				cout << "source_bigger=" << source_bigger << " target_bigger=" << target_bigger << endl;
				
				twoDfile_->close();
				for (int i = 0; i<50; i++) (gp_file[i])->close();

		
				std::ofstream* f;		
				for (int n=0;n<50; n++) {
					ostringstream os;
					os << "gnuplot/"<<n << "neighbors3d.txt";

					f = new std::ofstream( os.str().c_str() );
					for (int fr=0;fr<100;fr++) {
						for (int d=0; d<100;d++) {
							(*f) << double(fr)/100 << "\t" << double(d)/100 << "\t" << results[n][d][fr] <<endl;
						}
						(*f) <<endl;

					}
					f->close();
					delete f;
				}
			}
			//for (int i = 0; i<50;i++) (gp_file[i])->close();


*/
//--------------------------------------------------------------------------------
//  Testing the loading of xml files
//--------------------------------------------------------------------------------
/*		const World& w = sc.world();		
		for (int i=0;i<10; i++) {
			const Node& n =  *(w.find_node_by_id(i));
			Vec v = n.real_position();
			cout << "node " << i << " " << v << endl;
		}
		const Node& n =  *(w.find_node_by_id(w.node_count()-1));
		Vec v = n.real_position();
		cout << "node " << w.node_count()-1 << " " << v << endl;
*/		

//--------------------------------------------------------------------------------
//  Simulating 2 random circles, calculating their centers' distance by counting the nodes in their intersection
//--------------------------------------------------------------------------------
/*

		const World& w = sc.world();		
	   shawn::NeighborhoodIntersectionDistanceEstimate* nptr;
		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();

		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;



		double x = w.upper_right().x() - 20;
		double y = w.upper_right().y() - 20;

		Vec v1, v2;
		NodeFinder nf(&(sc.world_w()));
		double realdist;

		for (int i = 1; i < 10000;i++) {
			bool ok = false;
			if ((i % 1000)==0) cout << i << endl;
			while (!ok) {
				v1 = Vec( 10 + uniform_random_0i_1i() * x, 10+uniform_random_0i_1i() * y, 0);
				v2 = Vec( 10 + uniform_random_0i_1i() * x, 10+uniform_random_0i_1i() * y, 0);
				realdist = (v1-v2).euclidean_norm();
				ok = (realdist)<=10;
			}
			NodeSet ns1 = nf.findInCircle(v1, 10); 
			NodeSet ns2 = nf.findInCircle(v2, 10); 
			int intersection;
			count_set_differences( ns1, ns2, NULL, NULL, NULL, NULL, &intersection, NULL);
			int total = ns1.size();

			double unidist = nptr->calcEstimate(double(intersection)/double(total), 10.0);


			int container =  (int)floor(realdist *10.0);
			uniErr[container].set_file_handle_cache(fhc_);
			uniErr[container].insert(container, "uni",(unidist-realdist)/10.0);
		}




			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") {

		cout << "Writing Stats" << endl;
		DoubleHisto histo;
		histo = DoubleHisto(101);			
		histo.SetMinSpectrum(-1.01);
		histo.SetMaxSpectrum(1.01);

		cout << "Writing file ...";
			
		vector < double > vCenterPositions ( histo.GetCenterContainers() );
		std::string out_file_short;
		out_file_short="strange_circle_test.uni";
		std::ofstream out_uni((char*)(out_file_short.c_str()));
		cout << "opening files" << endl;
			
		out_uni      << "          #crit;";
		for(int j = 0; j < vCenterPositions.size(); j++) 	{
			out_uni  << DoubleFormatter(vCenterPositions[j]) << ";";
		}
		out_uni      << "            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;

		//Look for the highest key value
		int max = -1;
		for(vcMap::iterator it = uniErr.begin(), end = uniErr.end(); it!=end; ++it)
			if( it->first > max )
				max = it->first;

		cout << "max="<< max << endl;
		cout << "Before outer for" << endl;
		for(int i = 0; i <= max; i++){
			cout << "i="<< i << endl;

			out_uni  << DoubleFormatter(i) << ";";

			if( uniErr.find(i) == uniErr.end() ) {
				cout << "No entry for "<<i<<" in uniErr" << endl << flush;
				out_uni   << endl << flush;
				continue;
			}

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
			cout << "uni i=" << i << endl << flush;
			histo = DoubleHisto(101);			
			histo.SetMinSpectrum(-1.01);
			histo.SetMaxSpectrum(1.01);
			//uniErr[i].close();                  //-------------------------
			cout << "after uniErr["<<i<<"].close" << endl << flush;
			s = uniErr[i].getVals(i, "uni");             //-----------------
			assert( s!=NULL);
			cout << "after getting the values" << endl << flush;
			size = s->size();

			cout << "after uniErr["<<i<<"].getVals size="<<size << endl << flush;

			cur = 0;
			sum = abssum = 0.0;
			for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
				if ( cur == 0 ) min = *it;
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
			cout << "calculated uni data" << endl << flush;
			hist = vector< double >( histo.GetHistogramD() );
			for(int j = 0; j < vCenterPositions.size(); j++) 	{
				double u;
				hist[j] = hist[j] / size;//dists[i].distCount;
				u = hist[j];
				if (!((u>=0.0)&&(u<=1.0))) u=0.0;
				out_uni  << DoubleFormatter(u) << ";";   //-------------------------
			}

			out_uni  << DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
			//------------------------------
			histo.Clear();	
		}
		out_uni.close();//----------------
		delete(fhc_);
		cout << "done" << endl << flush;


}




*/



//--------------------------------------------------------------------------------
//  Simulating 2 random circles, calculating their centers' distance by counting the nodes in their intersection
//--------------------------------------------------------------------------------
/*

		const World& w = sc.world();		
	   shawn::NeighborhoodIntersectionDistanceEstimate* nptr;
		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();

		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;



		double x = w.upper_right().x() - 20;
		double y = w.upper_right().y() - 20;

		Vec v1, v2;
		NodeFinder nf(&(sc.world_w()));
		double realdist;

		for (int i = 1; i < 10000;i++) {
			bool ok = false;
			if ((i % 1000)==0) cout << i << endl;
			while (!ok) {
				v1 = Vec( 10 + uniform_random_0i_1i() * x, 10+uniform_random_0i_1i() * y, 0);
				v2 = Vec( 10 + uniform_random_0i_1i() * x, 10+uniform_random_0i_1i() * y, 0);
				realdist = (v1-v2).euclidean_norm();
				ok = (realdist)<=10;
			}
			NodeSet ns1 = nf.findInCircle(v1, 10); 
			NodeSet ns2 = nf.findInCircle(v2, 10); 
			int intersection;
			count_set_differences( ns1, ns2, NULL, NULL, NULL, NULL, &intersection, NULL);
			int total = ns1.size();

			double unidist = nptr->calcEstimate(double(intersection)/double(total), 10.0);


			int container =  (int)floor(realdist *10.0);
			uniErr[container].set_file_handle_cache(fhc_);
			uniErr[container].insert(container, "uni",(unidist-realdist)/10.0);
		}




			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") {

		cout << "Writing Stats" << endl;
		DoubleHisto histo;
		histo = DoubleHisto(101);			
		histo.SetMinSpectrum(-1.01);
		histo.SetMaxSpectrum(1.01);

		cout << "Writing file ...";
			
		vector < double > vCenterPositions ( histo.GetCenterContainers() );
		std::string out_file_short;
		out_file_short="strange_circle_test.uni";
		std::ofstream out_uni((char*)(out_file_short.c_str()));
		cout << "opening files" << endl;
			
		out_uni      << "          #crit;";
		for(int j = 0; j < vCenterPositions.size(); j++) 	{
			out_uni  << DoubleFormatter(vCenterPositions[j]) << ";";
		}
		out_uni      << "            avg;            bias;         median;        lower25;     upper25;        lower95;     upper95;            min;         max;          edges;cntrl" << endl;

		//Look for the highest key value
		int max = -1;
		for(vcMap::iterator it = uniErr.begin(), end = uniErr.end(); it!=end; ++it)
			if( it->first > max )
				max = it->first;

		cout << "max="<< max << endl;
		cout << "Before outer for" << endl;
		for(int i = 0; i <= max; i++){
			cout << "i="<< i << endl;

			out_uni  << DoubleFormatter(i) << ";";

			if( uniErr.find(i) == uniErr.end() ) {
				cout << "No entry for "<<i<<" in uniErr" << endl << flush;
				out_uni   << endl << flush;
				continue;
			}

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
			cout << "uni i=" << i << endl << flush;
			histo = DoubleHisto(101);			
			histo.SetMinSpectrum(-1.01);
			histo.SetMaxSpectrum(1.01);
			//uniErr[i].close();                  //-------------------------
			cout << "after uniErr["<<i<<"].close" << endl << flush;
			s = uniErr[i].getVals(i, "uni");             //-----------------
			assert( s!=NULL);
			cout << "after getting the values" << endl << flush;
			size = s->size();

			cout << "after uniErr["<<i<<"].getVals size="<<size << endl << flush;

			cur = 0;
			sum = abssum = 0.0;
			for ( multiset<float>::iterator it = s->begin(), end = s->end(); it != end; ++it) {
				if ( cur == 0 ) min = *it;
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
			cout << "calculated uni data" << endl << flush;
			hist = vector< double >( histo.GetHistogramD() );
			for(int j = 0; j < vCenterPositions.size(); j++) 	{
				double u;
				hist[j] = hist[j] / size;//dists[i].distCount;
				u = hist[j];
				if (!((u>=0.0)&&(u<=1.0))) u=0.0;
				out_uni  << DoubleFormatter(u) << ";";   //-------------------------
			}

			out_uni  << DoubleFormatter(mean) <<";"<< DoubleFormatter(bias)<<";"<< DoubleFormatter(median) <<";"<< DoubleFormatter(lower25) <<";"<< DoubleFormatter(upper25) <<";"<< DoubleFormatter(lower95) <<";"<< DoubleFormatter(upper95) <<";"<< DoubleFormatter(min) <<";"<< DoubleFormatter(max) <<";"<< size << endl << flush;
			//------------------------------
			histo.Clear();	
		}
		out_uni.close();//----------------
		delete(fhc_);
		cout << "done" << endl << flush;


}




*/


//--------------------------------------------------------------------------------
//calculating the density in the center
//--------------------------------------------------------------------------------
/*

		NodeFinder nf(&(sc.world_w()));
			Node* n = nf.findByPosition(Vec(114, 114, 0));

			int neighborCount;
						unsigned long int densities = 0;
			int min_dens = 999999;
			int max_dens = 0;
			float all_count = 0.0;

const World& w = sc.world();		
		int nodeID1 = 0;
		int nodeID2 = 0;
		double r = 10.0;
		double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
		shawn::Vec rr(r, r, 0);
		shawn::Vec ll = w.lower_left() + rr;
		shawn::Vec ur = w.upper_right() - rr;

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes();it != endit; ++it) {
				neighborCount = (*it).degree( CD_IN );
				density_histogram_[neighborCount]++;
				//all_count+= 1.0;
				//densities += neighborCount;
				//if (neighborCount > max_dens) max_dens = neighborCount;
				//if (neighborCount < min_dens) min_dens = neighborCount;

			}

			neighborCount = (*n).degree( CD_IN );
			all_count+= 1.0;
			multi_sim_avg_dens += neighborCount;
			if (neighborCount > max_dens) max_dens = neighborCount;
			if (neighborCount < min_dens) min_dens = neighborCount;


			double avg_dens = (double)(densities / all_count);
			cout << "DENSITY: average=" << avg_dens << " min=" << min_dens << " max=" << max_dens << endl;
			multi_sim_avg_dens +=avg_dens;
			simus+=1.0;

			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") {
				int count = 0;
				for (int i=0; i<100; i++) count += density_histogram_[i];
				std::string out_file_short;
				out_file_short= "density_histogram.txt";
				std::ofstream out((char*)(out_file_short.c_str()));
				for (int i=0; i<100; i++) {
					out << i << "\t" << double(density_histogram_[i])/double(count) << endl;
				}
				out.close();
				



				cout << "-------------------------------------------------------------------------------------" << endl;
				cout << "max=" << max_dens;
				cout << " min=" << min_dens;
				cout << " avg=" << double(multi_sim_avg_dens / simus);
				cout << endl;
				cout << "-------------------------------------------------------------------------------------" << endl;
			}

*/

//--------------------------------------------------------------------------------
//calculating average densities
//--------------------------------------------------------------------------------
/*		

			int neighborCount;
						unsigned long int densities = 0;
			int min_dens = 999999;
			int max_dens = 0;
			float all_count = 0.0;

const World& w = sc.world();		
		int nodeID1 = 0;
		int nodeID2 = 0;
		double r = 10.0;
		double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
		shawn::Vec rr(r, r, 0);
		shawn::Vec ll = w.lower_left() + rr;
		shawn::Vec ur = w.upper_right() - rr;

        shawn::CommunicationModel* com = &(sc_->world_w().communication_model_w());
		
	for( shawn::World::const_node_iterator it1 = sc.world().begin_nodes(), endit1 = sc.world().end_nodes();it1 != endit1; ++it1) 
		for( shawn::World::const_node_iterator it2 = sc.world().begin_nodes(), endit2 = sc.world().end_nodes();it2 != endit2; ++it2) {
			if (com->can_communicate_uni(*(it1), *it2)) cout << it1->label() << " and " << it2->label() << " can uni." << endl;
			if (com->can_communicate_bidi(*(it1), *it2)) cout << it1->label() << " and " << it2->label() << " can bidi." << endl;
		
		}

		
		for( shawn::World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes();it != endit; ++it) {
		cout << "node " << it->label()<<": "<< endl;
		for(shawn::Node::const_adjacency_iterator itnb = it->begin_adjacent_nodes(EdgeModel::CD_BIDI), end = it->end_adjacent_nodes(); itnb != end; ++itnb) {
			cout << "BIDI node " << itnb->label()  << endl;
		}
		for(shawn::Node::const_adjacency_iterator itnb = it->begin_adjacent_nodes(EdgeModel::CD_OUT), end = it->end_adjacent_nodes(); itnb != end; ++itnb) {
			cout << "OUT node " << itnb->label()  << endl;

		}
		for(shawn::Node::const_adjacency_iterator itnb = it->begin_adjacent_nodes(EdgeModel::CD_IN), end = it->end_adjacent_nodes(); itnb != end; ++itnb) {
			cout << "IN node " << itnb->label()  << endl;

		}
		for(shawn::Node::const_adjacency_iterator itnb = it->begin_adjacent_nodes(EdgeModel::CD_ANY), end = it->end_adjacent_nodes(); itnb != end; ++itnb) {
			cout << "ANY node " << itnb->label()  << endl;

		}
		cout << "------------------------------------------"<< endl;
	}
*/
//--------------------------------------------------------------------------------
//calculating average densities
//--------------------------------------------------------------------------------
/*		

			int neighborCount;
						unsigned long int densities = 0;
			int min_dens = 999999;
			int max_dens = 0;
			float all_count = 0.0;

const World& w = sc.world();		
		int nodeID1 = 0;
		int nodeID2 = 0;
		double r = 10.0;
		double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
		shawn::Vec rr(r, r, 0);
		shawn::Vec ll = w.lower_left() + rr;
		shawn::Vec ur = w.upper_right() - rr;

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes();it != endit; ++it) {
				if( out_of_bounds(ll, ur, *it )) //TODO: Zum initialen Testen muss nachher raus
					continue;


				neighborCount = (*it).degree( IN );
				all_count+= 1.0;
				densities += neighborCount;
				density_histogram_[neighborCount]++;
				if (neighborCount > max_dens) max_dens = neighborCount;
				if (neighborCount < min_dens) min_dens = neighborCount;

			}

			double avg_dens = (double)(densities / all_count);
			cout << "DENSITY: average=" << avg_dens << " min=" << min_dens << " max=" << max_dens << endl;
			multi_sim_avg_dens +=avg_dens;
			simus+=1.0;

			std::string filename = sc.environment().required_string_param("file_name");
			if (filename != "") {
				int count = 0;
				for (int i=0; i<100; i++) count += density_histogram_[i];
				std::string out_file_short;
				out_file_short= "density_histogram.txt";
				std::ofstream out((char*)(out_file_short.c_str()));
				for (int i=0; i<100; i++) {
					out << i << "\t" << double(density_histogram_[i])/double(count) << endl;
				}
				out.close();
				



				cout << "-------------------------------------------------------------------------------------" << endl;
				cout << "max=" << max_dens;
				cout << " min=" << min_dens;
				cout << " avg=" << double(multi_sim_avg_dens / simus);
				cout << endl;
				cout << "-------------------------------------------------------------------------------------" << endl;
			}
*/



		/*
//--------------------------------------------------------------------------------
//  How to write Spyglass files
//--------------------------------------------------------------------------------
		Spyglass *sg = Spyglass::get_spyglass();
		if ( sg != NULL)
		{
			StringPacket *sp = new StringPacket();
			Vec pos = owner().real_position();
			sp->set_parameters(0,STRINGPACKETTYPE,owner().id(),owner().current_time(),
				float(pos.x()),float(pos.y()),float(pos.z()));
			ostringstream os;
			os << owner().id() << " says hello. ";
			sp->set_string(os.str());
			sg->sendSpyglassPacket(sp);
			delete sp;
		}*/

				
				
				

//--------------------------------------------------------------------------------
// Show 3 d function of enhance NIDES
//--------------------------------------------------------------------------------
/*				
		shawn::NeighborhoodIntersectionDistanceEstimate* nptr;

		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();

		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;

			std::string out_file_short;
			out_file_short= "3dfunc.txt";
			std::ofstream out((char*)(out_file_short.c_str()));
			vector<double> v = nptr->calculateEstimationTable();
			for (int i = 0; i<v.size(); i++) {
				out << i << ";" << DoubleFormatter(v[i]) << endl;
				cout << i << ";" << v[i] << endl;}
			out.close();
		
*/

//--------------------------------------------------------------------------------
// Show a 3 d function of the RIM model
//--------------------------------------------------------------------------------

/*				
        shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);

			std::string out_file_short;
			out_file_short= "rim_3d_model.txt";
			std::ofstream out((char*)(out_file_short.c_str()));

		Vec zero = Vec (0, 0, 0);
		Vec v1;


		double step = 0.04;
		double range = 1.5;
		
		for (double x1 = (-range); x1 < range; x1 += step ) {
			for (double y1 = (-range); y1 < range; y1 += step ){
				v1 = Vec(x1, y1, 0 );
				if (rim->can_communicate_uni( zero, 0, v1 )) {
					out << x1 << "\t" << y1 << "\t" << 1.0 << endl;
				} else
					out << x1 << "\t" << y1 << "\t" << 0.0 << endl;
			} 
			out << endl;
		}
		out.close();

*/
			
/*
//--------------------------------------------------------------------------------
//Calculate single NIDES values for different shared neighbor fractions
//--------------------------------------------------------------------------------
				
		shawn::NeighborhoodIntersectionDistanceEstimate* nptr;

		shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "neighborhood_intersection_estimate" );
		if (nei.is_not_null()) {
			nptr = (shawn::NeighborhoodIntersectionDistanceEstimate*)nei.get();

		} else cout << "DistanceEstimateTask.int: Could not find NeighborhoodIntersectionDI!" << endl;


               // nptr->set_mode(M_UNI);
				double d1, d2, d3;

				d1 = nptr->calcEstimate( 5.0/15.0, 10.0 );
				cout << "n=15, frac=5 --> d=" << d1 << endl;

				d2 = nptr->calcEstimate( 6.0/15.0, 10.0 );
				cout << "n=15, frac=6 --> d=" << d2 << endl;

				d3 = nptr->calcEstimate( 7.0/15.0, 10.0 );
				cout << "n=15, frac=7 --> d=" << d3 << endl;

				cout << "d1-d2=" << d1-d2 << " d2-d3=" << d2-d3 << endl;

*/
 
//--------------------------------------------------------------------------------
//Calculate the Connection Probability Distribution of the currently set Communication Model
//--------------------------------------------------------------------------------

/*
shawn::PerfectDistanceEstimate* perfect_de_;
			//cout << ContainerSize << endl;
			shawn::NodeDistanceEstimateHandle nei = sc.distance_estimate_keeper_w().find_w( "perfect_estimate" );	
			if (nei.is_not_null()) 
				 perfect_de_ = (shawn::PerfectDistanceEstimate*)nei.get();
			else
				cout << "ERROR: TestTask::init: Could not retrieve perfect distance estimate." << endl;
			double r = sc.world().size_hint();
		double result;



        shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		//shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);

		//CommunicationModel * udg = sc.communication_model_keeper_w().find_w("disk_graph")->create(sc);

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

		double frac[200];
		std::string filename = sc.environment().required_string_param("file_name");
		if ( filename != "") {
			std::string out_file_short;
			out_file_short=filename;
			std::ofstream out((char*)(out_file_short.c_str()));


			for (int i = 0; i<200;i++) {
				if (could[i]+couldnot[i]>0)
					frac[i] = (double)(could[i])/(double)(could[i]+couldnot[i]);
				else
					frac[i]=0.0;
				out << DoubleFormatter((double)(i)/100.0) << "; " <<DoubleFormatter(frac[i]) << ";" <<could[i]+couldnot[i] <<  endl;
			}
			out.close();
			for (int i = 0; i<200;i++) {
				could[i]=0;
				couldnot[i]=0;
			}

		}
*/

//--------------------------------------------------------------------------------
//Check rotation
//--------------------------------------------------------------------------------
/*
		NodeFinder nf(&(sc.world_w()));
        shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
			
//			Node* n = nf.findByPosition(Vec(50, 50, 0));
			Node* n = nf.findById(540);

			// Paint all nodes;
			psc->setlinewidth2(0.1);
			psc->setcolor2(0.9, 0.9, 0.9);
			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				psc->procircle3(v2, 0.1, true );
			}


			//Paint node circles and RIM
			Vec v = n->real_position();
			paint_rim( *psc, rim, v);
			psc->procircle3(v, 10, false );
			psc->procircle3(v, 15, false );
			psc->procircle3(v, 5, false );

			// Paint nodes that can hear n
			psc->setcolor2( 0.0,0.0,0.0 );
			for(shawn::Node::adjacency_iterator itnb = n->begin_adjacent_nodes_w(OUT), end = n->end_adjacent_nodes_w(); itnb != end; ++itnb) 
			{
				Vec v = itnb->real_position();
				psc->procircle3(v, 0.1, true );
							psc->mystroke();

			}
		psc->newpage();
		delete psc;
*/		



//--------------------------------------------------------------------------------
//Paint the RIM Communication Model
//--------------------------------------------------------------------------------
/*
NodeFinder nf(&(sc.world_w()));
		Vec v1(0, 0, 0);
		Vec v2(100, 100, 0);
			int edges = 0;
			int more = 0;
			int less = 0;
			psc->setlinewidth2(0.1);
        shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
			Node* n = nf.findByPosition(Vec(50, 50, 0));
			psc->setcolor2(0.9, 0.9, 0.9);

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				psc->procircle3(v2, 0.1, true );
			}
			Vec v = n->real_position();
			psc->procircle3(v, 10, false );
			psc->setcolor2( 0.0,0.0,0.0 );
			for(shawn::Node::adjacency_iterator itnb = n->begin_adjacent_nodes_w(BIDI), end = n->end_adjacent_nodes_w(); itnb != end; ++itnb) 
			{
				cout << "treating neighbor " << itnb->id() << endl;
				Vec v = itnb->real_position();
				psc->procircle3(v, 0.1, true );
							psc->mystroke();
			}
			paint_rim( *psc, rim, v);
			cout << "nach rim Painting" << endl;
			psc->text( Vec(0, 0), getIdString(sc) );
			psc->mystroke();
			cout << getIdString(sc) << endl;
		psc->newpage();
		delete psc;
*/

//--------------------------------------------------------------------------------
//Paint the shared neighbors with different com models for the second paper
//--------------------------------------------------------------------------------
/*
NodeFinder nf(&(sc.world_w()));
		Vec v1(0, 0, 0);
		Vec v2(100, 100, 0);
			int edges = 0;
			int more = 0;
			int less = 0;
			psc->setlinewidth2(0.1);
        //shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		//shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
				
		shawn::CommunicationModel* com = sc.communication_model_keeper_w().find_w( "rim" ).get()->create(sc);	
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
		
		Node* n1 = nf.findByPosition(Vec(50, 50, 0));
		Node* n2 = nf.findByPosition(Vec(55, 50, 0));

		Box b = Box ( Vec( 35, 35, 0), Vec(70, 65, 0));

			psc->setcolor2(0.5, 0.5, 0.5);

			//paint all nodes

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				if (b.contains( v2) ) psc->procircle3(v2, 0.2, true );
			}

			// paint the two nodes	
			psc->setcolor2( 0.0,0.0,0.0 );
			Vec v = n1->real_position();
			psc->procircle3(v, 0.4, false );
			v = n2->real_position();
			psc->procircle3(v, 0.4, false );


			bool c1, c2;

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				c1 = com->can_communicate_uni(*n1, *it);
				c2 = com->can_communicate_uni(*n2, *it);
				v2 = (*it).real_position();
			psc->setcolor2( 0.0,0.0,0.0 );

				if (c1&&c2) psc->procircle3(v2, 0.2, true ); else
					if (c1||c2) {
						psc->setcolor2( 1.0,1.0,1.0 );
						psc->procircle3(v2, 0.2, true ); 
						psc->setcolor2( 0.0,0.0,0.0 );
						psc->procircle3(v2, 0.2, false ); 
					}

			}

			paint_rim( *psc, *rim, *n1);
			paint_rim( *psc, *rim, *n2);
			cout << "nach rim Painting" << endl;

			psc->mystroke();

		psc->newpage();




			psc->setlinewidth2(0.1);

				
com = sc.communication_model_keeper_w().find_w( "disk_graph" ).get()->create(sc);	

		

			psc->setcolor2(0.5, 0.5, 0.5);

			//paint all nodes

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				if (b.contains( v2) ) psc->procircle3(v2, 0.2, true );
			}

			// paint the two nodes	
			psc->setcolor2( 0.0,0.0,0.0 );
			v = n1->real_position();
			psc->procircle3(v, 0.4, false );
			v = n2->real_position();
			psc->procircle3(v, 0.4, false );



			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				c1 = com->can_communicate_uni(*it, *n1);
				c2 = com->can_communicate_uni(*it, *n2);
				v2 = (*it).real_position();
			psc->setcolor2( 0.0,0.0,0.0 );

				if (c1&&c2) psc->procircle3(v2, 0.2, true ); else
					if (c1||c2) {
						psc->setcolor2( 1.0,1.0,1.0 );
						psc->procircle3(v2, 0.2, true ); 
						psc->setcolor2( 0.0,0.0,0.0 );
						psc->procircle3(v2, 0.2, false ); 
					}

			}

			psc->setcolor2( 0.0,0.0,0.0 );
			v = n1->real_position();
			psc->procircle3(v, 10.0, false );

			v = n2->real_position();
			psc->procircle3(v, 10.0, false );
			psc->mystroke();
			cout << getIdString(sc) << endl;
		psc->newpage();


			psc->setlinewidth2(0.1);
						
		com = sc.communication_model_keeper_w().find_w( "stochastic" ).get()->create(sc);	
			psc->setcolor2(0.5, 0.5, 0.5);
			com->set_world(sc.world_w());

			//paint all nodes

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				if (b.contains( v2) ) psc->procircle3(v2, 0.2, true );
			}

			// paint the two nodes	
			psc->setcolor2( 0.0,0.0,0.0 );
			v = n1->real_position();
			psc->procircle3(v, 0.4, false );
			v = n2->real_position();
			psc->procircle3(v, 0.4, false );




			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				c1 = com->can_communicate_uni(*it, *n1);
				c2 = com->can_communicate_uni(*it, *n2);
				v2 = (*it).real_position();
			psc->setcolor2( 0.0,0.0,0.0 );

				if (c1&&c2) psc->procircle3(v2, 0.2, true ); else
					if (c1||c2) {
						psc->setcolor2( 1.0,1.0,1.0 );
						psc->procircle3(v2, 0.2, true ); 
						psc->setcolor2( 0.0,0.0,0.0 );
						psc->procircle3(v2, 0.2, false ); 
					}

			}

//			paint_rim( *psc, *rim, *n1);
//			paint_rim( *psc, *rim, *n2);

			psc->setcolor2( 0.0,0.0,0.0 );
			v = n1->real_position();
			psc->procircle3(v, 7.5, false );
			psc->procircle3(v, 12.5, false );

			v = n2->real_position();
			psc->procircle3(v, 7.5, false );
			psc->procircle3(v, 12.5, false );

			cout << "nach rim Painting" << endl;

			psc->mystroke();

		psc->newpage();



		delete psc;

*/

//--------------------------------------------------------------------------------
//Test node::neighborhood(int) from Dennis
//--------------------------------------------------------------------------------

/*
NodeFinder nf(&(sc.world_w()));
		Vec v1(0, 0, 0);
		Vec v2(100, 100, 0);
			int edges = 0;
			int more = 0;
			int less = 0;
			psc->setlinewidth2(0.1);
        shawn::CommunicationModel* com = &(sc.world_w().communication_model_w());
		shawn::RimCommModel* rim =dynamic_cast<shawn::RimCommModel*>(com);
			Node* n = nf.findByPosition(Vec(50, 50, 0));
			psc->setcolor2(0.9, 0.9, 0.9);

			for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
		    {            
				v2 = (*it).real_position();
				psc->procircle3(v2, 0.1, true );
			}

			Vec v = n->real_position();
			psc->procircle3(v, 10, false );
			psc->procircle3(v, 20, false );
			psc->procircle3(v, 30, false );
			psc->procircle3(v, 40, false );
			psc->setcolor2( 0.0,0.0,0.0 );

			NeighborhoodHandle nh = n->neighborhood_w( 3 );

			for(Neighborhood::iterator itnb = nh->begin(), end = nh->end(); itnb != end; ++itnb) 
			{
				
				//cout << "treating neighbor " << itnb->id() << endl;
				Vec v = (*itnb)->real_position();
				psc->procircle3(v, 0.1, true );
							psc->mystroke();
			}

			//paint_rim( *psc, rim, v);
			cout << "nach rim Painting" << endl;
//			psc->text( Vec(0, 0), getIdString(sc) );
//			psc->mystroke();
//			cout << getIdString(sc) << endl;
		psc->newpage();
		delete psc;
*/
//--------------------------------------------------------------------------------
//do something unknown
//--------------------------------------------------------------------------------

		/*
		int neighborCount;
        int nodeID1 = 0;
        int nodeID2 = 0;


		//preconditions ok?
		if (!initialized) init(sc);
		require_world(sc);
        assert( perfect != NULL && nptr != NULL );
		const World& w = sc.world();
        assert( w.has_size_hint());

		//int this run...
		double r = w.size_hint();
		bool perfdistok = false, unidistok = false, bidiavgdistok = false, bidimindistok = false, bidimaxdistok = false;
        double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
        shawn::Vec rr(r, r, 0);
        shawn::Vec ll = w.lower_left() + rr;
        shawn::Vec ur = w.upper_right() - rr;



        for( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it) 
        {            
            nodeID1 = (*it).id();

            if( out_of_bounds(ll, ur, *it ) ) //TODO: Zum initialen Testen muss nachher raus
                continue;

			neighborCount=  (*it).neighborhood(1).size()-1;
			neigh[neighborCount].nodeCount++;


            for(shawn::Node::const_adjacency_iterator itnb = (*it).begin_adjacent_nodes(); itnb != (*it).end_adjacent_nodes(); ++itnb) 
            {
                nodeID2 = (*itnb).id();

                if( out_of_bounds(ll, ur, *itnb ) ) //TODO: Zum initialen Testen muss nachher raus
                    continue;

                perfdistok = perfect->estimate_distance( (*it), *itnb, perfdist );
                nptr->set_mode(M_UNI);
				unidistok = nptr->estimate_distance( (*it), *itnb, unidist );
                nptr->set_mode(M_BIDI_AVG);
				bidiavgdistok = nptr->estimate_distance( (*it), *itnb, bidiavgdist );
                nptr->set_mode(M_BIDI_MIN);
				bidimindistok = nptr->estimate_distance( (*it), *itnb, bidimindist );
                nptr->set_mode(M_BIDI_MAX);
				bidimaxdistok = nptr->estimate_distance( (*it), *itnb, bidimaxdist );

                if(nodeID1 == nodeID2)
                {
                    continue;
                }
                else
                {
                    if( perfdistok && unidistok && bidiavgdistok && bidimindistok && bidimaxdistok)
                    { 
					   neigh[neighborCount].uniErr.Update((perfdist-unidist)/r);
					   neigh[neighborCount].bidiAvgErr.Update((perfdist-bidiavgdist)/r);
					   neigh[neighborCount].bidiMinErr.Update((perfdist-bidimindist)/r);
					   neigh[neighborCount].bidiMaxErr.Update((perfdist-bidimaxdist)/r);
					   neigh[neighborCount].edgeCount++;
					
                    }
                }
            }

        }
		std::string filename = sc.environment().required_string_param("file_name");
		if (filename != "") writeStats( filename );
*/


//--------------------------------------------------------------------------------
//paint special NIDES cases with RIM
//--------------------------------------------------------------------------------
/*
		
		int neighborCount;
        int nodeID1 = 0;
        int nodeID2 = 0;


		//preconditions ok?
		if (!initialized) init(sc);
		require_world(sc);
        assert( perfect != NULL && nptr != NULL );
		const World& w = sc.world();
        assert( w.has_size_hint());

		//int this run...
		double r = w.size_hint();
		bool perfdistok = false, unidistok = false, bidiavgdistok = false, bidimindistok = false, bidimaxdistok = false;
        double perfdist = 0.0, unidist = 0.0, bidimindist = 0.0, bidimaxdist = 0.0, bidiavgdist = 0.0;
        shawn::Vec rr(r, r, 0);
        shawn::Vec ll = w.lower_left() + rr;
        shawn::Vec ur = w.upper_right() - rr;



		for( shawn::World::node_iterator it = sc.world_w().begin_nodes_w(); it != sc.world_w().end_nodes_w(); ++it) 
        {            
            nodeID1 = (*it).id();

            if( out_of_bounds(ll, ur, *it ) ) //TODO: Zum initialen Testen muss nachher raus
                continue;

//			neighborCount=  (*it).degree();
//			neigh[neighborCount].nodeCount++;


            for(shawn::Node::adjacency_iterator itnb = (*it).begin_adjacent_nodes_w(IN); itnb != (*it).end_adjacent_nodes_w(); ++itnb) 
            {
                nodeID2 = (*itnb).id();

                if( out_of_bounds(ll, ur, *itnb ) ) //TODO: Zum initialen Testen muss nachher raus
                    continue;

                perfdistok = perfect->estimate_distance( (*it), *itnb, perfdist );
//				nptr->set_mode(M_UNI);
//				unidistok = nptr->estimate_distance( (*it), *itnb, unidist );
                nptr->set_mode(M_BIDI_AVG);
				bidiavgdistok = nptr->estimate_distance( (*it), *itnb, bidiavgdist );
//                nptr->set_mode(M_BIDI_MIN);  
//				bidimindistok = nptr->estimate_distance( (*it), *itnb, bidimindist );
//              nptr->set_mode(M_BIDI_MAX);
//				bidimaxdistok = nptr->estimate_distance( (*it), *itnb, bidimaxdist );

                if(nodeID1 == nodeID2)
                {
                    continue;
                }
                else
                {
                    if( perfdistok && bidiavgdistok && (perfdist/r > 0.1) && (perfdist/r < 0.2) && ((bidiavgdist-perfdist)/r) > 0.25)
					{ 
						paint_scene( *psc, (*it), *itnb );					
                    }
                }
            }

        }
//		std::string filename = sc.environment().required_string_param("file_name");
//		if (filename != "") writeStats( filename );

*/   
}
		// ----------------------------------------------------------------------
		bool 
			TestTask::
			out_of_bounds(shawn::Vec& ll, shawn::Vec& ur, const shawn::Node& node)  
			throw()
		{

			if( node.real_position().x() > ur.x() || node.real_position().y() > ur.y() ||
				node.real_position().x() < ll.x() || node.real_position().y() < ll.y() )
				return true;

			return false;


		}


 }
#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/test_task.cpp,v $
* Version $Revision: 197 $
* Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: test_task.cpp,v $
 *-----------------------------------------------------------------------*/
