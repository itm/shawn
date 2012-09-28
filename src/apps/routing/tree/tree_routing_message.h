/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_LEGACYAPPS_ROUTING_TREE_TREE_ROUTING_MESSAGE_H
#define __SHAWN_LEGACYAPPS_ROUTING_TREE_TREE_ROUTING_MESSAGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/message.h"

#include "apps/routing/tree/tree_routing.h"

namespace routing
{
	namespace tree
	{
		// For user data transmission
		class TreeRoutingMessage : public TreeRouting::RoutingMessage
		{ 
			friend std::ostream& operator<<(std::ostream& os, const TreeRoutingMessage& trm);
		public:
			TreeRoutingMessage( RoutingBase& routing_instance,
								const shawn::ConstMessageHandle& application_message, 
								const shawn::Node& next_hop, 
								const shawn::Node& sink,
								int time_to_live );
			TreeRoutingMessage( const TreeRoutingMessage& other, const shawn::Node& next_hop );
			virtual ~TreeRoutingMessage();
			const shawn::Node* next_hop() const throw();
			int time_to_live() const throw();
			virtual int size( void ) const throw();
		private:
			// member
			const shawn::Node* next_hop_;
			int time_to_live_;
			static const int MESSAGE_OVERHEAD;
			static const bool USE_ROUTING_MESSAGE_OVERHEAD;
		};// End of TreeRoutingMessage

		// For tree construction
		class TreeConstructionMessage : public shawn::Message
		{
			friend std::ostream& operator<<(std::ostream& os, const TreeConstructionMessage& tcm);
		public:
			TreeConstructionMessage( TreeRouting& ri, const shawn::Node& sink, double sink_creation_time );
			TreeConstructionMessage( const TreeConstructionMessage& other );
			virtual ~TreeConstructionMessage();
			const shawn::Node* sink() const throw();
			int hop_count() const throw();
			double sink_creation_time() const throw();
			const TreeRouting* routing_instance() const throw();
			TreeRouting* routing_instance_w() const throw();
		private:
			const shawn::Node* sink_;
			int hop_count_;
			double sink_creation_time_;
			mutable TreeRouting* routing_instance_;
			static const int MESSAGE_OVERHEAD;
		};// End of TreeConstructionMessage
	}
}

#endif
#endif
