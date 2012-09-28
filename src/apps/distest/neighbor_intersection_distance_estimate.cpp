/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "apps/distest/neighbor_intersection_distance_estimate.h"
#ifdef ENABLE_DISTEST

#include "sys/node.h"
#include "sys/util/set_count.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include <math.h>
#include "sys/distance_estimates/distance_estimate_keeper.h"
#include "sys/edge_models/grid_edge_model.h"
#include "sys/misc/number_formatter.h"
#include <fstream>
//#include <vector>
//#include <math.h>
//#include <sstream>

#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace shawn
{


	// ----------------------------------------------------------------------
	double
		CommunicationModelFunction::
		ThreeDFunction( double x, double y, double x_offset )
	{
		return TwoDFunction( sqrt(( x - x_offset )*( x - x_offset) + y * y));
	}


	// ----------------------------------------------------------------------
	double
		CommunicationModelFunction::
		stdIntegral( double step )
	{
		double integral = 0.0;
		for ( double y = min_y_; y<= max_y_; y+=step) {
			for ( double x = min_x_; x<= max_x_; x+=step) {
				integral += ThreeDFunction(x, y, 0.0);
			}
		}
		return integral;
	}

	// ----------------------------------------------------------------------
	double
		CommunicationModelFunction::
		multIntegral( double dist, double step )
	{
		double integral = 0.0;
		for ( double y = min_y_; y<= max_y_; y+=step) {
			for ( double x = min_x_; x<= max_x_; x+=step) {
				integral += ThreeDFunction(x, y, 0.0) * ThreeDFunction(x, y, dist);
			}
		}
		return integral;
	}


	void
		CommunicationModelFunction::
		gnuplot3D( string filename, bool mult, double dist )
	{
		double step = 0.05;
		std::ofstream out((char*)(filename.c_str()));
		for ( double x = min_x_; x<= max_x_; x+=step) {
			if ( x > -0.00001 && x < 0.00001 ) x = 0.0;
			for ( double y = min_y_; y<= max_y_; y+=step) {
				if ( y > -0.00001 && y < 0.00001 ) y = 0.0;
				double val;
				if (mult) val = ThreeDFunction(x, y, 0.0) * ThreeDFunction(x, y, dist);
				else val = ThreeDFunction(x, y, dist);
				out << y << "\t" << x << "\t" << val <<  endl;
			}
			out << endl;
		}
		out.close();
	}

	void
		CommunicationModelFunction::
		gnuplot2D( string filename )
	{
		double step = 0.05;
		std::ofstream out((char*)(filename.c_str()));
		for ( double x = 0.0; x<= max_x_; x+=step) {
			if ( x > -0.00001 && x < 0.00001 ) x = 0.0;
			out << x << "\t" << TwoDFunction( x ) <<  endl;
		}
		out.close();
	}

	vector<double>
	CommunicationModelFunction::
	calculateEstimationTable(double step)
	{
		//double step_ = 0.01;
		double min = 0.0;
		double max = 2.0;
		int size = int((max-min)/step);
		double std = stdIntegral( step );
		vector<double> v(size+1);
		int idx = 0;
		for (double dist = 0.0; dist<1.9999; dist += step) {
			double val = multIntegral( dist, step ) / std;
			v[idx]=val;
			idx++;
		}
		v[v.size()-1]=0;
		return v;
	}


	// ----------------------------------------------------------------------
	LinearCommunicationModelFunction::
		LinearCommunicationModelFunction ( double smooth_factor) : smooth_factor_(smooth_factor)
	{}


	// ----------------------------------------------------------------------
	double
		LinearCommunicationModelFunction::
		TwoDFunction( double x )
	{
		if ( 1 - smooth_factor_ > x) return 1;
		if ( 1 + smooth_factor_ < x) return 0;
		return ( -x/(2.0*smooth_factor_)+1.0/(2.0*smooth_factor_)+0.5);
	}

	// ----------------------------------------------------------------------
	double
		CircleCommunicationModelFunction::
		TwoDFunction( double x )
	{
		if ( x <= 1 ) return 1;
			else return 0;
	}

	double
		VectorCommunicationModelFunction::
		TwoDFunction( double x )
	{
		assert ( x >= 0 );
		if ( x >= 2.0 ) return 0;
		int size = vec_.size();
		//double step = 2.0 / size;
		double didx = size*0.5*x;
		int lidx = int(floor(didx));
		int hidx = int(ceil(didx));
		if( hidx >= size)
			hidx--;
		//cout << "vcmf.TwoDFunction( " << x << " ): size=" << size << " didx=" << didx << endl << flush;
		return vec_[lidx] + (didx-lidx)*(vec_[hidx]-vec_[lidx]);
	}

	void
		VectorCommunicationModelFunction::
		setVector( vector<double> v )
	{
		vec_ = v;

		std::string out_file_short="vector_comm_model-func.txt";
		std::ofstream out((char*)(out_file_short.c_str()));
		for (int i = 0; i<int(vec_.size());i++)
			out << DoubleFormatter((double)(i)/100.0) << "; " <<DoubleFormatter(vec_[i]) <<  endl;
		out.close();
	}




	vector<double>
	RimModelFunction::
	calculateEstimationTable(double step)
	{
		double range = rim_.communication_upper_bound() / rim_.get_range();
		int size = int(2*range/step);
		//double std = stdIntegral( step );
		vector<double> v(size);

		if (!iterations_) {
			v[0]=	1;
			v[1]=	0.945902944;
			v[2]=	0.912357465;
			v[3]=	0.883552108;
			v[4]=	0.849990056;
			v[5]=	0.817820207;
			v[6]=	0.789697693;
			v[7]=	0.761217184;
			v[8]=	0.733227261;
			v[9]=	0.70174084;
			v[10]=	0.670473918;
			v[11]=	0.641275524;
			v[12]=	0.612710001;
			v[13]=	0.581760324;
			v[14]=	0.551927388;
			v[15]=	0.525173315;
			v[16]=	0.49589442;
			v[17]=	0.466104103;
			v[18]=	0.437180361;
			v[19]=	0.410610924;
			v[20]=	0.38565462;
			v[21]=	0.357641651;
			v[22]=	0.330855779;
			v[23]=	0.303986564;
			v[24]=	0.279634049;
			v[25]=	0.25429026;
			v[26]=	0.229766964;
			v[27]=	0.207458234;
			v[28]=	0.184722078;
			v[29]=	0.163049712;
			v[30]=	0.141734288;
			v[31]=	0.121859255;
			v[32]=	0.10406405;
			v[33]=	0.087762377;
			v[34]=	0.071975509;
			v[35]=	0.057868792;
			v[36]=	0.045562394;
			v[37]=	0.034684339;
			v[38]=	0.025791054;
			v[39]=	0.018263824;
			v[40]=	0.011716207;
			v[41]=	0.007281803;
			v[42]=	0.003808973;
			v[43]=	0.001596855;
			v[44]=	0.00051831;
			v[45]=	6.63E-05;
			v[46]=	5.76E-06;
			v[47]=	0;
			v[48]=	0;
			v[49]=	0;
			v[50]=	0;
			v[51]=	0;
			v[52]=	0;
			v[53]=	0;
			v[54]=	0;
			v[55]=	0;
			v[56]=	0;
			v[57]=	0;
			v[58]=	0;
			v[59]=	0;
		} else {

		//double min = 0.0;
		//double max = 2.0;
		double d;
		vector<int> shared(size);
		vector<int> total(size);
		Vec zero = Vec (0, 0, 0);
		//bool c1, c2;
		Vec v1, v2;
		double didx, frac;
		int idx;
		for (int i = 0; i < size; i++ ) {
			shared[i] = 0;
			total[i] = 0;
		}

/*
		for (double x1 = min_x_; x1 < max_x_; x1 += step ) {
			cout << " x1: " << x1 << endl;
			for (double y1 = min_y_; y1 < max_y_; y1 += step )
				for (double x2 = min_x_; x2 < max_x_; x2 += step )
					for (double y2 = min_y_; y2 < max_y_; y2 += step ) {
						Vec v1 = Vec(x1, y1, 0 );
						Vec v2 = Vec(x2, y2, 0 );
						d = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
						//
						double didx = d/step;
						int idx;
						double frac = didx - int(didx);
						if (frac < 0.5) idx = floor(didx); else idx = ceil(didx);
						if (idx<size){
//						c1 = rim_.can_communicate_uni( zero, 0, v1 );
//						c2 = rim_.can_communicate_uni( zero, 0, v2 );
						c1 = (v1.euclidean_norm()<=1);
						c2 = (v2.euclidean_norm()<=1);

						if (c1 && c2) shared[idx]++;
						if (c1 || c2) total[idx]++;
						//if (c1 && c2) cout << v1 << " " << v2 << endl;
//						if (!idx) cout << "d:" << d << " idx:" << idx << endl;
						}
					}
		}*/
		for (int i=0; i<iterations_; i++)
		for (double x1 = (-range); x1 < range; x1 += step ) {
			cout << " i: " << i<< " x1: " << x1 << endl;
			for (double y1 = (-range); y1 < range; y1 += step )
				for (double x2 = (-3*range); x2 < (3*range); x2 += step )
					for (double y2 = (-3*range); y2 < (3*range); y2 += step ) {
						v1 = Vec(x1, y1, 0 );
						if (v1.euclidean_norm()<=1 && v1.y()>= 0) {
//						if (rim_.can_communicate_uni( zero, 0, v1 )) {
							d = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
							didx = d/step;
							frac = didx - int(didx);
							if (frac < 0.5) idx = int(floor(didx)); else idx = int(ceil(didx));
							if (idx<size){
								total[idx]++;
								v2 = Vec(x2, y2, 0 );
								if( v2.euclidean_norm()<=1 && v2.y()>= 0) shared[idx]++;
//								if( rim_.can_communicate_uni( zero, 0, v2 )) shared[idx]++;
							}
						}

					}
		}

		for (int i = 0; i < size; i++ )
			if (total[i] > 0) v[i] = double(shared[i])/double(total[i]); else v[i]=0;

		for (int i = 0; i < size; i++ )
//		cout << "i:" << i << " v:" << v[i] << " shared:" << shared[i] << " total: " << total[i] << endl;
		v[v.size()-1]=0;
		v[0]=1;
		}
		return v;
	}





	// ----------------------------------------------------------------------
	NeighborhoodIntersectionDistanceEstimate::
		NeighborhoodIntersectionDistanceEstimate() :
			mode				(0),
			step_				(0.05),
			cmf_				(new CircleCommunicationModelFunction())
	{
		//cout << "vor circle cmf calc est table!" << endl<< flush;
		//estimation_table_.resize(1000);
		estimation_table_ = cmf_->calculateEstimationTable(step_);
		//cout << "nach circle cmf calc est table!" << endl<< flush;
	}

	// ----------------------------------------------------------------------
	NeighborhoodIntersectionDistanceEstimate::
		~NeighborhoodIntersectionDistanceEstimate()
	{
		delete cmf_;
	}

	bool
		NeighborhoodIntersectionDistanceEstimate::
		getFraction( const Node& source,
		const Node& target,
		double& intersection,
		double& sourceSize,
		double& targetSize)
		const throw()
	{
		bool ok = false;


		ok = target.world().can_communicate_uni( target, source);

		//if (!ok) cout << "can not communicate uni " << target.id() << "," << source.id() << endl;
		if (ok) {

			int to_subtractor = 0;
			int from_subtractor = 0;
			int next_subtractor = 0;
			int prev_subtractor = 0;

			if (0 == target.world().simulation_controller().environment().optional_int_param("count_source", 0)) from_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_dest", 0)) to_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_next", 1)) next_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_prev", 1)) prev_subtractor=1;

			//if (prev_subtractor == 0) cout << "PREV == 0!!!" << endl;
			//if (next_subtractor == 0) cout << "NEXT == 0!!!" << endl;


			//The world needs a size hint which indicates a common communication range
			assert( source.world().has_size_hint() );

			//Check how many neighbors the two nodes have in common
			int intersect = 0;



			set<const Node*> source_neighbors = source.get_adjacent_nodes(EdgeModel::CD_IN);
			set<const Node*> target_neighbors = target.get_adjacent_nodes(EdgeModel::CD_IN);

			sourceSize = source_neighbors.size();
			if (source_neighbors.find( &source ) != source_neighbors.end() ) sourceSize -= from_subtractor;
			if (source_neighbors.find( &target ) != source_neighbors.end() ) sourceSize -= to_subtractor;
			if (sourceSize > 0) { sourceSize -=prev_subtractor; }//cout << "Subtracting " << prev_subtractor << " from sourceSize" << endl;}
			targetSize = target_neighbors.size();
			if (target_neighbors.find( &source ) != target_neighbors.end() ) targetSize -= from_subtractor;
			if (target_neighbors.find( &target ) != target_neighbors.end() ) targetSize -= to_subtractor;
			if (targetSize > 0) { targetSize -=next_subtractor; }//cout << "Subtracting " << next_subtractor << " from targetSize" << endl;}



			if (!sourceSize || !targetSize) {
				//cout << "NIDES: source  or target size became 0: sourceSize=" << sourceSize << ", targetSize=" << targetSize << endl;
				return false;
			}

			count_set_differences( source_neighbors, target_neighbors, NULL, NULL, NULL, NULL, &intersect, NULL);

			if (source_neighbors.find( &source ) != source_neighbors.end() &&
				target_neighbors.find( &source ) != target_neighbors.end()) intersect -= from_subtractor;
			if (source_neighbors.find( &target ) != source_neighbors.end() &&
				target_neighbors.find( &target ) != target_neighbors.end()) intersect -= to_subtractor;

//			cout << "sourceSize=" << sourceSize << " targetSize=" << targetSize << " intersection=" << intersection << " from_subtrator=" << from_subtractor << " to_subtractor= " << to_subtractor << endl << flush;
			assert( intersect >= 0);
			intersection = intersect;
		}
		return ok;
	}

