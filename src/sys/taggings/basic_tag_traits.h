/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_TAGGINGS_BASIC_TAG_TRAITS_H
#define __SHAWN_SYS_TAGGINGS_BASIC_TAG_TRAITS_H

namespace shawn
{

   class IntegerTag;
   class DoubleTag;
   class StringTag;
   class BoolTag;
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   template <typename T> struct basic_tag_traits;
   // ----------------------------------------------------------------------
   template<>
   struct basic_tag_traits<int>
   {
      typedef IntegerTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct basic_tag_traits<double>
   {
      typedef DoubleTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct basic_tag_traits<std::string>
   {
      typedef StringTag BaseClass;
   };
   // ----------------------------------------------------------------------
   template<>
   struct basic_tag_traits<bool>
   {
      typedef BoolTag BaseClass;
   };

}

#endif
