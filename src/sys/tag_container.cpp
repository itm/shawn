/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/tag_container.h"
#include "sys/tag.h"

namespace shawn
{

   TagContainer::
   TagContainer()
   {}
   // ----------------------------------------------------------------------
   TagContainer::
   ~TagContainer()
   {}
   // ----------------------------------------------------------------------
   void
   TagContainer::
   add_tag( const TagHandle& t )
      throw()
   {
      assert( t.is_not_null() );
      tags_[ t->name() ] = t;
   }
   // ----------------------------------------------------------------------
   void
   TagContainer::
   remove_tag( const ConstTagHandle& t )
      throw()
   {
      assert( t.is_not_null() );

      TagMap::iterator it = tags_.find( t->name() );

      assert( it != tags_.end() );
      assert( t == it->second );
      
      tags_.erase(it);
   }
   // ----------------------------------------------------------------------
   void
   TagContainer::
   remove_tag_by_name( const std::string& n )
      throw()
   {
      TagMap::iterator it = tags_.find( n );
      if( it != tags_.end() )
         tags_.erase(it);
   }
   // ----------------------------------------------------------------------
   void
   TagContainer::
   clear_tags( void )
      throw()
   {
      tags_.clear();
   }
   // ----------------------------------------------------------------------
   ConstTagHandle
   TagContainer::
   find_tag( const std::string& n )
      const throw()
   {
      TagMap::const_iterator it = tags_.find( n );
      if( it != tags_.end() )
         return it->second;
      else
         return NULL;
   }
   // ----------------------------------------------------------------------
   TagHandle
   TagContainer::
   find_tag_w( const std::string& n )
      throw()
   {
      TagMap::iterator it = tags_.find( n );
      if( it != tags_.end() )
         return it->second;
      else
         return NULL;
   }
   // ----------------------------------------------------------------------
   TagContainer::tag_iterator
   TagContainer::
   begin_tags( void )
      const throw()
   {
      return tags_.begin();
   }
   // ----------------------------------------------------------------------
   TagContainer::tag_iterator
   TagContainer::
   end_tags( void )
      const throw()
   {
      return tags_.end();
   }

   unsigned int
   TagContainer::
   count_tags( void ) 
      const throw() 
   {
       return tags_.size();
   }

}


/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag_container.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: tag_container.cpp,v $
 *-----------------------------------------------------------------------*/
