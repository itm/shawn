/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_WEIBULL_RANDOM_VARIABLE_H
#define __SHAWN_SYS_MISC_RANDOM_WEIBULL_RANDOM_VARIABLE_H

#include "sys/misc/random/random_variable.h"

#include <stdexcept>
#include <cmath>


namespace shawn{

   class WeibullRandomVariable
   : public RandomVariable

   {
   public:

      //static const std::string TYPE_INDICATOR_PREFIX;
      WeibullRandomVariable();
      virtual ~WeibullRandomVariable();

      virtual void set_lower_bound( double ) throw();
      virtual void set_upper_bound( double ) throw();
      virtual void set_lower_bound_inclusive( bool ) throw();
      virtual void set_upper_bound_inclusive( bool ) throw();
      virtual void set_alpha( double ) throw (std::runtime_error) ;
      virtual void set_beta( double ) throw (std::runtime_error);

      virtual void set_name( const std::string& ) throw();
      virtual void init( void ) throw( std::runtime_error );



      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual operator double( void ) const throw();

      virtual double mean( void ) const throw();
      virtual double variance( void ) const throw();
      virtual double standard_deviation( void ) const throw();

      virtual double gamma_func( double x,double d=0.2,double end=100) const throw()
       {
                double temp,result;
                result = 0;

               for(double i = d; i <= end;i=i+d)
               {
                //double e = 2.7183;
                 temp = pow(i,x-1) *exp(-i);
                }

                 return result * d;


        }


   private:
      bool initialized_;
      double lower_, upper_;
      bool lower_incl_, upper_incl_;

      std::string name_;
      std::string dist_descr_;
      //double mean;
      //double std_dev_;
      double alpha_;
      double beta_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/weibull_random_variable.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: weibull_random_variable.h,v $
 *-----------------------------------------------------------------------*/
