/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_LOCALIZATION_MATH_STATISTIC_H
#define __SHAWN_APPS_LOCALIZATION_MATH_STATISTIC_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include <iostream>
#include <list>


namespace localization
{

   /// Class that provides statistic information.
   /** This class provides statistic information such like mean, variance and
    *  standard deviation.
    */
   class LocalizationStatistic
   {

   public:

      typedef std::list<double> List;
      typedef std::list<double>::const_iterator ConstListIterator;

      ///@name construction / destruction
      ///@{
      ///
      LocalizationStatistic();
      LocalizationStatistic( const LocalizationStatistic& );
      virtual ~LocalizationStatistic();
      ///@}


      ///@name adding values
      ///@{
      /** Add value to internal representation.
       *  \sa operator+=()
       */
      virtual void add( double ) throw();
      /** Add value to internal representation.
       *  \sa add()
       */
      virtual LocalizationStatistic& operator+=( double ) throw();
      ///@}


      ///@name statistic methods
      ///@{
      /** This method gives the mean of all values with:
       *  \f[ \mu = \frac{1}{N}\sum^N_{i=1}x_i \f]
       *
       *  \return average of all values
       *  \sa variance(), std_dev()
       */
      virtual double mean( void ) const throw();
      /** This method gives the variance of all values with:
       *  \f[ \sigma^2 = \frac{1}{N}\sum^N_{i=1}{(x_i - \mu)^2} \f]
       *
       *  \return variance of all values
       *  \sa mean(), std_dev()
       */
      virtual double variance( void ) const throw();
      /** This method gives the standard deviation of all values with:
       *  \f[ \sigma = \sqrt{\sigma^2} \f]
       *
       *  \return standard deviation of all values
       *  \sa mean(), variance()
       */
      virtual double std_dev( void ) const throw();
      /** \return Minimum of all values
       */
      virtual double min( void ) const throw();
      /** \return Maximum of all values
       */
      virtual double max( void ) const throw();
      ///@}


      ///@name some internal info
      ///@{
      /** \return Number of values
       */
      virtual size_t size( void ) const throw();
      /** \return Values as list representation
       */
      virtual List as_list( void ) const throw();
      ///@}
		virtual void clear( void ) throw();

      /** Print values to given ostream.
       */
      friend std::ostream& operator<<(
            std::ostream&, const LocalizationStatistic& )
         throw();


   private:

      List values_;

   };

}// namespace localization
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_statistic.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_statistic.h,v $
 *-----------------------------------------------------------------------*/
