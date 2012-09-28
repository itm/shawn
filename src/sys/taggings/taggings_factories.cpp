/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/basic_tags.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/taggings_factories.h"
#include "sys/taggings/node_reference_tag.h"
#include "sys/util/string_conv.h"



namespace shawn
{
	// ----------------------------------------------------------------------
	GroupTagFactory::
		GroupTagFactory()
	{}
	// ----------------------------------------------------------------------
	GroupTagFactory::
		~GroupTagFactory()
	{}
	// ----------------------------------------------------------------------
	Tag*
		GroupTagFactory::
		create( const std::string& type,
		const std::string& name,
		const std::string& content ) 
		throw( std::runtime_error )
	{
		assert( GroupTag::TAG_TYPE == type );
		assert( content == "" );
		return new GroupTag( name );
	}
	// ----------------------------------------------------------------------
	std::string
		GroupTagFactory::
		name( void )
		const throw()
	{
		return GroupTag::TAG_TYPE;
	}
	// ----------------------------------------------------------------------
	std::string
		GroupTagFactory::
		description( void )
		const throw()
	{
		return std::string("constructs an GroupTag");
	}






   IntegerTagFactory::
   IntegerTagFactory()
   {}
   // ----------------------------------------------------------------------
   IntegerTagFactory::
   ~IntegerTagFactory()
   {}
   // ----------------------------------------------------------------------
   Tag*
   IntegerTagFactory::
   create( const std::string& type,
           const std::string& name,
           const std::string& content
           ) 
      throw( std::runtime_error )
   {
      assert( IntegerTag::TAG_TYPE == type );
      int val=conv_string_to_int(content);//throws through
      return new IntegerTag( name, val );
   }
   // ----------------------------------------------------------------------
   std::string
   IntegerTagFactory::
   name( void )
      const throw()
   {
      return IntegerTag::TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string
   IntegerTagFactory::
   description( void )
      const throw()
   {
      return std::string("constructs an IntegerTag");
   }





   DoubleTagFactory::
   DoubleTagFactory()
   {}
   // ----------------------------------------------------------------------
   DoubleTagFactory::
   ~DoubleTagFactory()
   {}
   // ----------------------------------------------------------------------
   Tag*
   DoubleTagFactory::
   create( const std::string& type,
           const std::string& name,
           const std::string& content
          ) 
      throw( std::runtime_error )
   {
      assert( DoubleTag::TAG_TYPE == type );
      double val=conv_string_to_double(content);//throws through
      return new DoubleTag( name, val );
   }
   // ----------------------------------------------------------------------
   std::string
   DoubleTagFactory::
   name( void )
      const throw()
   {
      return DoubleTag::TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string
   DoubleTagFactory::
   description( void )
      const throw()
   {
      return std::string("constructs a DoubleTag");
   }






   BoolTagFactory::
   BoolTagFactory()
   {}
   // ----------------------------------------------------------------------
   BoolTagFactory::
   ~BoolTagFactory()
   {}
   // ----------------------------------------------------------------------
   Tag*
   BoolTagFactory::
   create( const std::string& type,
           const std::string& name,
           const std::string& content
           ) 
      throw( std::runtime_error )
   {
      assert( BoolTag::TAG_TYPE == type );
      bool val=conv_string_to_bool(content);//throws through
      return new BoolTag( name, val );
   }
   // ----------------------------------------------------------------------
   std::string
   BoolTagFactory::
   name( void )
      const throw()
   {
      return BoolTag::TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string
   BoolTagFactory::
   description( void )
      const throw()
   {
      return std::string("constructs a BoolTag");
   }






   StringTagFactory::
   StringTagFactory()
   {}
   // ----------------------------------------------------------------------
   StringTagFactory::
   ~StringTagFactory()
   {}
   // ----------------------------------------------------------------------
   Tag*
   StringTagFactory::
   create( const std::string& type,
           const std::string& name,
           const std::string& content
           ) 
      throw( std::runtime_error )
   {
      return new StringTag( name, content );
   }
   // ----------------------------------------------------------------------
   std::string
   StringTagFactory::
   name( void )
      const throw()
   {
      return StringTag::TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string
   StringTagFactory::
   description( void )
      const throw()
   {
      return std::string("constructs an StringTag");
   }






   NodeReferenceTagFactory::
   NodeReferenceTagFactory()
   {}
   // ----------------------------------------------------------------------
   NodeReferenceTagFactory::
   ~NodeReferenceTagFactory()
   {}
   // ----------------------------------------------------------------------
   Tag*
   NodeReferenceTagFactory::
   create( const std::string& type,
           const std::string& name,
           const std::string& content
           ) 
      throw( std::runtime_error )
   {
	  if( has_world() )
		return new NodeReferenceTag( name, content, world_w() );

	  throw std::runtime_error("No world available to NodeReferenceTagFactory");
   }
   // ----------------------------------------------------------------------
   std::string
   NodeReferenceTagFactory::
   name( void )
      const throw()
   {
      return NodeReferenceTag::TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string
   NodeReferenceTagFactory::
   description( void )
      const throw()
   {
      return std::string("constructs an NodeReferenceTag");
   }



}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/taggings_factories.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: taggings_factories.cpp,v $
 *-----------------------------------------------------------------------*/
