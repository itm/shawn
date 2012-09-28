/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_REFCNT_POINTER_H
#define __SHAWN_SYS_REFCNT_POINTER_H

#include <iostream>
#include <assert.h>

namespace shawn
{

   template<typename T>
   class RefcntPointer
   {
   public:
      typedef T TargetType;
      typedef RefcntPointer<TargetType> SelfType;
      typedef RefcntPointer<const TargetType> ConstSelfType;

      inline RefcntPointer()
         : refcnt_pointer_( NULL )
      {}
      inline RefcntPointer( TargetType* t )
         : refcnt_pointer_( NULL )
      { *this = t; }
      inline RefcntPointer( const SelfType& t )
         : refcnt_pointer_( NULL )
      { *this = t; }
      inline ~RefcntPointer()
      { *this = NULL; }

      inline bool is_null( void )
         const throw()
      { return refcnt_pointer_==NULL; }

      inline bool is_not_null( void )
         const throw()
      { return refcnt_pointer_!=NULL; }


      inline T& operator * ( void )
         const throw()
      { return *refcnt_pointer_; }
      inline T* operator -> ( void )
         const throw()
      { return refcnt_pointer_; }
      inline T* get( void )
         const throw()
      { return refcnt_pointer_; }

      inline SelfType& 
      operator= ( const SelfType& s )
         throw()
      {
         return *this = s.get();
      }

      inline operator ConstSelfType ( void ) const throw()
      { return ConstSelfType(refcnt_pointer_); }
   
      inline SelfType&
      operator= ( TargetType* t )
      {
         if( refcnt_pointer_ != NULL )
            {
               assert( refcnt_pointer_->reference_counter_>0 );
               if( --refcnt_pointer_->reference_counter_ == 0 )
                  delete refcnt_pointer_;
            }
         refcnt_pointer_=t;
         if( refcnt_pointer_ != NULL )
            ++refcnt_pointer_->reference_counter_;
         return *this;
      }

      inline friend std::ostream& operator << ( std::ostream& os, const SelfType& st )
         throw()
      { return os << st.refcnt_pointer_; }

      inline bool operator < ( const SelfType& s ) const throw()
      { return refcnt_pointer_<s.refcnt_pointer_; }
      inline bool operator <= ( const SelfType& s ) const throw()
      { return refcnt_pointer_<=s.refcnt_pointer_; }
      inline bool operator > ( const SelfType& s ) const throw()
      { return refcnt_pointer_>s.refcnt_pointer_; }
      inline bool operator >= ( const SelfType& s ) const throw()
      { return refcnt_pointer_>=s.refcnt_pointer_; }
      inline bool operator== ( const SelfType& s ) const throw()
      { return refcnt_pointer_==s.refcnt_pointer_; }
      inline bool operator!= ( const SelfType& s ) const throw()
      { return refcnt_pointer_!=s.refcnt_pointer_; }

      inline bool operator < ( const T* s ) const throw()
      { return refcnt_pointer_<s; }
      inline bool operator <= ( const T* s ) const throw()
      { return refcnt_pointer_<=s; }
      inline bool operator > ( const T* s ) const throw()
      { return refcnt_pointer_>s; }
      inline bool operator >= ( const T* s ) const throw()
      { return refcnt_pointer_>=s; }
      inline bool operator== ( const T* s ) const throw()
      { return refcnt_pointer_==s; }
      inline bool operator!= ( const T* s ) const throw()
      { return refcnt_pointer_!=s; }

   private:
      T* refcnt_pointer_;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/refcnt_pointer.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: refcnt_pointer.h,v $
 *-----------------------------------------------------------------------*/
