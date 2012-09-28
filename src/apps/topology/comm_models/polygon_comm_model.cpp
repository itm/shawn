/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "shawn_config.h"


#include "apps/topology/comm_models/polygon_comm_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/tag_container.h"
#include "sys/taggings/basic_tags.h"
#include "sys/util/defutils.h"
#include "sys/distance_estimates/distance_estimate_keeper.h"

#include <fstream>
#include <math.h>
#include <cassert>
#include <limits>
#include <string>

using namespace std;
using namespace shawn;

namespace topology
{


	CommonTaggedPolygonBase::
		CommonTaggedPolygonBase(const PolygonTopology& topo,double upper, double lower, std::string fn, std::string rssi2distfile)
		: topo_(topo),
		initialized_ ( false ),
		upper_bound_  ( upper ),
		lower_bound_ ( lower ),
		has_range_   ( false ),
		fname_		 ( fn ),
		distance_estimate_rssi_dist_file_ (rssi2distfile)
	{}
	// ----------------------------------------------------------------------
	CommonTaggedPolygonBase::
		~CommonTaggedPolygonBase()
	{
		rssi_.clear();
		distance_estimate_rssi_dist_file_.clear();
	}

	// ----------------------------------------------------------------------
	void
		CommonTaggedPolygonBase::
		init( void )
		throw()
	{
		if(!initialized_)
		{
			initialized_ = true;
			urv_.set_upper_bound_inclusive(true);
			urv_.init();
			if( fname_.compare("")==0||distance_estimate_rssi_dist_file_.compare("") == 0)
				create_default_map();
			else{
				initialized_ = true;
				rssi_.clear();
				readFile();
			}

			cout << "CommonTaggedPolygonBase: initialized" << endl;

		}
	}

	// ----------------------------------------------------------------------
	void
		CommonTaggedPolygonBase::
		set_transmission_range( double tr )
		throw()
	{
		assert( !initialized_ );
		upper_bound_ = tr;
		has_range_ = true;
		cout << "CommonTaggedPolygonBase: Transmission range set to ["<< upper_bound_ <<"]" << endl;
	}

