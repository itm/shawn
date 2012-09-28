/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_SKIPNULL_ITERATOR_H
#define __SHAWN_SYS_MISC_SKIPNULL_ITERATOR_H

namespace shawn
{

   template< typename VAL,
             typename C,
             typename IT >
   class SkipNullIterator
      : public IT
   {
   public:
      ///@name types
      ///@{  
      ///      
      typedef VAL value_type;
      ///      
      typedef C container_type;
      ///      
      typedef IT base_type;
      ///      
      typedef SkipNullIterator<VAL, C, IT> self_type;
      ///@}
  
      ///@name construction 
      ///@{
      ///
      SkipNullIterator( const base_type& bt,
                        const container_type& c )
         : base_type( bt ),
            c_( &c )
      {
         while ( *this != container().end() &&
            base_type::operator*() == NULL )
            base_type::operator++();
      }
      ///
      SkipNullIterator( const self_type& st )
         : base_type(st),
            c_( &st.container() )
      {}
      ///
      inline self_type& operator= ( const self_type& st )
      {
         c_ = &st.container();
         base_type::operator=(st);
         return *this;
      }
      ///@}

  
      ///@name work on container
      ///@{
      ///
      inline const container_type& container() const
      { return *c_; }
      ///@}

      ///@name operators
      ///@{      
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
      ///
      inline self_type& operator[] (int idx)
      {
         // go to first element in container
         while ( *this != container().begin() )
            base_type::operator--();
         // then go to the first element in container that is != NULL
         while ( *this != container().end() &&
                  base_type::operator*() == NULL )
            base_type::operator++();
         // and finally move to the wanted position
         return self_type::operator+=(idx);
      }
      ///
      inline self_type& operator++ ()
      {
         base_type::operator++();
         while ( *this != container().end() &&
            base_type::operator*() == NULL )
         base_type::operator++();

         return *this;
      }
      ///
      inline self_type operator++ (int)
      {
         self_type tmp = *this;

         base_type::operator++();
         while ( *this != container().end() &&
                  base_type::operator*() == NULL )
            base_type::operator++();

         return tmp;
      }
      ///
      inline self_type& operator-- ()
      { 
         if ( *this != container().begin() )
            base_type::operator--();
         while ( *this != container().begin() &&
                  base_type::operator*() == NULL )
            base_type::operator--();

         return *this;
      }
      ///
      inline self_type operator-- (int)
      { 
         self_type tmp = *this;

         if ( *this != container().begin() )
            base_type::operator--();
         while ( *this != container().begin() &&
                  base_type::operator*() == NULL )
            base_type::operator--();

         return tmp;
      }
      ///
      inline self_type& operator+= (int dist)
      {
         for ( int i = 0; i < dist; ++i )
            self_type::operator++();

         return *this;
      }
      ///
      inline self_type operator+ (int dist)
      {
         self_type tmp = *this;
         tmp += dist;
         return tmp;
      }
      ///
      inline self_type& operator-= (int dist)
      {
         for ( int i = 0; i < dist; ++i )
            self_type::operator--();

         return *this;
      }
      ///
      inline self_type operator- (int dist)
      {
         self_type tmp = *this;
         tmp -= dist;
         return tmp;
      }
      ///
      int operator- ( const self_type& st )
      {
         int distance = 0;

         // if one of the iterators that are used to be compared points to the
         // end of the container, the while loop below counts one more element than
         // needed. Hence, decrease the distance if needed.
         if ( ( *this == container().end() ||
                  st == container().end() ) && *this != st )
            distance--;

         if ( st >= *this )
         {
            while ( st != *this )
            {
               self_type::operator++();
               distance++;
            }
         }
         else
         {
            while ( st != *this )
            {
               self_type::operator--();
               distance++;
            }
         }

         return distance;
      }
      ///@}
  
  
   private:
  
      const container_type* c_;
  
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/skipnull_iterator.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: skipnull_iterator.h,v $
 *-----------------------------------------------------------------------*/