/*

	bool
		NeighborhoodIntersectionDistanceEstimate::
		getFraction( const Node& source,
		const Node& target,
		double& sharedNodes,
		double& totalNodes)
		const throw()
	{
		bool ok = false;
		int myMode;

		std::string direction_mode_;
		if (mode == 0) {
			direction_mode_ = target.world().simulation_controller().environment().required_string_param("direction_mode");
			if (direction_mode_ == "unidirectional") myMode = M_UNI;
			if (direction_mode_ == "bidirectional_average") myMode = M_BIDI_AVG;
			if (direction_mode_ == "bidirectional_maximum") myMode = M_BIDI_MAX;
			if (direction_mode_ == "bidirectional_minimum") myMode = M_BIDI_MIN;
			assert (myMode != 0);
		} else myMode=mode;

		//if ( myMode != M_BIDI_AVG) cout << "not using avg" << endl;
//
//
//
//		if ( mode == M_UNI)
//			ok = target.world().can_communicate_uni(source, target);
//		else
//			ok = target.world().can_communicate_bidi(source, target);
//

		ok = target.world().can_communicate_uni( target, source);

		if (!ok) cout << "can not communicate uni " << target.id() << "," << source.id() << endl;
		if (ok) {

			int to_subtractor = 0;
			int from_subtractor = 0;
			int next_subtractor = 0;
			int prev_subtractor = 0;

			if (0 == target.world().simulation_controller().environment().optional_int_param("count_source", 0)) from_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_dest", 0)) to_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_next", 1)) next_subtractor=1;
			if (0 == target.world().simulation_controller().environment().optional_int_param("count_prev", 1)) prev_subtractor=1;

			//if (prev_subtractor == 0) cout << "PREV == 0!!!" << endl;
			//if (next_subtractor == 0) cout << "NEXT == 0!!!" << endl;


			//The world needs a size hint which indicates a common communication range
			assert( source.world().has_size_hint() );

			//Check how many neighbors the two nodes have in common
			int intersection = 0;



			set<const Node*> source_neighbors = source.get_adjacent_nodes(IN);
			set<const Node*> target_neighbors = target.get_adjacent_nodes(IN);

//			int sourceSize = source_neighbors.size()-(from_subtractor + to_subtractor);
//			int targetSize = target_neighbors.size()-(from_subtractor + to_subtractor);

			int sourceSize = source_neighbors.size();
			if (source_neighbors.find( &source ) != source_neighbors.end() ) sourceSize -= from_subtractor;
			if (source_neighbors.find( &target ) != source_neighbors.end() ) sourceSize -= to_subtractor;
			if (sourceSize > 0) { sourceSize -=prev_subtractor; }//cout << "Subtracting " << prev_subtractor << " from sourceSize" << endl;}
			int targetSize = target_neighbors.size();
			if (target_neighbors.find( &source ) != target_neighbors.end() ) targetSize -= from_subtractor;
			if (target_neighbors.find( &target ) != target_neighbors.end() ) targetSize -= to_subtractor;
			if (targetSize > 0) { targetSize -=next_subtractor; }//cout << "Subtracting " << next_subtractor << " from targetSize" << endl;}



			if (!sourceSize || !targetSize) {
				//cout << "NIDES: source  or target size became 0: sourceSize=" << sourceSize << ", targetSize=" << targetSize << endl;
				return false;
			}

			count_set_differences( source_neighbors, target_neighbors, NULL, NULL, NULL, NULL, &intersection, NULL);

//ACHTUNG: Die folgenden beiden Zeilen nur zwecks vergleichbarkeit mit Horst!!!
			if (source_neighbors.find( &source ) != source_neighbors.end() &&
				target_neighbors.find( &source ) != target_neighbors.end()) intersection -= from_subtractor;
			if (source_neighbors.find( &target ) != source_neighbors.end() &&
				target_neighbors.find( &target ) != target_neighbors.end()) intersection -= to_subtractor;

//			intersection -= (to_subtractor + from_subtractor);

//			cout << "sourceSize=" << sourceSize << " targetSize=" << targetSize << " intersection=" << intersection << " from_subtrator=" << from_subtractor << " to_subtractor= " << to_subtractor << endl << flush;


			assert( intersection >= 0);
			switch (myMode) {

				case M_BIDI_AVG:
							totalNodes = double(sourceSize + targetSize)/2.0;
							break;
				case M_BIDI_MIN:
							totalNodes = std::min(sourceSize, targetSize);
							break;
				case M_BIDI_MAX:
							totalNodes = std::max(sourceSize, targetSize);
							break;
				case M_UNI:
				default:
							totalNodes = sourceSize;
							break;
			}

			if (totalNodes == 0) {
				cout << "totalNodes == 0" << endl;
				return false;
			}
			sharedNodes = intersection;
		}
		return ok;
	}
*/

	// ----------------------------------------------------------------------
	bool
		NeighborhoodIntersectionDistanceEstimate::
		estimate_distance( const Node& source,
		const Node& target,
		double& result )
		const throw()
	{
		double sharedNodes;
		double totalNodes;
		double sourceSize;
		double targetSize;
		double r = source.world().size_hint();

		bool ok = getFraction( source, target, sharedNodes, sourceSize, targetSize );
		//cout << "sharedNodes:" << sharedNodes << " sourceSize:" << sourceSize << " targetSize:" << targetSize << endl;


		int myMode;
		std::string direction_mode_;
		if (mode == 0) {
			direction_mode_ = target.world().simulation_controller().environment().required_string_param("direction_mode");
			if (direction_mode_ == "unidirectional") myMode = M_UNI;
			if (direction_mode_ == "bidirectional_average") myMode = M_BIDI_AVG;
			if (direction_mode_ == "bidirectional_average2") myMode = M_BIDI_AVG2;
			if (direction_mode_ == "bidirectional_maximum") myMode = M_BIDI_MAX;
			if (direction_mode_ == "bidirectional_minimum") myMode = M_BIDI_MIN;
			assert (myMode != 0);
		} else myMode=mode;

			switch (myMode) {

				case M_BIDI_AVG:
							totalNodes = double(sourceSize + targetSize)/2.0;
							break;
				case M_BIDI_MIN:
							totalNodes = std::min(sourceSize, targetSize);
							break;
				case M_BIDI_MAX:
							totalNodes = std::max(sourceSize, targetSize);
							break;
				case M_UNI:
				default:
							totalNodes = sourceSize;
							break;
			}

			if (totalNodes == 0) {
				//cout << "totalNodes == 0" << endl;
				return false;
			}
//			sharedNodes = intersection;


		if (ok) {

			//double fraction = double(sharedNodes)/double(totalNodes);
/*
			bool limit_to_r = (1 == target.world().simulation_controller().environment().optional_int_param("limit", 0));

			if ((fraction < 0.39)&&(limit_to_r)) {
				fraction=0.39;
				//cout << "limiting distance to r" << endl;
			}*/
			if (myMode==M_BIDI_AVG2) {
				result = (calcEstimate( double(sharedNodes)/double(sourceSize), r ) + calcEstimate( double(sharedNodes)/double(targetSize), r ))/2.0;

			} else result = calcEstimate( double(sharedNodes)/double(totalNodes), r );
		}

		//cout << result << endl;

		return ok;
	}