	// ----------------------------------------------------------------------
	double
		CommonTaggedPolygonBase::
		transmission_range( void )
		const throw()
	{
		return upper_bound_;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		can_communicate_uni( const Node& u, const Node& v )
		const throw()
	{
		if(u.id() == v.id())
			return true;

		double rssi = getRSSI(euclidean_distance( u.real_position(), v.real_position() ));

		if( ( rssi  ) >= lower_bound_ )
			return true;

		double variable = (urv_);
		if( variable  <= (rssi /lower_bound_) )
		{
			assert(lower_bound_ != 0);
			return true;
		}

		return false;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyDistEst::
		estimate_distance( const Node& source, const Node& target, double& result )
		const throw()
	{
		//If nodes can not communication, no estimate is possible
		if( !source.world().communication_model().can_communicate_uni(source, target) )
			return false;


		double real_dist = euclidean_distance( source.real_position(), target.real_position() );
		double rssi = getRSSI( real_dist ) ;
		result = get_distance( rssi );

		cout << "CommonTaggedPolygonBase::estimate_distance: " << source.id() << " > " << target.id() << ", realdist " << real_dist << ", estimation " <<  result << endl;

		if( result < 0.0 )
			return false;

		return true;
	}

	// ----------------------------------------------------------------------
	std::string
		PolygonTopologyDistEst::
		name( void )
		const throw()
	{
		return name_;
	}

	// ----------------------------------------------------------------------
	std::string
		PolygonTopologyDistEst::
		description( void )
		const throw()
	{
		return "";

	}// ----------------------------------------------------------------------
	std::string
		PolygonTopologyCommunicationModel::
		name( void )
		const throw()
	{
		return "polygon_topology";
	}

	// ----------------------------------------------------------------------
	std::string
		PolygonTopologyCommunicationModel::
		description( void )
		const throw()
	{
		return "";
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		can_communicate_bidi( const Node& u, const Node& v )
		const throw()
	{
		return can_communicate_uni(u, v) && can_communicate_uni(v, u);
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		exists_communication_upper_bound( void )
		const throw()
	{
		return true;
	}

	// ----------------------------------------------------------------------
	double
		PolygonTopologyCommunicationModel::
		communication_upper_bound( void )
		const throw()
	{
		return upper_bound_;
	}

	// ----------------------------------------------------------------------
	bool
		PolygonTopologyCommunicationModel::
		is_status_available_on_construction( void )
		const throw()
	{
		return true;
	}

	// ----------------------------------------------------------------------
	void
		PolygonTopologyCommunicationModel::
		set_size_hint(double size_hint)
		throw()
	{
		if(has_range_)
			return;

		has_range_ = true;
		cout << "CommonTaggedPolygonBase: Using size hint ["<< upper_bound_ <<"] as comm range" << endl;
	}

	// ----------------------------------------------------------------------
	bool
		CommonTaggedPolygonBase::
		readFile()
		throw(std::runtime_error)
	{
		/** Load dist2rssi **/
		double key = 0, val = 0, factor = 0;
		ifstream in(fname_.c_str());
		if( !in )
			throw std::runtime_error(string("CommonTaggedPolygonBase:: cannot load file ")+fname_);

		in >>factor;
		assert( factor != 0 );

		while(in >> key >> val)
			rssi_.insert( make_pair( (key / factor), val) );

		in.close();

		/** Load rssi2dist **/
		ifstream in2(distance_estimate_rssi_dist_file_.c_str());
		if( !in2 )
			throw std::runtime_error(string("CommonTaggedPolygonBase:: cannot load file ")+distance_estimate_rssi_dist_file_);

		in2 >> factor;
		assert( factor != 0 );

		while(in2 >> key >> val)
			distance_estimate_rssi_dist_.insert( make_pair(val, (key / factor)) );

		in2.close();
		return true;
	}

	// ----------------------------------------------------------------------
	void
		CommonTaggedPolygonBase::
		create_default_map()
		throw()
	{
		rssi_.insert( make_pair(0.0 ,255.0 ) );
		rssi_.insert( make_pair( 0.45, 191.55 ) );
		rssi_.insert( make_pair( 0.9,161.86 ) );
		rssi_.insert( make_pair( 1.35, 148.63) );
		rssi_.insert( make_pair( 1.80,130.46 ) );
		rssi_.insert( make_pair( 2.25,127.0 ) );
		rssi_.insert( make_pair( 2.7,116.85 ) );
		rssi_.insert( make_pair( 3.15,103.83 ) );
		rssi_.insert( make_pair( 3.60, 99.32) );
		rssi_.insert( make_pair( 4.05, 91.83) );
		rssi_.insert( make_pair( 4.5, 90.94 ) );
		rssi_.insert( make_pair( 4.95,95.3 ) );
		rssi_.insert( make_pair( 5.4, 87.38) );
		rssi_.insert( make_pair( 5.85, 92.53) );
		rssi_.insert( make_pair( 6.3,84.47 ) );
		rssi_.insert( make_pair( 6.75,90.51 ) );
		rssi_.insert( make_pair( 7.20, 77.44) );
		rssi_.insert( make_pair( 7.65, 83.59) );
		rssi_.insert( make_pair( 8.1, 74.56) );
		rssi_.insert( make_pair( 8.55, 77.61) );
		rssi_.insert( make_pair( 9.0,81.87 ) );
		rssi_.insert( make_pair( 9.45, 91.01) );
		rssi_.insert( make_pair( 9.9, 80.77) );
		rssi_.insert( make_pair( 10.35,76.1 ) );
		rssi_.insert( make_pair( 10.8,65.22 ) );
		rssi_.insert( make_pair( 11.25,89.63 ) );
		rssi_.insert( make_pair( 11.7,82.43 ) );
		rssi_.insert( make_pair( 12.15, 68.78) );
		rssi_.insert( make_pair( 12.60,71.73 ) );
		rssi_.insert( make_pair( 13.05, 72.5) );
		rssi_.insert( make_pair( 13.50, 69.45 ) );
		rssi_.insert( make_pair( 18.0, 59.3) );
		rssi_.insert( make_pair( 22.5, 29.45 ) );
		rssi_.insert( make_pair( 28.0,40.4 ) );
		rssi_.insert( make_pair( 32.5,18.90 ) );
		rssi_.insert( make_pair( 37.0,6.6 ) );
		rssi_.insert( make_pair( 41.5,12.65 ) );

		value_set::const_iterator iter;
		for(iter = rssi_.begin(); iter != rssi_.end(); ++iter)
			distance_estimate_rssi_dist_.insert( make_pair(	iter->second,iter->first) );


	}
	// ----------------------------------------------------------------------
	double
		CommonTaggedPolygonBase::
		get_distance(double measuredRssi)
		const throw()
	{
		std::cout << "CommonTaggedPolygonBase::measuredRssi " << measuredRssi << std::endl;

		bool isFirstRssi = true;

		value_set::const_reverse_iterator firstRssi;
		value_set::const_reverse_iterator secondRssi;

		for(secondRssi = distance_estimate_rssi_dist_.rbegin(); secondRssi != distance_estimate_rssi_dist_.rend(); ++secondRssi)
		{
			std::cout << "CommonTaggedPolygonBase:: rssi: "  << secondRssi->first << ", dist " << secondRssi->second << std::endl;

			if ( EQDOUBLE(secondRssi->first, measuredRssi)) {
				double r = secondRssi->second;
				std::cout << "CommonTaggedPolygonBase::measured rssi: " << measuredRssi << ", distance: " << r << "    exact match" << std::endl;
				return r;
			}

			if (isFirstRssi) {
				isFirstRssi = false;
				firstRssi = secondRssi;
				continue;
			}

			if (measuredRssi > secondRssi->first)
			{
				double firstDist = firstRssi->second;
				double secondDist = secondRssi->second;
				double steigung = ((secondDist - firstDist) / (secondRssi->first - firstRssi->first));
				double deltaX = measuredRssi - firstRssi->first;
				double r = steigung * deltaX + firstDist;
				std::cout << "CommonTaggedPolygonBase::measured rssi: " << measuredRssi << ", distance: " << r << "    interval[" << firstRssi->first << "," << secondRssi->first << "]"  << std::endl;
				return r;
			}

			firstRssi = secondRssi;
		}


		double r = firstRssi->second + (( upper_bound_ - firstRssi->second ) / firstRssi->first) * (firstRssi->first - measuredRssi) ;
		std::cout << "CommonTaggedPolygonBase::measured rssi: " << measuredRssi << ", distance: " << r << "    LAST[" << 0 << "," << firstRssi->first << "]"  << std::endl;
		return r;
	}

	// ----------------------------------------------------------------------
	double
		CommonTaggedPolygonBase::
		getRSSI(double distance)
		const throw()
	{
//		bool set_value = false;
		double old_rssi = std::numeric_limits<double>::max();
		double old_dist = 0.0;

		/*
		* Search for the interval the given distance fits in and afterwards
		* approximate the RSSI value by a linear function.
		*/
		for(value_set::const_iterator iter = rssi_.begin(); iter != rssi_.end(); ++iter)
		{
			assert( !EQDOUBLE( iter->first, old_dist ) );
			double dist =( old_rssi - (((old_rssi - iter->second)/(iter->first- old_dist))*(distance - old_dist)));
			if(distance >= old_dist && distance<iter->first)
			{
				return dist;
			}

			old_dist = iter->first;
			old_rssi = iter->second;

		}

		assert( !EQDOUBLE( upper_bound_, old_dist ) );
		double m = - old_rssi / (upper_bound_ - old_dist);
		double value = m * (distance - old_dist) + old_rssi;

		if(value < 0)
			value = 0;

		return value;
	}
}

#endif

