/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/comm_models/rim_comm_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/misc/random/normal_random_variable.h"
#include "sys/misc/random/weibull_random_variable.h"
#include "sys/taggings/map_tags.h"
#include "sys/taggings/group_tag.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/util/string_conv.h"
#include "sys/misc/number_formatter.h"

#include <cstdlib>
#include <sstream>
#include <math.h>
#include <cassert>
//#include <cmath>
#include <fstream>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


using namespace std;

namespace shawn 
{
	/*RimCommModel::RimCommModel(double range,double min_range,double max_range,double doi,double granularity,double vsp) : 
	RimCommModel( 3.0, 5.0, 0.0, 0.0, range, min_range, max_range, doi, granularity, vsp )
	{}*/

	RimCommModel::RimCommModel(double alpha,double beta,double mean,double variance, double range,double min_range,double max_range,double doi,int granularity,double vsp, double epsilon, int rot, int sequence_count)
		:
		range_			( range ),
		min_range_      ( min_range ),
		max_range_		(max_range ),
		doi_            (doi ),
		granularity_	(granularity ),
		epsilon_		(epsilon),
		rotation_		(rot),
		sequence_count_	(sequence_count),
		sequences_		(std::vector<std::vector<double> > ((size_t)sequence_count) )
//		KiSeq_			(std::vector<double> ((size_t)ceil((double)(360/granularity))) )

	{ 
		if ( (doi < 0 ) || (doi > 1))
		{
			throw std::logic_error("ERROR: doi must be between 0 and 1");
		}	
		else if (!((granularity >= 1.0) && ((360 % granularity) == 0)))
		{	
			throw std::logic_error("ERROR: granularity must be factor of 360 and >= 2");;
		}	
		else
		{
			wrv_.set_alpha(alpha);
			wrv_.set_beta(beta);
			wrv_.init();
			nrv_.set_mean(mean);
			nrv_.set_variance(variance);
			nrv_.init();

			// Calculating the VSP adjusted sending power in this model
			// vsp = the maximum percentage variance of the signal sending power among different
			// devices.
			vsp_ = 1 + sign()*(nrv_ * vsp);
			sequences_.resize(sequence_count_);
			for (int j = 0; j < sequence_count_; j++) sequences_[j].resize(int(360/granularity));
		}
	}
	//---------------------------------------------------
	RimCommModel::~RimCommModel()
	{
		//TODO
		//Dispose Ki
	}

	//---------------------------------------------------
	int RimCommModel::sign()
	{
		double temp = double(rand()) / double(RAND_MAX);
		if (temp < 0.5)
			return -1;
		else
			return 1;
	}

	//------------------------------------------------------
	double RimCommModel::frac(double d ) {
		return d - int(d);
	}

	//------------------------------------------------------
	double RimCommModel::interpolate_Ki(double angle, int id)  const throw()
	{	
		
		assert (granularity_ > 0);

		if (rotation_ > 0) angle += ( id % 360);
		if ( angle >= 360.0) angle -= 360.0;
		
		//cout << "asking model no " << id%sequence_count_ << endl;
		std::vector<double> KiSeq = sequences_[id%sequence_count_];
		int l = int(floor(angle/double(granularity_))); 
		int h;
		if (l+1 == int(KiSeq.size())) h = 0; else h=l+1;

		assert ( l >= 0 );
		assert ( h < int(KiSeq.size()) );

		double Kl = KiSeq[l];
		double Kh = KiSeq[h];


		double weight = (angle - (l * granularity_))/(double)(granularity_);
		//cout << " angle:" << angle << " ki["<<l<<"]:" << Kl << " ki["<<h<<"]:" << Kh << endl;
		return Kl + (Kh-Kl) * weight / (double)(granularity_);
	}

	// ----------------------------------------------
	int
		RimCommModel::
		get_granularity( void )
		const throw()
	{
		return granularity_;
	}

	//----------------------------------
	void RimCommModel::
		set_doi( double doi )
		throw()
	{
		assert(world_w().edge_model_w().supports_mobility());

		doi_ = doi;
		generate_sequences();

	}
	//-----------------------------------

