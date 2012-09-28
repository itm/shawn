/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_LOCALIZATION

#include "apps/localization/math/localization_statistic.h"

#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

namespace localization
{

   LocalizationStatistic::
   LocalizationStatistic()
   {}
   // ----------------------------------------------------------------------
   LocalizationStatistic::
   LocalizationStatistic( const LocalizationStatistic& ls )
      : values_( ls.as_list() )
   {}
   // ----------------------------------------------------------------------
   LocalizationStatistic::
   ~LocalizationStatistic()
   {}
   // ----------------------------------------------------------------------
   void
   LocalizationStatistic::
   add( double value )
      throw()
   {
      values_.push_back( value );
   }
   // ----------------------------------------------------------------------
   LocalizationStatistic&
   LocalizationStatistic::
   operator+=( double value )
      throw()
   {
      this->add( value );

      return *this;
   }
   // ----------------------------------------------------------------------
   double
   LocalizationStatistic::
   mean( void )
      const throw()
   {
      if ( values_.size() == 0 ) return 0;

      double sum = 0;

      for ( ConstListIterator
               it = values_.begin();
               it != values_.end();
               ++it )
         sum += *it;

      return ( sum / values_.size() );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationStatistic::
   variance( void )
      const throw()
   {
      if ( values_.size() == 0 ) return 0;

      double var = 0;
      double avg = mean();

      for ( ConstListIterator
               it = values_.begin();
               it != values_.end();
               ++it )
         var += pow( *it - avg, 2 );

      return ( var / values_.size() );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationStatistic::
   std_dev( void )
      const throw()
   {
      return sqrt( variance() );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationStatistic::
   min( void )
      const throw()
   {
      if ( size() == 0 )
         return std::numeric_limits<double>::max();

      return *min_element( values_.begin(), values_.end() );
   }
   // ----------------------------------------------------------------------
   double
   LocalizationStatistic::
   max( void )
      const throw()
   {
      if ( size() == 0 )
         return std::numeric_limits<double>::min();

      return *max_element( values_.begin(), values_.end() );
   }
   // ----------------------------------------------------------------------
   size_t
   LocalizationStatistic::
   size( void )
      const throw()
   {
      return values_.size();
   }
   // ----------------------------------------------------------------------
   LocalizationStatistic::List
   LocalizationStatistic::
   as_list( void )
      const throw()
   {
      return values_;
   }
   // ----------------------------------------------------------------------
   void 
	LocalizationStatistic::
	clear( void )
	throw()
   {
	   values_.clear();
   }
   std::ostream&
   operator<<( std::ostream& os, const LocalizationStatistic& stat )
      throw()
   {
      for ( LocalizationStatistic::ConstListIterator
               it = stat.values_.begin();
               it != stat.values_.end();
               ++it )
         //os << *it << "\t";
		 os<<*it<< "; ";
		os<<std::endl;
      return os;
   }

}// namespace localization
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/localization/math/localization_statistic.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: localization_statistic.cpp,v $
 *-----------------------------------------------------------------------*/
