/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/random/uniform_random_variable.h"
#include "sys/misc/random/basic_random.h"
#include <sstream>
#include <math.h>

namespace shawn
{
   const std::string UniformRandomVariable::TYPE_INDICATOR_PREFIX( "uni" );
   // ----------------------------------------------------------------------
   UniformRandomVariable::
   UniformRandomVariable()
      : initialized_    ( false ),
        name_           ( "" ),
        dist_descr_     ( "" ),
        lower_          ( 0.0 ),
        upper_          ( 1.0 ),
        lower_incl_     ( true ),
        upper_incl_     ( false ),
        base_generator_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   UniformRandomVariable::
   ~UniformRandomVariable()
   {}
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   set_lower_bound( double lb )
      throw()
   {
      assert( !initialized_ );
      lower_ = lb;
   }
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   set_upper_bound( double ub )
      throw()
   {
      assert( !initialized_ );
      upper_ = ub;
   }
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   set_lower_bound_inclusive( bool lbi )
      throw()
   {
      assert( !initialized_ );
      lower_incl_ = lbi;
   }
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   set_upper_bound_inclusive( bool ubi )
      throw()
   {
      assert( !initialized_ );
      upper_incl_ = ubi;
   }
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   set_name( const std::string& n )
      throw()
   {
      assert( !initialized_ );
      name_ = n;
   }
   // ----------------------------------------------------------------------
   void
   UniformRandomVariable::
   init( void )
      throw( std::runtime_error )
   {
      assert( !initialized_ );
      initialized_ = true;
      std::ostringstream oss;
      oss << TYPE_INDICATOR_PREFIX
          << (lower_incl_ ? '[' : '(')
          << lower_
          << ';'
          << upper_
          << (upper_incl_ ? ']' : ')');
      dist_descr_ = oss.str();

      if( name_.empty() )
         name_ = dist_descr_;
      
      bool okay;
      if( lower_incl_ )
         if( upper_incl_ )
            {
               okay=(lower_ <= upper_);
               base_generator_ = &uniform_random_0i_1i;
            }
         else
            {
               okay=(lower_<upper_);
               base_generator_ = &uniform_random_0i_1e;
            }
      else // !lower_incl_
         if( upper_incl_ )
            {
               okay=(lower_<upper_);
               base_generator_ = &uniform_random_0e_1i;
            }
         else
            {
               okay=(lower_<upper_);
               base_generator_ = &uniform_random_0e_1e;
            }

      if( !okay )
         {
            name_ += "_defunct";
            throw std::runtime_error(std::string("distribution ") +
                                     dist_descr_ +
                                     std::string(" is impossible.") );
         }
   }



   // ----------------------------------------------------------------------
   std::string
   UniformRandomVariable::
   name( void )
      const throw()
   {
      return name_;
   }
   // ----------------------------------------------------------------------
   std::string
   UniformRandomVariable::
   description( void )
      const throw()
   {
      return std::string("uniform distribution ")+dist_descr_;
   }

   // ----------------------------------------------------------------------
   UniformRandomVariable::
   operator double( void )
      const throw()
   {
      assert( initialized_ );
      assert( base_generator_ != NULL );
      return lower_ + (upper_-lower_)*((*base_generator_)());
   }
   // ----------------------------------------------------------------------
   double 
   UniformRandomVariable::
   mean( void )
      const throw()
   {
      return (lower_ + upper_) / 2.0;
   }
   // ----------------------------------------------------------------------
   double
   UniformRandomVariable::
   variance( void )
      const throw()
   {
      return (upper_-lower_)*(upper_-lower_) / 12.0;
   }
   // ----------------------------------------------------------------------
   double
   UniformRandomVariable::
   standard_deviation( void )
      const throw()
   {
      return sqrt(variance());
   }

   // ----------------------------------------------------------------------
   double
   UniformRandomVariable::
   lower_bound( void )
      const throw()
   {
      return lower_;
   }
   // ----------------------------------------------------------------------
   double
   UniformRandomVariable::
   upper_bound( void )
      const throw()
   {
      return upper_;
   }
   // ----------------------------------------------------------------------
   bool
   UniformRandomVariable::
   is_lower_bound_inclusive( void )
      const throw()
   {
      return lower_incl_;
   }
   // ----------------------------------------------------------------------
   bool
   UniformRandomVariable::
   is_upper_bound_inclusive( void )
      const throw()
   {
      return upper_incl_;
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/uniform_random_variable.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: uniform_random_variable.cpp,v $
 *-----------------------------------------------------------------------*/
