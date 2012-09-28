/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_SYS_NODE_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_NODE_DISTANCE_ESTIMATE_H

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/util/defutils.h"

namespace shawn
{
	class Node;

	DECLARE_HANDLES(NodeDistanceEstimate);

	/** Base class for all classes that estimated distances between two nodes. These are kept in the
	  * distance_estimate_keeper and applications may retrieve these distance estimate instances
	  * using the following example code:
	  * <code>
	  *		std::string dist_est_name = se.required_string_param("myapp_dist_est");
      *		shawn::ConstNodeDistanceEstimateHandle dist_est_ = 
	  *			owner().world().simulation_controller().distance_estimate_keeper().find(dist_est_name);
	  * </code>
	  */
	class NodeDistanceEstimate
		: public KeeperManaged
	{
	public:
		///Does nothing, required because of the virtual methods
		virtual ~NodeDistanceEstimate();

		/** Estimates the distance between two nodes source and target.
		  * @param source The source node for the estimation
		  * @param target The destination node for the estimation
		  * @param result The result of the estimation will be placed here ONLY IF true is returned
		  * @return true if a distance could be estimated between the two nodes, false otherwise
		  */
		virtual bool estimate_distance( const Node& source, const Node& target, double& result ) const throw() = 0;
	};

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/node_distance_estimate.h,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: node_distance_estimate.h,v $
*-----------------------------------------------------------------------*/
