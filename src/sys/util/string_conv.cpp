/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/util/string_conv.h"
#include "shawn_config.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <stdexcept>

namespace shawn
{

   int 
   conv_string_to_int( const std::string& v )
      throw( std::runtime_error )
   {
      const char* cstr = v.c_str();
      char* errpt;
      int res = strtol( cstr, &errpt, 10 );

      if( (*cstr != 0) && (*errpt == 0) )
         return res;
      else
         throw std::runtime_error( std::string("Value '") +
                                   v +
                                   std::string("' is no integer.") );
   }
   // ----------------------------------------------------------------------
   double
   conv_string_to_double( const std::string& v )
      throw( std::runtime_error )
   {
      const char* cstr = v.c_str();
      char* errpt;
      double res = strtod( cstr, &errpt );

      if( (*cstr != 0) && (*errpt == 0) )
         return res;
      else
         throw std::runtime_error( std::string("Value '") +
                                   v +
                                   std::string("' is not numeric.") );
   }
   // ----------------------------------------------------------------------
   bool
   conv_string_to_bool( const std::string& v )
      throw( std::runtime_error )
   {
      typedef std::pair<std::string,bool> BoolName;
      static const BoolName bool_names_[] = {
         BoolName( "true",  true ),
         BoolName( "false", false ),
         BoolName( "yes",   true ),
         BoolName( "no",    false ),
         BoolName( "y",     true ),
         BoolName( "n",     false ),
         BoolName( "",      false )
      };

      const char* cstr = v.c_str();
      char* errpt;
      int intres = strtol( cstr, &errpt, 10 );
      
      if( (*cstr != 0) && (*errpt == 0) )
         return intres != 0;

      const BoolName* bn = bool_names_;
      while( !bn->first.empty() )
         {
            if( v == bn->first )
               return bn->second;
            ++bn;
         }
      
      throw std::runtime_error( std::string("Value '") +
                                v +
                                std::string("' is no bool.") );

      // cannot reach here
      abort();
      return false;
   }
   // ----------------------------------------------------------------------
   void
   split_comma_separated( const std::string& s,
                          std::list<std::string>& out )
      throw()
   {
      std::string::size_type pos = 0;
      std::string::size_type next;

      while( (next=s.find(',',pos)) != std::string::npos )
         {
            out.push_back(s.substr(pos,next-pos));
            pos=next; ++pos;
         }
      out.push_back(s.substr(pos,std::string::npos));
   }
   

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/util/string_conv.cpp,v $
 * Version $Revision: 182 $
 * Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: string_conv.cpp,v $
 *-----------------------------------------------------------------------*/