/*
	// ----------------------------------------------------------------------
	bool
		NeighborhoodIntersectionDistanceEstimate::
		estimate_distance( const Node& source,
		const Node& target,
		double& result )
		const throw()
	{

//		double correction[8] = {0.20, 0.16, 0.15, 0.13, 0.1, 0.06, 0.04, 0.02 };
		double correction[11] = {0.40, 0.31, 0.19, 0.11, 0.08, 0.07, 0.05, 0.04, 0.03, 0.02, 0.01 };
		bool correct = (1 == target.world().simulation_controller().environment().optional_int_param("correction", 0));

		double sharedNodes;
		double totalNodes;
		double r = source.world().size_hint();

		bool ok = getFraction( source, target, sharedNodes, totalNodes );
		if (ok) {

			double fraction = double(sharedNodes)/double(totalNodes);

			bool limit_to_r = (1 == target.world().simulation_controller().environment().optional_int_param("limit", 0));

			if ((fraction < 0.39)&&(limit_to_r)) {
				fraction=0.39;
				//cout << "limiting distance to r" << endl;
			}

			result = calcEstimate( fraction, r );

//
//			if (correct&& totalNodes < 11) {
//				double result2;
//				unsigned short int n = int(floor (totalNodes));
//				target.world().simulation_controller().distance_estimate_keeper().find( "perfect_estimate" )->estimate_distance( source, target, result2);
//				cout << "correcting! n=" << totalNodes << " dist=" << result2 << " " << result << "+=" << r * correction[n] << endl;
//				result += r * correction[n];
//			}
//

		}

		return ok;
	}
*/
	// ----------------------------------------------------------------------
	double
		NeighborhoodIntersectionDistanceEstimate::
		calcEstimate( double fraction, double radius)
		const throw()
	{

		int size = estimation_table_.size();
		int i = 0;
		double d;
		bool ok = true;
		bool overflow = false;
		while ( ok ) {
			if (i>=size) { ok = false; overflow=true; }
			else {if (estimation_table_[i] > fraction) i++; else ok = false;}
		}
		assert( !overflow);
		if (i==0) d=i; else {

			double corr = (fraction-estimation_table_[i])/(estimation_table_[i-1]-estimation_table_[i]);
			d=double(i)- corr;
		}
		return d*step_*radius;

/*
		double Kl = KiSeq[l];
		double Kh = KiSeq[h];
		double weight = (angle - (l * granularity_))/(double)(granularity_);
		return Kl + (Kh-Kl) * weight / (double)(granularity_);
*/
/*
		int size = estimation_table_.size();
		int i = 0;
		double d;
		bool ok = true;
		bool overflow = false;
		while ( ok ) {
			if (i>=size) { ok = false; overflow=true; }
			else if (estimation_table_[i] > fraction) i++; else ok = false;
///			cout << "i:" << i << " table[i]:"<<estimation_table_[i]<< endl;
		}
		assert( !overflow);
		if (i==0) d=i; else d=i-0.5;
		return d*step_*radius;
*/



/*

			// Define SQRT(2)
			const double wurzel2 = sqrt(2.0);

			//k = 0.25 * (M_PI * r * r * k);// //v1  Umrechnung des Kreisbruchteils k in echten Integralwert
			//double asin_part = double(3.0 * wurzel2 * (4 * k - M_PI * r * r) ) / double(16.0 * r * r); //v1


			double asin_part = double(3.0 * wurzel2 * M_PI * (fraction - 1)) / 16.0; //v2 Zusammenfassung der beiden oben

			assert( asin_part <= 1.0 );
			assert( asin_part >= -1.0 );


			return ( -4.0 * wurzel2 * radius * sin( asin(asin_part) / 3.0 ) );
*/


//--------------------------- example calculations from the paper...
/*			double A = 0.25 * (M_PI * 100 * 100 * (13.0/21.0));
			asin_part = double(3.0 * wurzel2 * (4 * A - M_PI * 100 * 100) ) / double(16.0 * 100 * 100);

			double example = -4.0 * wurzel2 * 100 * sin( asin(asin_part) / 3.0 );

			cout << "A=" << A  << " asin_part=" << asin_part  << " distance=" << example << endl;
*/
			// std::cout << source.id() << ", " << target.id() << ": k=" << k << " inter=" << intersection << ", res=" << result<<", asin=" << asin_part << std::endl;


	}

	void
		NeighborhoodIntersectionDistanceEstimate::
		set_mode(int newmode )
	{
		mode = newmode;
	}

	// ----------------------------------------------------------------------
	std::string
		NeighborhoodIntersectionDistanceEstimate::
		name( void )
		const throw()
	{
		return std::string("neighborhood_intersection_estimate");
	}

	// ----------------------------------------------------------------------
	std::string
		NeighborhoodIntersectionDistanceEstimate::
		description( void )
		const throw()
	{
		return std::string("Estimate of node distances based on the intersection of neighbors");
	}

	// ----------------------------------------------------------------------
	void
		NeighborhoodIntersectionDistanceEstimate::
		setCommunicationModelFunction( CommunicationModelFunction* cmf)
	{
		delete cmf_;
		cmf_ = cmf;
		estimation_table_ = cmf_->calculateEstimationTable(step_);
		writeEstimationTable();
		gnuplotEstimationTable(  );
	}


	void
		NeighborhoodIntersectionDistanceEstimate::
		writeEstimationTable(  )
	{
		cout << "Writing estimation table..."<< endl << flush;
		std::string out_file_short="estimation-table.txt";
		std::ofstream out((char*)(out_file_short.c_str()));
		for (int i = 0; i<int(estimation_table_.size());i++)
			out << DoubleFormatter(i*step_) << "; " <<DoubleFormatter(estimation_table_[i]) <<  endl;
		out.close();

	}

	void
		NeighborhoodIntersectionDistanceEstimate::
		gnuplotEstimationTable(  )
	{
		std::string out_file_short="gnuplot-estimation-table.txt";
		std::ofstream out((char*)(out_file_short.c_str()));
		for (int i = 0; i<int(estimation_table_.size());i++)
			out << i*step_ << "\t" <<estimation_table_[i] <<  endl;
		out.close();

	}


}

#endif // #ifdef ENABLE_DISTEST

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/distest/neighbor_intersection_distance_estimate.cpp,v $
* Version $Revision: 237 $
* Date    $Date: 2008-08-22 15:07:26 +0200 (Fri, 22 Aug 2008) $
*-----------------------------------------------------------------------
* $Log: neighbor_intersection_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
