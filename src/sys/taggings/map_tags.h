/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_MAPS_TAGS_H
#define __SHAWN_SYS_TAGGINGS_MAPS_TAGS_H

#include "sys/tag.h"
#include <map>

namespace shawn
{

   template<typename _K, typename _V>
   class MapTag
      : public Tag
   {
   public:
      typedef _K KeyType;
      typedef _V ValueType;
      typedef std::map<KeyType,ValueType> Map;
      typedef MapTag<_K,_V> SelfType;

      ///@name Ctor, Dtor
      ///@{
      ///
      MapTag( const std::string& n,
              bool lock = false )
         : Tag(n,lock)
      {}
      ///
      virtual ~MapTag()
      {}
      ///@}

      ///@name Value access
      ///@{
      ///
      virtual const Map& value( void ) const throw()
      { return map_; }
      ///
      virtual Map& value_w( void ) throw()
      { return map_; }
      ///@}


      ///@name Persistency
      ///@{
      ///
      virtual std::string encoded_content( void ) const throw( std::runtime_error )
      { return std::string(""); }
      ///
      virtual const_entry_iterator begin_entries( void ) const throw()
      { return const_entry_iterator( new MapIteratorData(map_.begin(),*this) ); }
      ///
      virtual const_entry_iterator end_entries( void ) const throw()
      { return const_entry_iterator( new MapIteratorData(map_.end(),*this) ); }
      ///
      virtual void add_indexed_entry( const std::string& index, const std::string& value ) throw( std::runtime_error )
      { map_[ string_to_key(index) ] = string_to_value(value); }
      ///@}
      
   protected:

         class MapIteratorData
            : public Tag::const_entry_iterator::IteratorData
         {
         public:
            MapIteratorData( const typename Map::const_iterator& it,
                             const SelfType& owner )
               : it_(it), owner_(owner) {}
            virtual ~MapIteratorData()
            {}
            virtual void next( void ) throw()
            { ++it_; }
            virtual Tag::const_entry_iterator::IteratorData* clone( void ) const throw()
            { return new MapIteratorData(it_,owner_); }
            virtual bool has_index( void ) const throw()
            { return true; }
            virtual std::string index( void ) const throw()
            { return owner_.key_to_string(it_->first); }
            virtual std::string value( void ) const throw()
            { return owner_.value_to_string(it_->second); }
            virtual bool operator== ( const Tag::const_entry_iterator::IteratorData& itd ) const throw()
            {
               const MapIteratorData* mid = dynamic_cast<const MapIteratorData*>(&itd);
               if( mid==NULL )
                  return false;
               else
                  return it_ == mid->it_;
            }
         private:
            typename Map::const_iterator it_;
            const SelfType& owner_;
         };
      virtual KeyType string_to_key( const std::string& ) const throw( std::runtime_error ) = 0;
      virtual ValueType string_to_value( const std::string& ) const throw( std::runtime_error ) = 0;
      virtual std::string key_to_string( const KeyType& ) const throw() = 0;
      virtual std::string value_to_string( const ValueType& ) const throw() = 0;
   private:
      Map map_;
   };


#if 0
   class IntegerIntegerMapTag
      : public MapTag<int,int>
   {
   public:
      static const std::string TAG_TYPE;
      IntegerIntegerMapTag( const std::string& n,
                            bool lock = false );
      virtual ~IntegerIntegerMapTag();
      virtual const std::string& type_identifier( void ) const throw();
   protected:
      virtual KeyType string_to_key( const std::string& ) const throw( std::runtime_error );
      virtual ValueType string_to_value( const std::string& ) const throw( std::runtime_error );
      virtual std::string key_to_string( const KeyType& ) const throw();
      virtual std::string value_to_string( const ValueType& ) const throw();
   };
#endif


#define AUTO_DECLARE_MAP_TAG( TYPENAME, KEYTYPE, VALUETYPE )                 \
   class TYPENAME                                                            \
      : public MapTag<KEYTYPE,VALUETYPE>                                     \
   {                                                                         \
   public:                                                                   \
      typedef MapTag<KEYTYPE,VALUETYPE> BaseType;                            \
      static const std::string TAG_TYPE;                                     \
      TYPENAME( const std::string& n, bool lock = false );                   \
      virtual ~TYPENAME();                                                   \
      virtual const std::string& type_identifier( void ) const throw();      \
   protected:                                                                \
      virtual KeyType string_to_key( const std::string& )                    \
         const throw( std::runtime_error );                                  \
      virtual ValueType string_to_value( const std::string& )                \
         const throw( std::runtime_error );                                  \
      virtual std::string key_to_string( const KeyType& ) const throw();     \
      virtual std::string value_to_string( const ValueType& ) const throw(); \
   }

   AUTO_DECLARE_MAP_TAG( IntegerIntegerMapTag, int, int );
   AUTO_DECLARE_MAP_TAG( IntegerDoubleMapTag, int, double );
   AUTO_DECLARE_MAP_TAG( IntegerStringMapTag, int, std::string );
   AUTO_DECLARE_MAP_TAG( IntegerBoolMapTag, int, bool );
   AUTO_DECLARE_MAP_TAG( DoubleIntegerMapTag, double, int );
   AUTO_DECLARE_MAP_TAG( DoubleDoubleMapTag, double, double );
   AUTO_DECLARE_MAP_TAG( DoubleStringMapTag, double, std::string );
   AUTO_DECLARE_MAP_TAG( DoubleBoolMapTag, double, bool );
   AUTO_DECLARE_MAP_TAG( StringIntegerMapTag, std::string, int );
   AUTO_DECLARE_MAP_TAG( StringDoubleMapTag, std::string, double );
   AUTO_DECLARE_MAP_TAG( StringStringMapTag, std::string, std::string );
   AUTO_DECLARE_MAP_TAG( StringBoolMapTag, std::string, bool );
   AUTO_DECLARE_MAP_TAG( BoolIntegerMapTag, bool, int );
   AUTO_DECLARE_MAP_TAG( BoolDoubleMapTag, bool, double );
   AUTO_DECLARE_MAP_TAG( BoolStringMapTag, bool, std::string );

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/map_tags.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: map_tags.h,v $
 *-----------------------------------------------------------------------*/
