/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_RANDOM_NORMAL_RANDOM_VARIABLE_H
#define __SHAWN_SYS_MISC_RANDOM_NORMAL_RANDOM_VARIABLE_H

#include "sys/misc/random/random_variable.h"
#include <stdexcept>


namespace shawn
{

   class NormalRandomVariable
      : public RandomVariable
   {
   public:
      static const std::string TYPE_INDICATOR_PREFIX;
      NormalRandomVariable();
      virtual ~NormalRandomVariable();

      virtual void set_mean( double ) throw();
      virtual void set_variance( double ) throw( std::runtime_error );
      virtual void set_standard_deviation( double ) throw( std::runtime_error );
      virtual void set_name( const std::string& ) throw();
      virtual void init( void ) throw( std::runtime_error );



      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual operator double( void ) const throw();
      virtual double mean( void ) const throw();
      virtual double variance( void ) const throw();
      virtual double standard_deviation( void ) const throw();

   private:
      bool initialized_;

      std::string name_;
      std::string dist_descr_;
      double mean_;
      double std_dev_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/random/normal_random_variable.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: normal_random_variable.h,v $
 *-----------------------------------------------------------------------*/
