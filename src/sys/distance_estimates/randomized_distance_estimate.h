/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_DISTANCE_ESTIMATES_RANDOMIZED_DISTANCE_ESTIMATE_H
#define __SHAWN_SYS_DISTANCE_ESTIMATES_RANDOMIZED_DISTANCE_ESTIMATE_H

#include "sys/node_distance_estimate.h"
#include "sys/misc/random/random_variable.h"

namespace shawn
{

   class RandomizedDistanceEstimate
      : public NodeDistanceEstimate
   {
   public:
      RandomizedDistanceEstimate();
      virtual ~RandomizedDistanceEstimate();
      virtual void set_multiplier( const ConstRandomVariableHandle& ) throw();
      virtual void set_offset( const ConstRandomVariableHandle& ) throw();
      virtual void set_chop_lower( double ) throw();
      virtual void set_resample_chopped( bool ) throw();
      virtual void set_name( const std::string& ) throw();
      virtual void init( void ) throw();

      virtual bool estimate_distance( const Node& source,
                                      const Node& target,
                                      double& result )
         const throw();
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();


   private:
      ConstRandomVariableHandle multiplier_;
      ConstRandomVariableHandle offset_;
      bool initialized_;
      std::string name_;
      std::string descr_;
      double chop_lower_;
      bool resample_chopped_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/randomized_distance_estimate.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: randomized_distance_estimate.h,v $
 *-----------------------------------------------------------------------*/
