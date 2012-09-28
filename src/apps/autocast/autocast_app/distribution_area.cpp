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

#include "distribution_area.h"
#include <iostream>
#ifdef ENABLE_AUTOCAST

namespace autocast {

	DistributionArea::DistributionArea() 
	{
	}

	bool DistributionArea::is_inside(double x, double y, double offset) const
	{
		return false;
	}

	DistributionArea* DistributionArea::copy() const
	{
		return new DistributionArea();
	}
}
#endif

/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/

