/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/basic_tags.h"
#include <sstream>

namespace shawn
{


   // ----------------------------------------------------------------------
   const std::string IntegerTag::TAG_TYPE("int");
   // ----------------------------------------------------------------------
	IntegerTag::
   IntegerTag( const std::string& n,
               int                v,
               bool               lock )
      : SimpleValueTag<int>(n,v,lock)
   {}
   // ----------------------------------------------------------------------
   IntegerTag::
   ~IntegerTag()
   {}
   // ----------------------------------------------------------------------
   const std::string&
   IntegerTag::
   type_identifier( void )
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string 
   IntegerTag::
   encoded_content( void )
      const throw( std::runtime_error )
   {
      std::ostringstream oss;
      oss << value();
      return oss.str();
   }



   // ----------------------------------------------------------------------
   const std::string DoubleTag::TAG_TYPE("double");
   // ----------------------------------------------------------------------
	DoubleTag::
   DoubleTag( const std::string& n,
              double             v,
              bool               lock )
      : SimpleValueTag<double>(n,v,lock)
   {}
   // ----------------------------------------------------------------------
   DoubleTag::
   ~DoubleTag()
   {}
   // ----------------------------------------------------------------------
   const std::string&
   DoubleTag::
   type_identifier( void )
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string 
   DoubleTag::
   encoded_content( void )
      const throw( std::runtime_error )
   {
      std::ostringstream oss;
      oss << value();
      return oss.str();
   }





   // ----------------------------------------------------------------------
   const std::string BoolTag::TAG_TYPE("bool");
   const std::string BoolTag::DEFAULT_TRUE_ENCODING("true");
   const std::string BoolTag::DEFAULT_FALSE_ENCODING("false");
   // ----------------------------------------------------------------------
	BoolTag::
   BoolTag( const std::string& n,
            bool               v,
            bool               lock )
      : SimpleValueTag<bool>(n,v,lock)
   {}
   // ----------------------------------------------------------------------
   BoolTag::
   ~BoolTag()
   {}
   // ----------------------------------------------------------------------
   const std::string&
   BoolTag::
   type_identifier( void )
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string 
   BoolTag::
   encoded_content( void )
      const throw( std::runtime_error )
   {
      if( value() )
         return DEFAULT_TRUE_ENCODING;
      else
         return DEFAULT_FALSE_ENCODING;
   }




   // ----------------------------------------------------------------------
   const std::string StringTag::TAG_TYPE("string");
   // ----------------------------------------------------------------------
	StringTag::
   StringTag( const std::string& n,
              const std::string& v,
               bool              lock )
      : SimpleValueTag<std::string>(n,v,lock)
   {}
   // ----------------------------------------------------------------------
   StringTag::
   ~StringTag()
   {}
   // ----------------------------------------------------------------------
   const std::string&
   StringTag::
   type_identifier( void )
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   std::string 
   StringTag::
   encoded_content( void )
      const throw( std::runtime_error )
   {
      return value();
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/basic_tags.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: basic_tags.cpp,v $
 *-----------------------------------------------------------------------*/
