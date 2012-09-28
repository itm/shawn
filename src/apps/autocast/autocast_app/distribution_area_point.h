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

#ifndef distributionareapoint_h
#define distributionareapoint_h

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include "distribution_area.h"

namespace autocast {
//	using namespace std;

	class DistributionAreaPoint: public DistributionArea
	{
	public:
		/// This is the constructor.
		DistributionAreaPoint();

		DistributionAreaPoint(double x, double y);

		/// returns true if a given point is inside the HDCs´ area
		virtual bool is_inside(double x, double y, double offset = 0) const;

		/// returns a pointer to a copy of this object
		virtual DistributionArea* copy() const;

		double X_;
		double Y_;

	};
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
