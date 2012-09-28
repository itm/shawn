/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/taggings/map_tags.h"
#include "sys/util/string_conv.h"

#include <sstream>

namespace shawn
{




#if 0
   const std::string IntegerIntegerMapTag::TAG_TYPE("map-int-int");
   // ----------------------------------------------------------------------
   IntegerIntegerMapTag::
   IntegerIntegerMapTag( const std::string& n,
                         bool lock )
      : MapTag<int,int>(n,lock)
   {}
   // ----------------------------------------------------------------------
   IntegerIntegerMapTag::
   ~IntegerIntegerMapTag()
   {}
   // ----------------------------------------------------------------------
   const std::string& 
   IntegerIntegerMapTag::
   type_identifier( void )
      const throw()
   {
      return TAG_TYPE;
   }
   // ----------------------------------------------------------------------
   IntegerIntegerMapTag::KeyType
   IntegerIntegerMapTag::
   string_to_key( const std::string& s )
      const throw( std::runtime_error )
   { return conv_string_to_int(s); }
   // ----------------------------------------------------------------------
   IntegerIntegerMapTag::ValueType
   IntegerIntegerMapTag::
   string_to_value( const std::string& s )
      const throw( std::runtime_error )
   { return conv_string_to_int(s); }
   // ----------------------------------------------------------------------
   std::string
   IntegerIntegerMapTag::
   key_to_string( const KeyType& k )
      const throw()
   {
      std::ostringstream oss;
      oss << k;
      return oss.str();
   }
   // ----------------------------------------------------------------------
   std::string
   IntegerIntegerMapTag::
   value_to_string( const ValueType& v )
      const throw()
   {
      std::ostringstream oss;
      oss << v;
      return oss.str();
   }
#endif



#define AUTO_IMPLEMENT_MAP_TAG( TYPENAME,TYPEID,KEYCONV,VALUECONV )          \
   const std::string TYPENAME::TAG_TYPE(TYPEID);                             \
   TYPENAME::TYPENAME( const std::string& n, bool lock )                     \
      : BaseType(n,lock) {}                                                  \
   TYPENAME::~TYPENAME()                                                     \
   {}                                                                        \
   const std::string& TYPENAME::type_identifier( void ) const throw()        \
   { return TAG_TYPE; }                                                      \
   TYPENAME::KeyType TYPENAME::string_to_key( const std::string& s )         \
   const throw( std::runtime_error )                                         \
   { return KEYCONV(s); }                                                    \
   TYPENAME::ValueType TYPENAME::string_to_value( const std::string& s )     \
      const throw( std::runtime_error )                                      \
   { return VALUECONV(s); }                                                  \
   std::string TYPENAME::key_to_string( const KeyType& k ) const throw()     \
   { std::ostringstream oss; oss << k; return oss.str(); }                   \
   std::string TYPENAME::value_to_string( const ValueType& v ) const throw() \
   { std::ostringstream oss; oss << v; return oss.str(); }


#define conv_string_to_string(s) s

   AUTO_IMPLEMENT_MAP_TAG( IntegerIntegerMapTag, "map-int-int", conv_string_to_int, conv_string_to_int );
   AUTO_IMPLEMENT_MAP_TAG( IntegerDoubleMapTag, "map-int-double", conv_string_to_int, conv_string_to_double );
   AUTO_IMPLEMENT_MAP_TAG( IntegerStringMapTag, "map-int-string", conv_string_to_int, conv_string_to_string );
   AUTO_IMPLEMENT_MAP_TAG( IntegerBoolMapTag, "map-int-bool", conv_string_to_int, conv_string_to_bool );
   AUTO_IMPLEMENT_MAP_TAG( DoubleIntegerMapTag, "map-double-int", conv_string_to_double, conv_string_to_int );
   AUTO_IMPLEMENT_MAP_TAG( DoubleDoubleMapTag, "map-double-double", conv_string_to_double, conv_string_to_double );
   AUTO_IMPLEMENT_MAP_TAG( DoubleStringMapTag, "map-double-string", conv_string_to_double, conv_string_to_string );
   AUTO_IMPLEMENT_MAP_TAG( DoubleBoolMapTag, "map-double-bool", conv_string_to_double, conv_string_to_bool );
   AUTO_IMPLEMENT_MAP_TAG( StringIntegerMapTag, "map-string-int", conv_string_to_string, conv_string_to_int );
   AUTO_IMPLEMENT_MAP_TAG( StringDoubleMapTag, "map-string-double", conv_string_to_string, conv_string_to_double );
   AUTO_IMPLEMENT_MAP_TAG( StringStringMapTag, "map-string-string", conv_string_to_string, conv_string_to_string );
   AUTO_IMPLEMENT_MAP_TAG( StringBoolMapTag, "map-string-bool", conv_string_to_string, conv_string_to_bool );
   AUTO_IMPLEMENT_MAP_TAG( BoolIntegerMapTag, "map-bool-int", conv_string_to_bool, conv_string_to_int );
   AUTO_IMPLEMENT_MAP_TAG( BoolDoubleMapTag, "map-bool-double", conv_string_to_bool, conv_string_to_double );
   AUTO_IMPLEMENT_MAP_TAG( BoolStringMapTag, "map-bool-string", conv_string_to_bool, conv_string_to_string );


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/taggings/map_tags.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: map_tags.cpp,v $
 *-----------------------------------------------------------------------*/
