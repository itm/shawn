/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_BASIC_TAGS_H
#define __SHAWN_SYS_TAGGINGS_BASIC_TAGS_H

#include "sys/tag.h"


namespace shawn
{

   template<typename T>
   class SimpleValueTag
      : public Tag
   {
   public:
      typedef T ValueType;

      ///@name Ctor, Dtor
      ///@{
      ///
      SimpleValueTag( const std::string& n,
                      const ValueType&   v,
                      bool lock = false )
         : Tag(n,lock),
           value_( v )
      {}
      ///
      virtual ~SimpleValueTag()
      {}
      ///@}

      ///@name Value access
      ///@{
      ///
      virtual void set_value( const ValueType& v ) throw()
      { value_ = v; }
      ///
      virtual ValueType value( void ) const throw()
      { return value_; }

   private:
      ValueType value_;
   };



   class IntegerTag
      : public SimpleValueTag<int>
   {
   public:
      static const std::string TAG_TYPE;

      IntegerTag( const std::string& n,
                  int                v,
                  bool               lock = false );
      virtual ~IntegerTag();
      virtual const std::string& type_identifier( void ) const throw();
      virtual std::string encoded_content( void ) const throw( std::runtime_error );
   };

   class DoubleTag
      : public SimpleValueTag<double>
   {
   public:
      static const std::string TAG_TYPE;

      DoubleTag( const std::string& n,
                 double             v,
                 bool               lock = false );
      virtual ~DoubleTag();
      virtual const std::string& type_identifier( void ) const throw();
      virtual std::string encoded_content( void ) const throw( std::runtime_error );
   };

   class BoolTag
      : public SimpleValueTag<bool>
   {
   public:
      static const std::string TAG_TYPE;
      static const std::string DEFAULT_TRUE_ENCODING;
      static const std::string DEFAULT_FALSE_ENCODING;

      BoolTag( const std::string& n,
               bool               v,
               bool               lock = false );
      virtual ~BoolTag();
      virtual const std::string& type_identifier( void ) const throw();
      virtual std::string encoded_content( void ) const throw( std::runtime_error );
   };

   class StringTag
      : public SimpleValueTag<std::string>
   {
   public:
      static const std::string TAG_TYPE;

      StringTag( const std::string& n,
                 const std::string& v,
                  bool              lock = false );
      virtual ~StringTag();
      virtual const std::string& type_identifier( void ) const throw();
      virtual std::string encoded_content( void ) const throw( std::runtime_error );
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/basic_tags.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: basic_tags.h,v $
 *-----------------------------------------------------------------------*/
