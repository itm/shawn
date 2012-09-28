/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/
#include "sys/edge_model.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/communication_model.h"

namespace shawn
{
	template<typename NodeType>
	shawn::EdgeModel::AbstractIteratorHelper<NodeType>::
		~AbstractIteratorHelper()
	{}

	// ----------------------------------------------------------------------
	// This constructs object code for <Node> and <const Node> instantiations
	// of AbstractIteratorHelper. This is needed as the dtor is not in the header.
	template class shawn::EdgeModel::AbstractIteratorHelper<Node>;
	template class shawn::EdgeModel::AbstractIteratorHelper<const Node>;

	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		AbstractAdjacencyIterator()
		: helper_ ( NULL )
	{}

	// ----------------------------------------------------------------------
	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		AbstractAdjacencyIterator( Helper* h )
		: helper_( h )
	{
		if( h != NULL )
			helper_->init();
	}

	// ----------------------------------------------------------------------
	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		AbstractAdjacencyIterator( const AbstractAdjacencyIterator<NodeType>& ait )
	{
		if( ait.helper_ == NULL )
			helper_ = NULL;
		else
			helper_ = ait.helper_->clone();

		assert( *this == ait );
	}
	// ----------------------------------------------------------------------
	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>&
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator = ( const AbstractAdjacencyIterator<NodeType>& ait )
		throw()
	{
		if( helper_ != NULL )
			delete helper_;

		if( ait.helper_ == NULL )
			helper_ = NULL;
		else
			helper_ = ait.helper_->clone();

		assert( *this == ait );
		return *this;
	}

	// ----------------------------------------------------------------------
	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		~AbstractAdjacencyIterator()
	{
		if( helper_ != NULL )
			delete helper_;
	}

	// ----------------------------------------------------------------------
	template<typename NodeType>
	shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>&
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator++ ( void )
		throw()
	{
		if( helper_ != NULL )
			helper_->next();
		return *this;
	}

	// ----------------------------------------------------------------------
	template<typename NodeType>
	NodeType&
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator* ( void )
		const throw()
	{
		assert( helper_ != NULL ); // deref of end() iterators is forbidden
		return *(helper_->current());
	}
	// ----------------------------------------------------------------------
	template<typename NodeType>
	NodeType*
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator -> ( void )
		const throw()
	{
		assert( helper_ != NULL );  // deref of end() iterators is forbidden
		return helper_->current();
	}
	// ----------------------------------------------------------------------
	template<typename NodeType>
	bool
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator == ( const AbstractAdjacencyIterator<NodeType>& ait )
		const throw()
	{
		// helper_==NULL and helper_->current()==NULL are considered
		// equal for easier construction of end() iterators
		if( (helper_ == NULL) || (helper_->current()==NULL) )
			return (ait.helper_ == NULL) || (ait.helper_->current()==NULL);
		else if( ait.helper_ == NULL )
			return false;
		else
			return ait.helper_->current() == helper_->current();
	}
	// ----------------------------------------------------------------------
	template<typename NodeType>
	bool
		shawn::EdgeModel::AbstractAdjacencyIterator<NodeType>::
		operator != ( const AbstractAdjacencyIterator<NodeType>& ait )
		const throw()
	{
		return !( *this == ait );
	}

	// ----------------------------------------------------------------------
	// This constructs object code for <Node> and <const Node> instantiations
	// of AbstractAdjacencyIterator. This is needed as class is not
	// implemented in the header.
	template class EdgeModel::AbstractAdjacencyIterator<Node>;
	template class EdgeModel::AbstractAdjacencyIterator<const Node>;









	// ----------------------------------------------------------------------
	EdgeModel::
		EdgeModel()
		: world_ ( NULL ),
		  comm_model_ ( NULL )
	{}

	// ----------------------------------------------------------------------
	EdgeModel::
		~EdgeModel()
	{}

	// ----------------------------------------------------------------------
	void
		EdgeModel::
		set_world( World& w )
		throw()
	{
		world_ = &w;
	}

	// ----------------------------------------------------------------------
	void
		EdgeModel::
		set_communication_model( const CommunicationModel& cm )
		throw()
	{
		comm_model_ = &cm;
	}

	// ----------------------------------------------------------------------
	World&
		EdgeModel::
		world_w( void )
		throw()
	{
		assert( world_ != NULL );
		return *world_;
	}

	// ----------------------------------------------------------------------
	const World&
		EdgeModel::
		world( void )
		const throw()
	{
		assert( world_ != NULL );
		return *world_;
	}

	// ----------------------------------------------------------------------
	const CommunicationModel&
		EdgeModel::
		communication_model( void )
		const throw()
	{
		assert( comm_model_ != NULL );
		return *comm_model_;
	}

	// ----------------------------------------------------------------------
	void
		EdgeModel::
		init( void )
		throw()
	{
		assert( comm_model_ != NULL );
		assert( world_ != NULL );
	}

	// ----------------------------------------------------------------------
	bool
		EdgeModel::
		can_communicate_bidi( const Node& u, const Node& v )
		const throw()
	{
		return communication_model().can_communicate_bidi(u,v);
	}

	// ----------------------------------------------------------------------
	bool
		EdgeModel::
		are_adjacent( const Node& u, const Node& v, CommunicationDirection d )
		const throw()
	{
		switch( d )
		{
			case CD_IN:
				return communication_model().can_communicate_uni(v,u);

			case CD_OUT:
				return communication_model().can_communicate_uni(u,v);

			case CD_BIDI:
				return communication_model().can_communicate_bidi(u,v);

			case CD_ANY:
				return communication_model().can_communicate_uni(u,v) ||
					communication_model().can_communicate_uni(v,u);

			default:
				// empty statement to avoid warning about not handled enum element
				;
		}

		return false;
	}

	// ----------------------------------------------------------------------
	bool
		EdgeModel::
		can_communicate_uni( const Node& u, const Node& v )
		const throw()
	{
		return communication_model().can_communicate_uni(u, v);
	}

	// ----------------------------------------------------------------------
	void
		EdgeModel::
		id_changed( int, int )
		throw()
	{}

	// ----------------------------------------------------------------------
	bool
		EdgeModel::
		invalidate( void )
		throw()
	{
		return false;
	}


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_model.cpp,v $
* Version $Revision: 389 $
* Date    $Date: 2010-01-22 18:20:49 +0100 (Fri, 22 Jan 2010) $
*-----------------------------------------------------------------------
* $Log: edge_model.cpp,v $
*-----------------------------------------------------------------------*/
