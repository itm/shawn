/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAG_CONTAINER_H
#define __SHAWN_SYS_TAG_CONTAINER_H


#include <map>
#include <string>
#include <stdexcept>
#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"
#include "sys/taggings/basic_tag_traits.h"

namespace shawn
{
   class Tag;
   
   DECLARE_HANDLES(TagContainer);
   DECLARE_HANDLES(Tag);

   class TagContainer
      : public RefcntPointable
   {
   public:
      ///@name Public Types
      ///@{
      ///
      typedef std::map<std::string, RefcntPointer<Tag> > TagMap;
      ///
      typedef TagMap::const_iterator tag_iterator;
      ///@}


      ///@name Ctor, Dtor
      ///@{
      ///
      TagContainer();
      ///
      virtual ~TagContainer();
      ///@}


      ///@name Tag Modification
      ///@{
      /// overwrites already existing tag with same name
      virtual void add_tag( const TagHandle& ) throw();
      /// tag must be contained in this container!
      virtual void remove_tag( const ConstTagHandle& ) throw();
      /// passing non-existing tag name is okay
      virtual void remove_tag_by_name( const std::string& ) throw();
      ///
      virtual void clear_tags( void ) throw();
      ///@}

      ///@name Tag Access
      ///@{      
      /// NULL if nonex
      virtual RefcntPointer<const Tag> find_tag( const std::string& ) const throw();
      /// NULL if nonex
      virtual RefcntPointer<Tag> find_tag_w( const std::string& ) throw();
      ///
      template<typename T> inline const T* find_typed_tag( const std::string& t ) const throw( std::runtime_error )
      {
         RefcntPointer<const Tag> cth = find_tag(t);
         if( cth.is_null() ) return NULL;
         const T* dc = dynamic_cast<const T*>(cth.get());
         if( dc==NULL )
            throw std::runtime_error(std::string("Tag '")+t+std::string("' has wrong type"));
         return dc;
      }
      ///
      template<typename T> inline T* find_typed_tag_w( const std::string& t ) throw( std::runtime_error )
      {
         RefcntPointer<Tag> th = find_tag_w(t);
         if( th.is_null() ) return NULL;
         T* dc = dynamic_cast<T*>(th.get());
         if( dc==NULL )
            throw std::runtime_error(std::string("Tag '")+t+std::string("' has wrong type"));
         return dc;
      }
      ///
      virtual tag_iterator begin_tags( void ) const throw();
      ///
      virtual tag_iterator end_tags( void ) const throw();
      ///
      virtual unsigned int count_tags( void ) const throw();
      ///@}
      
      ///@name Simple Tag Access
      ///@{
      // ----------------------------------------------------------------------
      template <typename ST>
      void write_simple_tag( const std::string& tag_name, ST value )
         throw( std::runtime_error )
      {
         typedef typename basic_tag_traits<ST>::BaseClass BaseClass;
         
         RefcntPointer<Tag> tag = find_tag_w( tag_name );
         if ( tag.is_not_null() )
         {
            BaseClass* tagtype = dynamic_cast<BaseClass*>( tag.get() );
            if ( tagtype != NULL )
            {
               tagtype->set_value( value );
            }
            else
            {
               throw( std::runtime_error("Tag already exists with other type.") );
            }
         }
         else
         {
            BaseClass *tagtype = new BaseClass( tag_name, value );
            tagtype->set_persistency( true );
            add_tag( tagtype );
         }
      };
      // ----------------------------------------------------------------------
      template <typename ST>
      ST read_simple_tag( const std::string& tag_name )
         const throw( std::runtime_error )
      {
         typedef typename basic_tag_traits<ST>::BaseClass BaseClass;
         
         RefcntPointer<const Tag> tag = find_tag( tag_name );
         if ( tag.is_not_null() )
         {
            const BaseClass* tagtype = dynamic_cast<const BaseClass*>( tag.get() );
            if ( tagtype )
               return tagtype->value();
            else
               throw( std::runtime_error("Tag of wrong type.") );
         }
      
         throw( std::runtime_error("Tag not found.") );
      };
      ///@}

   private:
      TagMap tags_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag_container.h,v $
 * Version $Revision: 504 $
 * Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
 *-----------------------------------------------------------------------
 * $Log: tag_container.h,v $
 *-----------------------------------------------------------------------*/
