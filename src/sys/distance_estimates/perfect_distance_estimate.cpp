/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/perfect_distance_estimate.h"
#include "sys/node.h"


namespace shawn
{

   PerfectDistanceEstimate::
   PerfectDistanceEstimate()
   {}
   // ----------------------------------------------------------------------
   PerfectDistanceEstimate::
   ~PerfectDistanceEstimate()
   {}
   // ----------------------------------------------------------------------
   bool
   PerfectDistanceEstimate::
   estimate_distance( const Node& source,
                      const Node& target,
                      double& result )
      const throw()
   {
      result = (source.real_position()-target.real_position()).euclidean_norm();
      return true;
   }
   // ----------------------------------------------------------------------
   std::string
   PerfectDistanceEstimate::
   name( void )
      const throw()
   {
      return std::string("perfect_estimate");
   }
   // ----------------------------------------------------------------------
   std::string
   PerfectDistanceEstimate::
   description( void )
      const throw()
   {
      return std::string("perfect estimate of node distances");
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/perfect_distance_estimate.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: perfect_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
