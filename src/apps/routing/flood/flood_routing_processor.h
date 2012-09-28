/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_PROCESSOR_H
#define __SHAWN_APPS_ROUTING_FLOOD_FLOOD_ROUTING_PROCESSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_ROUTING

#include "sys/message.h"

#include "sys/processor.h"

namespace routing
{
	namespace flood
	{
		class FloodRoutingProcessor : public shawn::Processor
		{
		public:
			FloodRoutingProcessor();
			virtual ~FloodRoutingProcessor();
			virtual void boot( void ) throw();
			virtual bool process_message( const shawn::ConstMessageHandle& ) throw();
			virtual void work( void ) throw();
		};// End of FloodRoutingProcessor
	}
}
#endif
#endif

