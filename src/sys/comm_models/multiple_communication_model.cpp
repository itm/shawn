/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <limits>

#include "sys/comm_models/multiple_communication_model.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/util/defutils.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

	// ----------------------------------------------------------------------
	MultipleCommunicationModel::
		MultipleCommunicationModel()
		: default_(NULL)
	{}

	// ----------------------------------------------------------------------
	MultipleCommunicationModel::
		~MultipleCommunicationModel()
	{}

	// ----------------------------------------------------------------------
	void
		MultipleCommunicationModel::
		init( void )
		throw()
	{
        for(CommModelVecIterator it = cds_.begin(), end = cds_.end(); it!=end; ++it)
            (*it)->init();
	}

	// ----------------------------------------------------------------------
	void 
		MultipleCommunicationModel::
		add_comm_model(CommunicationModel& cm)
		 throw()
	{
		cds_.push_back(&cm);
	}

	// ----------------------------------------------------------------------			
	void 
		MultipleCommunicationModel::
		set_default_comm_model(CommunicationModel& m) 
		throw()
	{
		default_ = &m;
	}

	// ----------------------------------------------------------------------
	bool
		MultipleCommunicationModel::
		can_communicate_bidi( const Node& u, const Node& v )
		const throw()
	{
		for(CommModelVecIterator it = cds_.begin(), end = cds_.end(); it!=end; ++it)
			if( (*it)->in_domain(u,v) )
				return (*it)->can_communicate_bidi(u,v);

		if( default_ != NULL )
			return default_->can_communicate_bidi(u,v);

		return false;
	}

	// ----------------------------------------------------------------------
	bool
		MultipleCommunicationModel::
		can_communicate_uni( const Node& u, const Node& v )
		const throw()
	{
		ABORT_NOT_IMPLEMENTED;

		return false;
	}

	// ----------------------------------------------------------------------
	bool
		MultipleCommunicationModel::
		exists_communication_upper_bound( void )
		const throw()
	{
		for(CommModelVecIterator it = cds_.begin(), end = cds_.end(); it!=end; ++it)
			if (!(*it)->exists_communication_upper_bound())
				return false;

		if( default_ != NULL )
			return default_->exists_communication_upper_bound();

		return false;
	}

	// ----------------------------------------------------------------------
	double
		MultipleCommunicationModel::
		communication_upper_bound( void )
		const throw()
	{
		double upper = std::numeric_limits<double>::min(), tmp;

		for(CommModelVecIterator it = cds_.begin(), end = cds_.end(); it!=end; ++it)
			if ((*it)->exists_communication_upper_bound())
				if( (tmp = (*it)->communication_upper_bound()) > upper)
					upper = tmp;

		if( default_ != NULL )
			if (default_->exists_communication_upper_bound())
				if( (tmp = default_->communication_upper_bound()) > upper)
					upper = tmp;

		return upper;    
	}

	// ----------------------------------------------------------------------
	bool
		MultipleCommunicationModel::
		is_status_available_on_construction( void )
		const throw()
	{
		for(CommModelVecIterator it = cds_.begin(), end = cds_.end(); it!=end; ++it)
			if(! (*it)->is_status_available_on_construction() ) 
				return false;

		if( default_ != NULL )
			return default_->is_status_available_on_construction();

		return false;
	}


}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/comm_models/multiple_communication_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: multiple_communication_model.cpp,v $
 *-----------------------------------------------------------------------*/
