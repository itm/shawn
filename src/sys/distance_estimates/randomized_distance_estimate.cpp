/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/distance_estimates/randomized_distance_estimate.h"
#include "sys/node.h"
#include <sstream>
#include <limits>

namespace shawn
{

   RandomizedDistanceEstimate::
   RandomizedDistanceEstimate()
      : multiplier_       ( NULL ),
        offset_           ( NULL ),
        initialized_      ( false ),
        chop_lower_       ( std::numeric_limits<double>::min() ),
        resample_chopped_ ( false )
   {}
   // ----------------------------------------------------------------------
   RandomizedDistanceEstimate::
   ~RandomizedDistanceEstimate()
   {}
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   init( void )
      throw()
   {
      assert( !initialized_ );
      initialized_=true;

      std::ostringstream oss;
      if( multiplier_.is_not_null() )
         oss << multiplier_->name();
      else
         oss << "0";
      oss << "xDist+";
      if( offset_.is_not_null() )
         oss << offset_->name();
      else
         oss << "0";
      descr_=oss.str();
      if( name_.empty() ) name_=descr_;
   }
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   set_multiplier( const ConstRandomVariableHandle& m )
      throw()
   {
      assert( !initialized_ );
      multiplier_ = m;
   }
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   set_offset( const ConstRandomVariableHandle& o ) 
      throw()
   {
      assert( !initialized_ );
      offset_ = o;
   }
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   set_resample_chopped( bool r )
      throw()
   {
      assert( !initialized_ );
      resample_chopped_ = r;
   }
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   set_chop_lower( double c )
      throw()
   {
      assert( !initialized_ );
      chop_lower_ = c;
   }
   // ----------------------------------------------------------------------
   void
   RandomizedDistanceEstimate::
   set_name( const std::string& n )
      throw()
   {
      assert( !initialized_ );
      name_= n;
   }
   // ----------------------------------------------------------------------
   bool
   RandomizedDistanceEstimate::
   estimate_distance( const Node& source,
                      const Node& target,
                      double& result )
      const throw()
   {
	   int cnt = 0;
	   double mult = 0.0;

	   do {
		   result = 0.0;

		   if( multiplier_.is_not_null() ) 
		   {
			   mult = (*multiplier_);
			   result += (source.real_position()-target.real_position()).euclidean_norm() * mult;
		   }

		   if( offset_.is_not_null() )
			   result += *offset_;

		   cnt++;
		   if(cnt > 50000)
			   result = chop_lower_;

	   } while( resample_chopped_ && (result < chop_lower_));

	   if( result < chop_lower_ ) 
		   result=chop_lower_;

	   //std::cout << "estimate distance(): result " << result << ", mult " << mult << ", real dist: " << (source.real_position()-target.real_position()).euclidean_norm() << std::endl;

	   return true;
   }
   // ----------------------------------------------------------------------
   std::string
   RandomizedDistanceEstimate::
   name( void )
      const throw()
   {
      assert( initialized_ );
      return name_;
   }
   // ----------------------------------------------------------------------
   std::string
   RandomizedDistanceEstimate::
   description( void )
      const throw()
   {
      assert( initialized_ );
      return std::string("randomized distance estimate ")+descr_;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/distance_estimates/randomized_distance_estimate.cpp,v $
 * Version $Revision: 105 $
 * Date    $Date: 2007-11-13 08:53:43 +0100 (Tue, 13 Nov 2007) $
 *-----------------------------------------------------------------------
 * $Log: randomized_distance_estimate.cpp,v $
 *-----------------------------------------------------------------------*/
