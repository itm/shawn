/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "tree_routing_processor.h"
#ifdef ENABLE_ROUTING

#include "sys/node.h"

#include "apps/routing/routing_keeper.h"

#include "apps/routing/tree/tree_routing_factory.h"
#include "apps/routing/tree/tree_routing_processor.h"
#include "apps/routing/tree/tree_routing_message.h"

using namespace shawn;
using namespace std;

namespace routing
{
	namespace tree
	{
		// TreeRoutingProcessor:

		TreeRoutingProcessor::
			TreeRoutingProcessor() 
		{}
		// ----------------------------------------------------------------------
		TreeRoutingProcessor::
			~TreeRoutingProcessor()
		{}
		// ----------------------------------------------------------------------
		void
			TreeRoutingProcessor::
			boot( void )
			throw()
		{
			routing::RoutingKeeper& rk = routing::routing_keeper_w( owner_w().world_w().simulation_controller_w() );
			for( RoutingKeeper::HandleMapType::iterator it = rk.begin_handles_w(); 
				 it != rk.end_handles_w(); ++it )
			{
				TreeRouting* routing_instance = dynamic_cast<TreeRouting*>( it->second.get() );
				if( routing_instance )
				{
					routing_instance->boot( owner_w() );
				}
			}
		}
		// ----------------------------------------------------------------------
		bool
			TreeRoutingProcessor::
			process_message( const ConstMessageHandle& mh ) 
			throw()
		{
			const TreeRoutingMessage* trm = dynamic_cast<const TreeRoutingMessage*>( mh.get() );
			if(trm)
			{
				TreeRouting* routing_instance = static_cast<TreeRouting*>( trm->routing_instance_w() );
				return routing_instance->process_tree_routing_message(owner_w(),*trm);
			}
			const TreeConstructionMessage* tcm = dynamic_cast<const TreeConstructionMessage*>( mh.get() );
			if(tcm)
			{
				return tcm->routing_instance_w()->process_tree_construction_message(owner_w(),*tcm);
			}
			return Processor::process_message( mh );
		}
		// ----------------------------------------------------------------------
		void
			TreeRoutingProcessor::
			work( void )
			throw()
		{
			//
		}
	}
}
#endif

