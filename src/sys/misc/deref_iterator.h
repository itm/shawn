/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_DEREF_ITERATOR_H
#define __SHAWN_SYS_MISC_DEREF_ITERATOR_H

namespace shawn
{

   /// Convenience iterator wrapper to auto-dereference pointers
   /** Iterator wrapper that dereferences pointers at access.
    *
    *  Consider there is a vector in which Thing* pointers are stored
    *  for some reason, but the pointers are all non-Null
    *  \code
    *  vector<Thing*> things;
    *  \endcode
    *
    *  ...then the vector can be treated like a vector<Thing>:
    *  \code
    *  for( DerefIterator< Thing, vector<Thing>::iterator > it=things.begin();
    *       it != things.end();
    *       ++it )
    *    {
    *       Thing& t = *it;
    *       it->method();
    *    }
    *  \endcode
    */
   template< typename VAL,
             typename IT >
   class DerefIterator
      : public IT
   {
   public:
      ///@name types
      ///@{
      ///      
      typedef IT base_type;
      ///      
      typedef VAL value_type;
      ///      
      typedef DerefIterator<VAL, IT> self_type;
      ///@}

      ///@name construction
      ///@{
      ///
      DerefIterator(){}
      ///
      DerefIterator( const self_type& st )
         : base_type(st)
      {}
      ///
      DerefIterator( const base_type& bt )
         : base_type(bt)
      {}
      ///@}


      ///@name operators
      ///@{
      ///
      inline self_type& operator= ( const self_type& st )
      { base_type::operator=(st); return *this; }
      ///
      inline value_type& operator* ( void ) const
      {
         return *(base_type::operator*()); 
      }
      ///
      inline value_type* operator-> ( void ) const
      { 
         return *(base_type::operator->());
      }
      ///@}

   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/deref_iterator.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: deref_iterator.h,v $
 *-----------------------------------------------------------------------*/
