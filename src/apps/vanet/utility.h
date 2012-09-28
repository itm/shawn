/**
 * @file utility.h
 * Defines some helper functions and classes.
 *
 * @author Walter Bamberger
 *
 * $Revision: 632 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/utility.h $
 * $Id: utility.h 632 2011-11-11 08:13:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_UTILITY_H
#define __SHAWN_APPS_VANET_UTILITY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/vec.h"
#include <algorithm>
#include <exception>
#include <numeric>
#include <string>

namespace vanet
{

template<class C>
   typename C::value_type
   mean( const C& container )
   {
      typename C::value_type sum(0);

      if ( container.size() == 0 )
         return sum;

      sum = std::accumulate(container.begin(), container.end(), sum);
      return sum / container.size();
   }

template<class C>
   typename C::value_type
   variance( const C& container, const typename C::value_type& mean )
   {
      typename C::value_type sum(0);

      if ( container.size() == 0 )
         return sum;

      for ( typename C::const_iterator it = container.begin();
               it != container.end(); ++it )
         sum += (*it - mean) * (*it - mean);

      return sum / container.size();
   }

template<class _T1, class _T2, class _T3>
   struct triplet
   {
      typedef _T1 first_type;
      typedef _T2 second_type;
      typedef _T3 third_type;

      _T1 first;
      _T2 second;
      _T3 third;

      triplet()
      {
      }

      triplet( const _T1& t1, const _T2& t2, const _T3 t3 ) :
               first(t1), second(t2), third(t3)
      {
      }

      template<class _U1, class _U2, class _U3>
         triplet( const triplet<_U1, _U2, _U3>& t ) :
                  first(t.first), second(t.second), third(t.third)
         {
         }
   };

// ----------------------------------------------------------------------

template<class _T1, class _T2, class _T3>
   inline triplet<_T1, _T2, _T3>
   make_triplet( const _T1& t1, const _T2& t2, const _T3& t3 )
   {
      return triplet<_T1, _T2, _T3>(t1, t2, t3);
   }

// ----------------------------------------------------------------------

struct VecLess : public std::binary_function<shawn::Vec, shawn::Vec, bool>
{
   bool
   operator()( const shawn::Vec& v1, const shawn::Vec& v2 ) const
   {
      double distance = (v1 - v2).euclidean_norm();
      if ( distance < 1.0 )
      {
         return false;
      }
      else if ( v1.euclidean_norm() < v2.euclidean_norm() || v1.x() < v2.x()
               || v1.y() < v2.y() || v1.z() < v2.z() )
      {
         return true;
      }
      return false;
   }
};

// ----------------------------------------------------------------------

template<typename _Iterator, typename _Tp>
   void
   normalizeSum( _Iterator begin, _Iterator end, _Tp targetSum )
   {
      _Tp actualSum = std::accumulate(begin, end, _Tp(0));
      std::transform(begin, end, begin,
               std::bind2nd(std::multiplies<_Tp>(), targetSum / actualSum));
   }

// ----------------------------------------------------------------------

template<typename _Iterator>
   typename std::iterator_traits<_Iterator>::difference_type
   maxElementNumber( _Iterator begin, _Iterator end )
   {
      return std::max_element(begin, end) - begin;
   }

// ----------------------------------------------------------------------

/**
 * Indicates an error that would lead to a wrong simulation result. As a
 * consequence, the simulation must be stopped in case of such an error. No
 * component may catch the exception except the one that is controlling the
 * simulation. So the component that throws this error must be certain that the
 * simulation cannot recover from this error.
 */
class SimulationFailed : public std::exception
{
public:
   /**
    * Creates the exception.
    *
    * @param msg
    *              A description of the causes of the current error.
    * @throw None
    */
   explicit
   SimulationFailed( const std::string& msg ) :
            msg_(msg)
   {
   }

   /**
    * Destroys the exception.
    *
    * @throw None
    */
   virtual
   ~SimulationFailed() throw ()
   {
   }

   /**
    * Provides a description of the causes of the reported error.
    *
    * @return The description of the error (was given in the constructor)
    */
   virtual const char*
   what() const throw ()
   {
      return msg_.c_str();
   }

private:
   std::string msg_;

};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_UTILITY_H */
