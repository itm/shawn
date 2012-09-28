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

#ifndef distributionarea_h
#define distributionarea_h

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

namespace autocast {

	class DistributionArea
	{
	public:

		/// This is the constructor.
		DistributionArea();

		/// returns true if a given point is inside the HDCs´ area
		virtual bool is_inside(double x, double y, double offset = 0) const;

		/// returns a pointer to a copy of this object
		virtual DistributionArea* copy() const;

	private:
		DistributionArea(const DistributionArea&) {} //kein Copy-Constructor!

	};

}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
