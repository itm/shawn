/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/

#include "sys/edge_models/lazy_edge_model.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/misc/box.h"

namespace shawn
{
	template<typename NodeType, typename WorldNodeIterator>
	shawn::LazyEdgeModel::LazyIteratorHelper<NodeType, WorldNodeIterator>::
		LazyIteratorHelper( const LazyEdgeModel& em,
		EdgeModel::CommunicationDirection dir,
		NodeType& n,
		const WorldNodeIterator& wit,
		const WorldNodeIterator& wend )
		: AbstractIteratorHelper<NodeType>(dir),
		edge_model_   ( em ),
		node_         ( n ),
		world_it_     ( wit ),
		world_end_it_ ( wend )
	{}

	// ----------------------------------------------------------------------
	template<typename NodeType,
		typename WorldNodeIterator>
		shawn::LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>::
		~LazyIteratorHelper()
	{}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename WorldNodeIterator>
	void
		shawn::LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>::
		init( void )
		throw()
	{
		while( (world_it_!=world_end_it_) && !edge_model_.are_adjacent(node_, *world_it_, base_type::direction_))
			++world_it_;
	}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename WorldNodeIterator>
	void
		shawn::LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>::
		next( void )
		throw()
	{
		if( world_it_ != world_end_it_ )
		{ 
			++world_it_; 
			init(); 
		}
	}
	// ----------------------------------------------------------------------
	template<typename NodeType, typename WorldNodeIterator>
	NodeType*
		shawn::LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>::
		current( void )
		const throw()
	{
		if( world_it_ == world_end_it_ )
			return NULL;
		else
			return &(*world_it_);
	}

	// ----------------------------------------------------------------------
	template<typename NodeType, typename WorldNodeIterator>
	shawn::EdgeModel::AbstractIteratorHelper<NodeType>*
		shawn::LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>::
		clone( void )
		const throw()
	{
		return new LazyEdgeModel::LazyIteratorHelper<NodeType,WorldNodeIterator>
         (edge_model_, base_type::direction_, node_, world_it_, world_end_it_ );
	}

	// ----------------------------------------------------------------------
	template class LazyEdgeModel::LazyIteratorHelper<Node,World::node_iterator>;
	template class LazyEdgeModel::LazyIteratorHelper<const Node,World::const_node_iterator>;

	// ----------------------------------------------------------------------
	LazyEdgeModel::
		LazyEdgeModel()
	{}

	// ----------------------------------------------------------------------
	LazyEdgeModel::
		~LazyEdgeModel()
	{}

	// ----------------------------------------------------------------------
	Box
		LazyEdgeModel::
		observer_initial_zone(Node&, const Vec& pos, const Vec& velo ) 
		throw()
	{
		return Box::INFINITE_3D_SPACE;
	}

	// ----------------------------------------------------------------------
	int
		LazyEdgeModel::
		nof_adjacent_nodes( const Node& v, CommunicationDirection d ) 
		const throw()
	{
		int res = 0;
		for(const_adjacency_iterator it = begin_adjacent_nodes(v, d), endit = end_adjacent_nodes(v); it != endit; ++it )
			++res;
		return res;
	}
	// ----------------------------------------------------------------------
	EdgeModel::const_adjacency_iterator
		LazyEdgeModel::
		begin_adjacent_nodes( const Node& v, CommunicationDirection d )
		const throw()
	{
		return new LazyIteratorHelper
			<const Node,World::const_node_iterator>
			( *this, d , v, world().begin_nodes(), world().end_nodes() );
	}
	// ----------------------------------------------------------------------
	EdgeModel::const_adjacency_iterator
		LazyEdgeModel::
		end_adjacent_nodes( const Node& )
		const throw()
	{
		return EdgeModel::const_adjacency_iterator( NULL );
	}
	// ----------------------------------------------------------------------
	EdgeModel::adjacency_iterator
		LazyEdgeModel::
		begin_adjacent_nodes_w( Node& v, CommunicationDirection d )
		throw()
	{
		return new LazyIteratorHelper
			<Node,World::node_iterator>
			( *this, d, v, world_w().begin_nodes_w(), world_w().end_nodes_w() );
	}
	// ----------------------------------------------------------------------
	EdgeModel::adjacency_iterator
		LazyEdgeModel::
		end_adjacent_nodes_w( Node& v )
		throw()
	{
		return EdgeModel::adjacency_iterator( NULL );
	}

	// ----------------------------------------------------------------------
	Box
		LazyEdgeModel::
		observer_update_zone(Node& v,  const Vec& newpos,  const Vec& velo )
		throw()
	{ 
		return Box::INFINITE_3D_SPACE; 
	}

	// ----------------------------------------------------------------------
	bool
		LazyEdgeModel::
		supports_mobility( void )
		const throw()
	{
		return true;
	}
	// ----------------------------------------------------------------------  
	void
		LazyEdgeModel::
		node_added( Node& )
		throw()
	{
		// do nothing, the important 'add-method' here is add_edge()
	}
	// ----------------------------------------------------------------------
	void
		LazyEdgeModel::
		node_removed( Node& )
		throw()
	{
		// do nothing, the adjacent_nodes are selected by iterating (every time
		// on demand) through all nodes in the world
	}
}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_models/lazy_edge_model.cpp,v $
* Version $Revision: 504 $
* Date    $Date: 2011-01-24 20:00:04 +0100 (Mon, 24 Jan 2011) $
*-----------------------------------------------------------------------
* $Log: lazy_edge_model.cpp,v $
*-----------------------------------------------------------------------*/