	void RimCommModel::
		generate_sequences()
	{
		vector<double> Ki_temp(360);
		vector<double> Ki_temp2(int(360/granularity_));
		int count = (int)(360 / granularity_);
		cout << "Generating "<<sequence_count_<<" sequences...";
		for (int j = 0; j < sequence_count_; j++ ) {
//			cout << "generating sequence " << j << endl << flush;
			Ki_temp = generate_Ki_seq(doi_);
			for (int i= 0; i <count; i++ ) {
				Ki_temp2[i] = Ki_temp[i*granularity_];
			}
			sequences_[j]= Ki_temp2;
		}
		cout << " done" << endl << flush;
/*		for (int j = 0; j < sequence_count_; j++ ) 
			for (int i= 0; i < sequences_[j].size(); i++ )
				cout << "j:"<<j<<" i:" << (sequences_[j])[i] << endl;*/
	}

	//-----------------------------------
	void RimCommModel::
		set_granu( int granularity ) throw()
	{
		assert(world_w().edge_model_w().supports_mobility());

		// if granulatity is reset, a new Ki_seq has to be generated
		granularity_= granularity;
		generate_sequences();
	}


	//------------------------------------------------------------------------
	// Creates possible sequence of K(i) values
	// or aborts, if no possible sequence is found
	vector<double> RimCommModel::generate_Ki_seq(double doi)throw()
	{
		//const double epsilon = 0.05;
		vector<double> KiSeq(360);
		KiSeq[0] = (min_range_+max_range_)/2;
		for (int i = 1; i < 360 ;i++) KiSeq[i] = 1000;
		int tries = 100000000;
		double avg = 1000;
		double integral = 1000;
		int pos = 0;
		while (	( ( fabs( KiSeq[pos % 360] - KiSeq[(pos + 1) % 360]) > doi) ||
				  (fabs( integral - 1.0) > epsilon_) )
			&& 
				(tries > 0))
		{
			pos++;
			KiSeq[pos%360]= 1000;
			while (!((KiSeq[pos % 360] >= min_range_) && (KiSeq[pos % 360] <= max_range_))) {
				KiSeq[pos % 360] = KiSeq[(pos-1) % 360] + wrv_ * sign() * doi;
			
			}
			avg = 0.0;
			integral =0.0;
			for (int i = 0; i < 360; i++) {
				avg+= KiSeq[i];
				integral += KiSeq[i]*KiSeq[i%360];
			}
			integral *= sin(M_PI/180.0)*0.5;
			integral /= M_PI;
			avg/=360.0;
			tries--;
		}
		assert( tries > 0 );

		//cout << "Avg=" << avg << " integral=" << integral << endl;


		return KiSeq;

	}
	//---------------------------------------
	bool
		RimCommModel::
		can_communicate_bidi( const Node& u,
		const Node& v )
		const throw()
	{
		return can_communicate_uni( u, v )
			&&
			can_communicate_uni( v, u );
	}

	double 
		RimCommModel::
		angle(  Vec u, Vec v )
		const throw()
	{
		double distance = euclidean_distance(u, v);
		double dx = v.x() - u.x();
		double dy = v.y() - u.y();

		double degree_u =asin( abs(dy)/distance);

		if (dx > 0) {
			if (dy < 0 )							//quadrant IV
				degree_u = 2 * M_PI - degree_u;
		}
		else
			if (dy < 0 )							//quadrant III
				degree_u +=  M_PI;
			else
				degree_u = M_PI - degree_u;			//quadrant II
		return degree_u / ( 2 * M_PI) * 360;	//transcode from radian to degree

	}



	// ----------------------------------------------------------------------
	bool
		RimCommModel::
		can_communicate_uni( const Node& u,
		const Node& v )
		const throw()
	{
		double distance = euclidean_distance(u.real_position(), v.real_position());
		
		if ( distance < range_ * min_range_ * vsp_) return true;
		if ( distance > range_ * max_range_ * vsp_) return false;
		// Calculate transmission range
		double a = angle( u.real_position(), v.real_position() );
		
		//double i = interpolate_Ki(a, u.id());
		//cout <<" d=" << distance <<  " interpol="<<i<<" vsp:" << vsp_<< " a=" << a << endl; 
		return  distance <= range_ * interpolate_Ki(a, u.id()) * vsp_;
	}
	
	bool
		RimCommModel::
		can_communicate_uni( Vec u, int uid, Vec v )
		const throw()
	{
		double distance = euclidean_distance(u, v);
	
		//cout << " u: " << u << " v: " << v << " d=" << distance << endl; 

		if ( distance < min_range_ * vsp_) return true;
		if ( distance > max_range_ * vsp_) return false;
		// Calculate transmission range
		double a = angle( u, v );
		//cout << "angle:" << a << endl;


		

		double i = interpolate_Ki(a, 1);

		//cout <<" d=" << distance <<  " interpol="<<i<<" vsp:" << vsp_<< " a=" << a << endl; 
		return distance <= i * vsp_;
	}

