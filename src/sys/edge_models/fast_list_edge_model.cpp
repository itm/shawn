/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/edge_models/fast_list_edge_model.h"
#include "sys/edge_models/grid_edge_model.h"

namespace shawn
{


	FastListEdgeModel::
		FastListEdgeModel( GridEdgeModel* gem )
		: grid_model_( gem )
	{
		assert( gem != NULL );
	}
	// ----------------------------------------------------------------------
	FastListEdgeModel::
		~FastListEdgeModel()
	{
		delete grid_model_;
	}
	// ----------------------------------------------------------------------
	void
		FastListEdgeModel::
		add_node_neighbors( Node& v, const Vec& pos, const Vec& velo )
		throw()
	{
      //		std::cout << " flem.begin" << std::endl << std::flush;

		grid_model_->observer_initial_zone(v, pos, velo);
		for( GridEdgeModel::adjacency_iterator
			it    = grid_model_->begin_adjacent_nodes_w(v, CD_ANY),
			endit = grid_model_->end_adjacent_nodes_w(v);
			it != endit; ++it )
				add_edge(v,*it);
//		std::cout << " flem.end" << std::endl << std::flush;
	}
	// ----------------------------------------------------------------------
	void
		FastListEdgeModel::
		set_world( World& w )
		throw()
	{
		grid_model_->set_world(w);
		ListEdgeModel::set_world(w);
	}
	// ----------------------------------------------------------------------
	void
		FastListEdgeModel::
		set_communication_model( const CommunicationModel& cm )
		throw()
	{
		grid_model_->set_communication_model(cm);
		ListEdgeModel::set_communication_model(cm);
	}
	// ----------------------------------------------------------------------
	void 
		FastListEdgeModel::
		init( void ) 
		throw()
	{
		grid_model_->init();
		ListEdgeModel::init();
	}
   // ----------------------------------------------------------------------  
   void
   FastListEdgeModel::
   node_added( Node& )
      throw()
   {
      // do nothing, the important 'add-method' here is add_edge()
   }
   // ----------------------------------------------------------------------
   void
   FastListEdgeModel::
   node_removed( Node& )
      throw()
   {
      ABORT_NOT_IMPLEMENTED;
   }  

}

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/sys/edge_models/fast_list_edge_model.cpp,v $
* Version $Revision: 38 $
* Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
*-----------------------------------------------------------------------
* $Log: fast_list_edge_model.cpp,v $
 *-----------------------------------------------------------------------*/
