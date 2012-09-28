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

#ifndef ns_autocastneighborhood_h
#define ns_autocastneighborhood_h

#include "_apps_enable_cmake.h"
#ifdef ENABLE_AUTOCAST

#include <map>
#include <vector>

#include "sys/node.h"

namespace autocast {
	

	class Neighborhood
	{
	public:
		/// Construct a neighborhood table and sets the standard timeout for leaving nehighbors
		Neighborhood(double);

		/// Set owner
		void set_owner(const shawn::Node&) throw();
		/// Get number of neighbors heard at least timeOut_ seconds before
		int neighborhood_size();

		/// Get number of neighbors heard at least duration seconds before
		int neighborhood_size(double duration);

		/// Get number of new neighbors that appear in the last duration seconds
		int get_neighborhood_changed(double duration);

		/// Mark a node as seen in this moment
		void append_to_neighborhood(int nodeId);

		/// Mark a node as seen in this moment
		void append_to_neighborhood(int nodeId, double nextUpdateTime);

		void clear();

	private:
		/// Shring the neighborhood table and throw away entries older than timeOut_ seconds
		void shrink_neighborhood_table();

		const shawn::Node* owner_;

		struct NeighborInfo
		{
			double first_heard_time;
			double last_heard_time;
			double next_update_time;
		};

		/// This map stores the neighborhood information
		std::map<int, NeighborInfo > neighborhood_;

		/// A node that is not heard for more than neighborhood_.second + timeOut_ seconds is no neighbor anymore
		double time_out_;
	};
}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $HeadURL$
 * Version $LastChangedRevision$
 * Date    $LastChangedDate$
 *-----------------------------------------------------------------------*/
