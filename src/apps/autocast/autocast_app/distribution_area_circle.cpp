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

#include "distribution_area_circle.h"
#include <iostream>
#ifdef ENABLE_AUTOCAST

namespace autocast {

	// Constructors
	DistributionAreaCircle::DistributionAreaCircle() : DistributionArea(),
													   X_(0),
													   Y_(0),
													   radius_(0)
	{}

	DistributionAreaCircle::DistributionAreaCircle(double x, double y, double radius) : DistributionArea(),
																						X_(x),
																						Y_(y),
																						radius_(radius)
	{}

	bool DistributionAreaCircle::is_inside(double x, double y, double offset) const
	{
		// Compute distance to center
		return ( (x-X_)*(x-X_) + (y-Y_)*(y-Y_) <= (radius_+offset)*(radius_+offset) );
	}

	DistributionArea* DistributionAreaCircle::copy() const
	{
		return new DistributionAreaCircle(X_, Y_, radius_);
	}
}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
