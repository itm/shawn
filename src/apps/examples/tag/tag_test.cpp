/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/examples/tag/tag_test.h"
#ifdef ENABLE_EXAMPLES

#include "sys/world.h"
#include "sys/node.h"
#include <iostream>

using namespace shawn;
using namespace std;

namespace examples
{
	namespace tagtest
	{

		// ----------------------------------------------------------------------
		TagTestTask::
			TagTestTask()
		{}
	
		// ----------------------------------------------------------------------
		TagTestTask::
			~TagTestTask()
		{}

		// ----------------------------------------------------------------------
		std::string
			TagTestTask::
			name( void )
			const throw()
		{
			return "tagtest";
		}

		// ----------------------------------------------------------------------
		std::string
			TagTestTask::
			description( void )
			const throw()
		{
            return "";
		}

		// ----------------------------------------------------------------------
		void
			TagTestTask::
			run( shawn::SimulationController& sc )
			throw( std::runtime_error )
		{
			cout << "Simulation environment tags: " << endl;
			display( sc.environment().begin_tags(), sc.environment().end_tags(), 1);

			if( sc.has_world() ) 
			{
				cout << "World tags: " << endl;
				display( sc.world().begin_tags(), sc.world().end_tags(), 1);
				
				for ( shawn::World::const_node_iterator it = sc.world().begin_nodes(); it != sc.world().end_nodes(); ++it )
				{
					const Node& n = *it;
					cout << "Node id " << n.id() << " tags: " << endl;
					display( n.begin_tags(), n.end_tags(), 1);
				}
			}
		}
		
		// ----------------------------------------------------------------------
		void
			TagTestTask::
			indent(int indent)
		{
			for(int i = 0; i < indent; ++i)
				cout << "\t";
		}

		// ----------------------------------------------------------------------
		void
			TagTestTask::
			display(TagContainer::tag_iterator it, TagContainer::tag_iterator end, int _indent)
		{
			
			for( ;it!=end; ++it) 
			{
				TagHandle t = it->second;
				indent(_indent);
				cout << t->name() << " = " << t->encoded_content() << " [" << t->type_identifier() << "]" << endl;
				
				if( t->begin_tags() != t->end_tags() )
				{
					indent(_indent);
					cout << "Child tags: " << endl; 
					display(t->begin_tags(), t->end_tags(), _indent + 1);
				}
				
			}
			
			
		}
		
	}
}

#endif
