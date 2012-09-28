/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/


#include <limits>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <math.h>
#include "sys/misc/random/weibull_random_variable.h"


namespace shawn{


    WeibullRandomVariable::
        WeibullRandomVariable()
        : initialized_    ( false ),
        //lower_          ( std::numeric_limits<double>::min() ),
        //upper_          ( std::numeric_limits<double>::max() ),
        lower_          ( 0 ),
        upper_          ( 1 ),
        lower_incl_     ( true ),
        upper_incl_     ( false ),
        name_           ( "" ),
        dist_descr_     ( "" ),
        alpha_          ( 3 ),
        beta_        ( 5)
    {}

    WeibullRandomVariable::
        ~WeibullRandomVariable()
    {}

    void
        WeibullRandomVariable::
        set_lower_bound( double lb )
        throw()
    {
        assert( !initialized_ );
        lower_ = lb;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        set_upper_bound( double ub )
        throw()
    {
        assert( !initialized_ );
        upper_ = ub;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        set_lower_bound_inclusive( bool lbi )
        throw()
    {
        assert( !initialized_ );
        lower_incl_ = lbi;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        set_upper_bound_inclusive( bool ubi )
        throw()
    {
        assert( !initialized_ );
        upper_incl_ = ubi;
    }


    void
        WeibullRandomVariable::
        set_alpha( double alpha )
        throw( std::runtime_error )
    {
        assert( !initialized_ );
        if( alpha<0 )
            throw std::runtime_error(std::string("alpha must be positive in construction of weibull distribution"));
        else
            alpha_= alpha ;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        set_beta( double beta )
        throw( std::runtime_error )
    {
        assert( !initialized_ );
        if( beta<0 )
            throw std::runtime_error(std::string("beta must be positive in construction of weibull distribution"));
        else
            beta_= beta ;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        set_name( const std::string& n )
        throw()
    {
        assert( !initialized_ );
        name_ = n;
    }
    // ----------------------------------------------------------------------
    void
        WeibullRandomVariable::
        init( void )
        throw( std::runtime_error )
    {
        assert( !initialized_ );
        initialized_ = true;

        //std::ostringstream oss;
        //std::cout << "weibull"
        //  << '('
        //<< alpha_
        //<< ';'
        //<< (beta_)
        //<< ')';
        //dist_descr_ = oss.str();

        if( name_.empty() )
            name_ = dist_descr_;
    }



    // ----------------------------------------------------------------------
    std::string
        WeibullRandomVariable::
        name( void )
        const throw()
    {
        return name_;
    }
    // ----------------------------------------------------------------------
    std::string
        WeibullRandomVariable::
        description( void )
        const throw()
    {
        return std::string("weibull distribution ")+dist_descr_;
    }

    // ----------------------------------------------------------------------

    WeibullRandomVariable::
        operator double( void )
        const throw()
    {
        double result;
        assert( initialized_ );
        bool done;

        done = 0;
        while (!done) {
            double temp = (double)(rand()) / (double)(RAND_MAX);

            result = pow ( (log (1-temp)/  -alpha_),(1/beta_));

            switch(lower_incl_ )
            {
            case true : switch(upper_incl_ ) {case true : if(lower_<= result && result <=upper_) done = 1;break;
            case false : if(lower_<= result && result <upper_) done = 1;break;} break;

            case false : switch(upper_incl_ ) {case true : if(lower_<result && result <=upper_) done = 1;break;
            case false : if(lower_ < result && result <upper_) done = 1;break;} break;
            }


        }
        return result;
    }

    double WeibullRandomVariable::mean( void ) const throw()
    {
        return pow(alpha_ ,-1 / beta_)*gamma_func(1/beta_ +1);
    }

    double WeibullRandomVariable::variance( void ) const throw()
    {
        return  pow(alpha_,-2 / beta_) * (gamma_func(2/beta_+1)-gamma_func(1/beta_ +1) * gamma_func(1/beta_ +1));
    }

    double WeibullRandomVariable::standard_deviation( void ) const throw()
    {
        return sqrt(variance());
    }

}
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/misc/random/weibull_random_variable.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: weibull_random_variable.cpp,v $
 *-----------------------------------------------------------------------*/
