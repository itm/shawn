/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/tag.h"


// move to shawn_config?
//#define TAG_DEFAULT_PERSISTENT


namespace shawn
{
   Tag::const_entry_iterator::IteratorData::
   ~IteratorData()
   {}
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator::
   const_entry_iterator()
      : data_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator::
   const_entry_iterator( const const_entry_iterator& it )
      : data_ ( NULL )
   {
      operator= ( it );
   }
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator::
   const_entry_iterator( IteratorData* d )
      : data_(d)
   {}
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator::
   ~const_entry_iterator()
   {
      if( data_ != NULL )
         delete data_;
   }
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator&
   Tag::const_entry_iterator::
   operator= ( const const_entry_iterator& it )
      throw()
   {
      if( data_ != NULL )
         delete data_;

      if( it.data_ == NULL )
         data_ = NULL;
      else
         data_ = it.data_->clone();

      return *this;
   }
   // ----------------------------------------------------------------------
   bool
   Tag::const_entry_iterator::
   has_index( void ) 
      const throw()
   {
      assert( data_ != NULL );
      return data_->has_index();
   }
   // ----------------------------------------------------------------------
   std::string
   Tag::const_entry_iterator::
   index( void )
      const throw()
   {
      assert( data_ != NULL );
      return data_->index();
   }
   // ----------------------------------------------------------------------
   std::string
   Tag::const_entry_iterator::
   value( void ) 
      const throw()
   {
      assert( data_ != NULL );
      return data_->value();
   }
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator& 
   Tag::const_entry_iterator::
   operator++ ( void )
      throw()
   {
      assert( data_ != NULL );
      data_->next();
      return *this;
   }
   // ----------------------------------------------------------------------
   bool
   Tag::const_entry_iterator::
   operator == ( const const_entry_iterator& it )
      const throw()
   {
      if( data_ == NULL )
         return it.data_==NULL;
      else if( it.data_==NULL )
         return false;
      else
         return (*data_) == (*it.data_);
   }
   // ----------------------------------------------------------------------
   bool
   Tag::const_entry_iterator::
   operator != ( const const_entry_iterator& it )
      const throw()
   {
      return ! (*this == it);
   }









   Tag::
   Tag( const std::string& s,
        bool lock )
      : name_       ( s ),
#ifdef TAG_DEFAULT_PERSISTENT
        persistent_ ( true ),
#else
        persistent_ ( false ),
#endif 
        lock_count_ ( lock ? 1 : 0 )
   {}
   // ----------------------------------------------------------------------
   Tag::
   ~Tag()
   {}
   // ----------------------------------------------------------------------
   const std::string& 
   Tag::
   name( void )
      const throw()
   {
      return name_;
   }
   // ----------------------------------------------------------------------
   void
   Tag::
   set_persistency( bool p )
      throw()
   {
      persistent_ = p;
   }
   // ----------------------------------------------------------------------
   bool
   Tag::
   is_persistent( void )
      const throw()
   {
      return persistent_;
   }
   // ----------------------------------------------------------------------
   bool
   Tag::
   is_locked( void )
      const throw()
   {
      return lock_count_ > 0;
   }
   // ----------------------------------------------------------------------
   void
   Tag::
   lock( void )
      throw()
   {
      ++lock_count_;
   }
   // ----------------------------------------------------------------------
   void
   Tag::
   unlock( void )
      throw()
   {
      assert( lock_count_ > 0 );
      --lock_count_;
   }
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator
   Tag::
   begin_entries( void )
      const throw()
   {
      return const_entry_iterator(NULL);
   }
   // ----------------------------------------------------------------------
   Tag::const_entry_iterator
   Tag::
   end_entries( void )
      const throw()
   {
      return const_entry_iterator(NULL);
   }
   // ----------------------------------------------------------------------
   void
   Tag::
   add_indexed_entry( const std::string& index, const std::string& value )
      throw( std::runtime_error )
   {
      throw std::runtime_error( std::string("entries not supported by tag type.") );
   }
   // ----------------------------------------------------------------------
   void
   Tag::
   add_value_entry( const std::string& value )
      throw( std::runtime_error )
   {
      throw std::runtime_error( std::string("entries not supported by tag type.") );
   }
   
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: tag.cpp,v $
 *-----------------------------------------------------------------------*/
