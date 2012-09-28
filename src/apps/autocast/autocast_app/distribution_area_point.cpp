/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2007 by AutoNomos (www.auto-nomos.de)                **
 ** This part of Shawn is free software; you can redistribute it and/or**
 ** modify it under the terms of the BSD License. Refer to the		   **
 ** shawn-licence.txt file in the root of the Shawn source tree for	   **
 ** further details.												   **
 **                                                                    **
 ** \author Axel Wegener <wegener@itm.uni-luebeck.de>				   **
 **                                                                    **
 ************************************************************************/

#include "distribution_area_point.h"
#include "math.h"
#include "float.h"
#include <iostream>
#ifdef ENABLE_AUTOCAST

namespace autocast {

	#define EPSILON .00001
	#define EQDOUBLE(a,b) (fabs((a)-(b))<EPSILON)

	// Constructors
	DistributionAreaPoint::DistributionAreaPoint() : X_(0),
													 Y_(0)
	{}

	DistributionAreaPoint::DistributionAreaPoint(double x, double y) : X_(x),
																	   Y_(y)
	{}

	bool DistributionAreaPoint::is_inside(double x, double y, double offset) const
	{
		// Radius
		if (offset > 0) return ( (x-X_)*(x-X_) + (y-Y_)*(y-Y_) <= offset*offset );
		// the dots have to be very near to each other
		return ( EQDOUBLE(x,X_) && EQDOUBLE(y,Y_) );
	}

	DistributionArea* DistributionAreaPoint::copy() const
	{
		return new DistributionAreaPoint(X_, Y_);
	}
}
#endif


/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
