/************************************************************************
** This file is part of the network simulator Shawn.                  **
** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
** Shawn is free software; you can redistribute it and/or modify it   **
** under the terms of the BSD License. Refer to the shawn-licence.txt **
** file in the root of the Shawn source tree for further details.     **
************************************************************************/

#include <cstdlib>
#include <iostream>
#include "sys/processor.h"
#include "sys/node.h"

using namespace std;

namespace shawn
{

	// ----------------------------------------------------------------------
	Processor::
		Processor()
		: owner_ ( NULL ),
		state_ ( Active ),
		auto_terminate_(false)
	{}

	// ----------------------------------------------------------------------
	Processor::
		~Processor()
	{}

	// ----------------------------------------------------------------------
	void
		Processor::
		set_owner( Node& n )
		throw()
	{
		owner_ = &n;
	}

	// ----------------------------------------------------------------------
	const Node&
		Processor::
		owner( void )
		const throw()
	{
		assert( owner_ != NULL );
		return *owner_;
	}

	// ----------------------------------------------------------------------
	Node&
		Processor::
		owner_w( void )
		throw()
	{
		assert( owner_ != NULL );
		return *owner_;
	}

	// ----------------------------------------------------------------------
	void
		Processor::
		special_boot( void )
		throw()
	{}

	// ----------------------------------------------------------------------
	void
		Processor::
		boot( void )
		throw()
	{}

	// ----------------------------------------------------------------------
	bool
		Processor::
		process_message( const ConstMessageHandle& )
		throw()
	{ return false; }
	
	// ----------------------------------------------------------------------
	bool
		Processor::
		process_sent_indication( const ConstMessageHandle&, tx_state state, int tries )
		throw()
	{ return false; }
	
	// ----------------------------------------------------------------------
	void
		Processor::
		work( void )
		throw()
	{}

	// ----------------------------------------------------------------------
	void
		Processor::
		send( const MessageHandle& mh )
		throw()
	{
		owner_w().send( mh );
	}

	// ----------------------------------------------------------------------
	void
		Processor::
		set_state( const Processor::ProcessorState& ps )
		throw()
	{
		state_ = ps;
	}

	// ----------------------------------------------------------------------
	Processor::ProcessorState
		Processor::
		state( void )
		const throw()
	{
		return state_;
	}

	// ----------------------------------------------------------------------
	bool
		Processor::
		auto_terminate(void)
		const throw()
	{
		return auto_terminate_;
	}

	// ----------------------------------------------------------------------
	void
		Processor::
		set_auto_terminate( bool enable ) 
		throw()
	{
		auto_terminate_ = enable;   
	}

	// ----------------------------------------------------------------------
	int
		Processor::
		simulation_round( void )
		const throw()
	{
		return owner().simulation_round();
	}

	// ----------------------------------------------------------------------
	int
		Processor::
		id( void )
		const throw()
	{
		return owner().id();
	}

	// ----------------------------------------------------------------------
	void
		Processor::
		sync_to_tags( TagContainer& )
		throw()
	{ 
		ABORT_NOT_IMPLEMENTED; 
	}

	// ----------------------------------------------------------------------
	void
		Processor::
		sync_from_tags( const TagContainer& )
		throw( std::runtime_error )
	{ 
		ABORT_NOT_IMPLEMENTED; 
	}

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/processor.cpp,v $
* Version $Revision: 212 $
* Date    $Date: 2008-06-13 11:26:06 +0200 (Fri, 13 Jun 2008) $
*-----------------------------------------------------------------------
* $Log: processor.cpp,v $
*-----------------------------------------------------------------------*/
