/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/uniform_random_variable.h"
#include "sys/distance_estimates/absolute_error_distance_estimate.h"
#include "sys/node.h"
#include <iostream>

namespace shawn
{

	// ----------------------------------------------------------------------
	AbsoluteErrorDistanceEstimate::
		AbsoluteErrorDistanceEstimate(std::string name, double offset, double absolute_error)
		: name_(name),
		  offset_error_(offset),
		  absolute_error_(absolute_error)
		{
			double absolute_error_half = absolute_error / 2.0;
			ur_.set_lower_bound(- absolute_error_half);
			ur_.set_upper_bound(+ absolute_error_half);
			ur_.set_lower_bound_inclusive(true);
			ur_.set_upper_bound_inclusive(true);
			ur_.init();
		}

	// ----------------------------------------------------------------------
	AbsoluteErrorDistanceEstimate::
		~AbsoluteErrorDistanceEstimate()
		{}

	// ----------------------------------------------------------------------
	bool
		AbsoluteErrorDistanceEstimate::
		estimate_distance( const Node& source, const Node& target, double& result ) 
		const throw()
	{
		result = offset_error_ + double(ur_) + ( source.real_position() - target.real_position() ).euclidean_norm();
		return true;
	}
	
	// ----------------------------------------------------------------------
	std::string
		AbsoluteErrorDistanceEstimate::
		name( void ) 
		const throw()
	{
		return name_;
	}
	
	// ----------------------------------------------------------------------
	std::string
		AbsoluteErrorDistanceEstimate::
		description( void )
		const throw()
	{
		return std::string("distance estimate to other nodes with an offset and an absolute error");
	}


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/perfect_distance_estimate.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: perfect_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
