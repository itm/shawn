/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_ABSOLUTE_ERROR__DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_ABSOLUTE_ERROR__DISTANCE_ESTIMATE_H

#include "sys/node_distance_estimate.h"
#include "sys/misc/random/uniform_random_variable.h"
#include <string>

namespace shawn
{

   class AbsoluteErrorDistanceEstimate
	: public NodeDistanceEstimate
   {
	public:
		AbsoluteErrorDistanceEstimate(std::string name, double offset, double absolute_error);
		virtual ~AbsoluteErrorDistanceEstimate();

		virtual bool estimate_distance( const Node& source, const Node& target, double& result ) const throw();
		virtual std::string name( void ) const throw();
		virtual std::string description( void ) const throw();

	private:
		std::string name_;
		double offset_error_;
		double absolute_error_;
		UniformRandomVariable ur_;
	
	};

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/perfect_distance_estimate.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: perfect_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/
