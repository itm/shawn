/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#ifndef __SHAWN_SYS_MISC_DYNAMIC_NODE_ARRAY_H
#define __SHAWN_SYS_MISC_DYNAMIC_NODE_ARRAY_H

#include "sys/world.h"
#include "sys/misc/node_change_listener.h"
#include <cstdlib>
#include <cassert>
#include <vector>

namespace shawn
{
	class Node;

	//---------------------------------------------------------------------
	/**
	*
	*/
	template<typename T>
	class DynamicNodeArray
		: public NodeChangeListener
	{
	public:
		typedef T        value_type;
		typedef T&       reference;
		typedef const T& const_reference;
		typedef int      size_type;

		//---------------------------------------------------------------------
		/**
		*/
		DynamicNodeArray( World& w )
			: world_ ( &w ),
			data_  ( w.node_id_space_size() )
		{
			w.add_node_change_listener(*this);
		}

		//---------------------------------------------------------------------
		/**
		*/
		virtual ~DynamicNodeArray()
		{
			if( world_!=NULL )
				world_->remove_node_change_listener(*this);
		}

		//---------------------------------------------------------------------
		/**
		*/
		bool find( Node& v ) throw()
		{
			assert( v.id() >= 0 );
			return v.id() < int(data_.size());
		}
		
		//---------------------------------------------------------------------
		/**
		*/
		bool find( const Node& v ) throw()
		{
			assert( v.id() >= 0 );
			return v.id() < int(data_.size());
		}

		//---------------------------------------------------------------------
		/**
		*/
		reference operator [] ( const Node& v ) throw()
		{
			assert( v.id() >= 0 );
			assert( v.id() < int(data_.size()) );
			return data_[v.id()];
		}

		//---------------------------------------------------------------------
		/**
		*/
		const_reference operator [] ( const Node& v ) const throw()
		{
			assert( v.id() >= 0 );
			assert( v.id() < int(data_.size()) );
			return data_[v.id()];
		}

		//---------------------------------------------------------------------
		/**
		*/
		void clear_to( const value_type& val ) throw()
		{ 
			std::fill( data_.begin(), data_.end(), val ); 
		}

		//---------------------------------------------------------------------
		/**
		*/
		virtual void node_added( Node& v ) throw()
		{
			assert( v.id() >= 0 );
			if(!find(v))
				data_.resize(v.id()+1);
		}

		//---------------------------------------------------------------------
		/**
		*/
		virtual void node_removed( Node& v ) throw()
		{
			ABORT_NOT_IMPLEMENTED;
		}

		//---------------------------------------------------------------------
		/**
		*/
		virtual void id_changed( int old_id, int new_id ) throw()
		{
			ABORT_NOT_IMPLEMENTED;
		}

		//---------------------------------------------------------------------
		/**
		*/
		virtual bool invalidate( void ) throw()
		{
			data_.clear(); 
			world_=NULL; 
			return false; 
		}

		//---------------------------------------------------------------------
		/**
		*/
		inline World& world_w( void ) throw()
		{ 
			assert(world_!=NULL); 
			return *world_; 
		}

	private:
		///
		World* world_;

		///
		std::vector<value_type> data_;
	};

}

#endif
/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/misc/dynamic_node_array.h,v $
* Version $Revision: 213 $
* Date    $Date: 2008-06-13 15:36:21 +0200 (Fri, 13 Jun 2008) $
*-----------------------------------------------------------------------
* $Log: dynamic_node_array.h,v $
*-----------------------------------------------------------------------*/
