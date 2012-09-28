/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <sstream>
#include <cmath>
#include <math.h>

#include "sys/misc/random/normal_random_variable.h"
#include "sys/misc/random/basic_random.h"

using namespace std;

namespace shawn
{
   const std::string NormalRandomVariable::TYPE_INDICATOR_PREFIX( "normal" );
   // ----------------------------------------------------------------------
   NormalRandomVariable::
   NormalRandomVariable()
      : initialized_    ( false ),
        name_           ( "" ),
        dist_descr_     ( "" ),
        mean_           ( 0.0 ),
        std_dev_        ( 1.0 )
   {}
   // ----------------------------------------------------------------------
   NormalRandomVariable::
   ~NormalRandomVariable()
   {}
   // ----------------------------------------------------------------------
   void
   NormalRandomVariable::
   set_mean( double mu )
      throw()
   {
      assert( !initialized_ );
      mean_ = mu;
   }
   // ----------------------------------------------------------------------
   void
   NormalRandomVariable::
   set_variance( double v )
      throw( std::runtime_error )
   {
      assert( !initialized_ );
      if( v<0 )
         throw std::runtime_error(std::string("variance must be nonnegative in construction of normal distribution"));
      else
         std_dev_=sqrt( v );
   }
   // ----------------------------------------------------------------------
   void
   NormalRandomVariable::
   set_standard_deviation( double s )
      throw( std::runtime_error )
   {
      assert( !initialized_ );
      if( s<0 )
         throw std::runtime_error(std::string("variance must be nonnegative in construction of normal distribution"));
      else
         std_dev_=s;
   }
   // ----------------------------------------------------------------------
   void
   NormalRandomVariable::
   set_name( const std::string& n )
      throw()
   {
      assert( !initialized_ );
      name_ = n;
   }
   // ----------------------------------------------------------------------
   void
   NormalRandomVariable::
   init( void )
      throw( std::runtime_error )
   {
      assert( !initialized_ );
      initialized_ = true;

      std::ostringstream oss;
      oss << TYPE_INDICATOR_PREFIX
          << '('
          << mean_
          << ';'
          << (std_dev_*std_dev_)
          << ')';
      dist_descr_ = oss.str();

      if( name_.empty() )
         name_ = dist_descr_;
   }



   // ----------------------------------------------------------------------
   std::string
   NormalRandomVariable::
   name( void )
      const throw()
   {
      return name_;
   }
   // ----------------------------------------------------------------------
   std::string
   NormalRandomVariable::
   description( void )
      const throw()
   {
      return std::string("normal distribution ")+dist_descr_;
   }

   // ----------------------------------------------------------------------
#ifdef WIN32
#define M_PI       3.14159265358979323846 //MS VC is not able to resolve this
#endif
   NormalRandomVariable::
   operator double( void )
      const throw()
   {
      assert( initialized_ );
      return
         mean_ +
         std_dev_
         * sqrt( -2.0 * log(uniform_random_0e_1i()) )
         * cos( 2.0 * M_PI * uniform_random_0e_1i());
   }
   // ----------------------------------------------------------------------
   double 
   NormalRandomVariable::
   mean( void )
      const throw()
   {
      return mean_;
   }
   // ----------------------------------------------------------------------
   double
   NormalRandomVariable::
   variance( void )
      const throw()
   {
      return std_dev_*std_dev_;
   }
   // ----------------------------------------------------------------------
   double
   NormalRandomVariable::
   standard_deviation( void )
      const throw()
   {
      return std_dev_;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/normal_random_variable.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: normal_random_variable.cpp,v $
 *-----------------------------------------------------------------------*/
