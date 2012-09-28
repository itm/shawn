/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_STATIC_NODE_ARRAY_H
#define __SHAWN_SYS_MISC_STATIC_NODE_ARRAY_H

#include "sys/world.h"
#include "sys/misc/node_change_listener.h"
#include <cassert>


namespace shawn
{
   class Node;

   template<typename T>
   class StaticNodeArray
      : public NodeChangeListener
   {
   public:
      typedef T        value_type;
      typedef T&       reference;
      typedef const T& const_reference;
      typedef int      size_type;
      
      StaticNodeArray( const World& w )
         : world_ ( w ),
           size_  ( w.node_id_space_size() ),
           data_  ( new value_type[size_] )
      {}
      ~StaticNodeArray()
      { delete [] data_; }

      inline reference operator [] ( const Node& v ) throw()
      {
         assert( world_.node_id_space_size() == size_ );
         assert( v.id() >= 0 );
         assert( v.id() < size_ );
         return data_[v.id()];
      }

      inline const_reference operator [] ( const Node& v ) const throw()
      {
         assert( world_.node_id_space_size() == size_ );
         assert( v.id() >= 0 );
         assert( v.id() < size_ );
         return data_[v.id()];
      }
      
      inline const_reference operator() ( const Node& v ) const throw()
      { return operator[](v); }

      inline void set( const Node& v, const_reference val ) throw()
      { operator[](v) = val; }

      inline const_reference get( const Node& v ) const throw()
      { return operator[](v); }

      inline void clear_to( const value_type& val ) throw()
      { std::fill( data_, data_+size_, val ); }

      virtual void node_added( Node& v ) throw()
      {
         ABORT_NOT_IMPLEMENTED;
      }

      virtual void node_removed( Node& v ) throw()
      {
         ABORT_NOT_IMPLEMENTED;
      }

      virtual void id_changed( int old_id, int new_id ) throw()
      {
         ABORT_NOT_IMPLEMENTED;
      }

      virtual bool invalidate( void ) throw()
      {
         return false;		 
      }

   private:
      const World& world_;
      int size_;
      value_type* data_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/static_node_array.h,v $
 * Version $Revision: 42 $
 * Date    $Date: 2007-06-12 18:55:07 +0200 (Tue, 12 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: static_node_array.h,v $
 *-----------------------------------------------------------------------*/
