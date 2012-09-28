/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_FACTORIES_H
#define __SHAWN_SYS_TAGGINGS_FACTORIES_H

#include "sys/taggings/tag_factory.h"

namespace shawn
{
   class World;

   class GroupTagFactory
      : public TagFactory
   {
   public:
      GroupTagFactory();
      virtual ~GroupTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };

   class IntegerTagFactory
      : public TagFactory
   {
   public:
      IntegerTagFactory();
      virtual ~IntegerTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


   class DoubleTagFactory
      : public TagFactory
   {
   public:
      DoubleTagFactory();
      virtual ~DoubleTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


   class BoolTagFactory
      : public TagFactory
   {
   public:
      BoolTagFactory();
      virtual ~BoolTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


   class StringTagFactory
      : public TagFactory
   {
   public:
      StringTagFactory();
      virtual ~StringTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };


   class NodeReferenceTagFactory
      : public TagFactory
   {
   public:
      NodeReferenceTagFactory();
      virtual ~NodeReferenceTagFactory();
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error );
      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();
   };



   template<typename TagType>
   class MapTagFactory
      : public TagFactory
   {
   public:
      MapTagFactory(){}
      virtual ~MapTagFactory(){}
      
      virtual Tag* create( const std::string& type,
                           const std::string& name,
                           const std::string& content
						   ) throw( std::runtime_error )
      { return new TagType(name); }
      virtual std::string name( void ) const throw()
      { return TagType::TAG_TYPE; }
      virtual std::string description( void ) const throw()
      { return TagType::TAG_TYPE; }
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/taggings_factories.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: taggings_factories.h,v $
 *-----------------------------------------------------------------------*/
