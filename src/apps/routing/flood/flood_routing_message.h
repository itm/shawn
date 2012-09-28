/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_MESSAGE_H
#define __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_MESSAGE_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "apps/routing/flood/flood_routing.h"

namespace routing
{
	namespace flood
	{
		class FloodRoutingMessage : public FloodRouting::RoutingMessage
		{
			friend class FloodRouting;
			friend std::ostream& operator<<(std::ostream& os, const FloodRoutingMessage& frm);
		public:
			FloodRoutingMessage( RoutingBase& routing_instance,
								 const shawn::ConstMessageHandle& application_message,
								 int time_to_live );
			FloodRoutingMessage( const FloodRoutingMessage& other );
			virtual ~FloodRoutingMessage();
			virtual int size( void ) const throw();
			virtual int time_to_live() const throw();
		protected:
			//
		private:
			FloodRoutingMessage();
			static const int MESSAGE_OVERHEAD;
			static const bool USE_ROUTING_MESSAGE_OVERHEAD;
		};
	}
}
#endif
#endif