	// ----------------------------------------------------------------------
	bool
		RimCommModel::
		exists_communication_upper_bound( void )
		const throw()
	{
		return true;
	}
	// ----------------------------------------------------------------------
	bool
		RimCommModel::exists_communication_lower_bound( void ) const throw()
	{
		return true;
	}
	// ----------------------------------------------------------------------
	double
		RimCommModel::
		communication_lower_bound( void )
		const throw()
	{
		return range_ * min_range_;
	}
	// ----------------------------------------------------------------------
	double
		RimCommModel::
		communication_upper_bound( void )
		const throw()
	{
		return range_ * max_range_;
	}
	// ----------------------------------------------------------------------
	bool
		RimCommModel::
		is_status_available_on_construction( void )
		const throw()
	{
		return true;
	}


	// --------------------------------------------------------------------
	void
		RimCommModel::
		save_to_tag(SimulationEnvironment& se) throw()
	{
		/*Vector<double> v;
		IntegerDoubleMapTag* t;
		for (int j = 0; j < sequence_count; j++ ) {
			t = new IntegerDoubleMapTag("ki_seq");
				t->set_persistency(true);

				for (int i = 0; i < KiSeq_.size(); i++)
		{
			t->value_w().insert(make_pair(i,KiSeq_[i]));
		}
		se.add_tag(t);*/
	}

	// --------------------------------------------------------------------
	void
		RimCommModel::
		save_to_file()
	{
		ofstream o ("rim.xml",ios_base::app);
		o << "<scenario><environment><tag type=\"group\" name=\"rim\">";
		for (int j = 0; j < sequence_count_; j++ ) {

			
			o << "<tag sequence-no=\""<<j<<"\" type=\"map-int-double\" name=\"ki_seq\">" <<endl;
			for (int i = 0; i < int(sequences_[j].size()); i++)
			{
				o << "<entry index=\""<<i<<"\" value=\""<<sequences_[j][i]<< "\" />"<<endl;
			}
			o << "</tag>" <<endl;
		}
		o << "</tag></environment></scenario>" <<endl;
		
	}


	// --------------------------------------------------------------------
	bool
		RimCommModel::
		load_from_tag(const SimulationEnvironment& se) throw()
	{
		/*
		ConstTagHandle pl_tag_group = se.find_tag("rim");
		const GroupTag* gt = pl_tag_group.is_not_null() ? dynamic_cast<const GroupTag*> (pl_tag_group.get()) : NULL;

		if (gt != NULL){
			ConstTagHandle t = gt->find_tag("ki_seq");

			if( t.is_not_null() )
			{
				const IntegerDoubleMapTag* mt = dynamic_cast<const IntegerDoubleMapTag*> (t.get());
				if( mt != NULL )
				{
					const IntegerDoubleMapTag::Map& m = mt->value();
					for ( IntegerDoubleMapTag::Map::const_iterator it = m.begin(); it != m.end(); ++it ) {
						KiSeq_[it->first] = it->second;
					}
					return true;
				}
			}
		}*/
		return false;
	}

	std::vector<double>
		RimCommModel::
		get_Ki(int sequence_index){
			//assert( sequence_index < sequence_count_);
			return sequences_[sequence_index%sequence_count_];
		
	}

	double RimCommModel::
		get_range(){
			return range_;
	}

	// Generates Output for GnuPlot
	/*	void RimCommModel::print( void ) throw()
	{
	//		ofstream outfile ("test.plt",ios_base::app);
	//		for (int i = 0; i < KiSeq_.size(); i++)
	//		{
	//			outfile << i << " " << DoubleFormatter( KiSeq_[i]) <<endl;
	//		}
	//
	//    ----- alternatives -----
	//		double sum = 0;
	//		for (int i = 0; i<100000; i++){
	//			vector<double> v = Ki_seq(doi_);
	//			for (int j = 0;j<360;j++)sum+=v[j];
	//		}
	//		sum/=(100000*360);

	//		cout << "durchschnitt: " << sum;

	}*/

}//namespace shawn





/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/rim_comm_model.cpp,v $
* Version $Revision: 182 $
* Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: rim_comm_model.cpp,v $
 *-----------------------------------------------------------------------*/
