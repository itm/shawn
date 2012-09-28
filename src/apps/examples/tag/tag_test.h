/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_EXAMPLES_TAG_TAGTEST_H__
#define __SHAWN_APPS_EXAMPLES_TAG_TAGTEST_H__
#include "_apps_enable_cmake.h"
#ifdef ENABLE_EXAMPLES

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/tag.h"

namespace examples
{
	namespace tagtest 
	{
		class TagTestTask
			: public shawn::SimulationTask
		{
		public:
			TagTestTask();
			virtual ~TagTestTask();

			virtual std::string name( void ) const throw();
			virtual std::string description( void ) const throw();

			virtual void run( shawn::SimulationController& sc ) throw( std::runtime_error );

		private:
			void display(shawn::TagContainer::tag_iterator it, shawn::TagContainer::tag_iterator end, int indent);
			void indent(int indent);
		};
	}
}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/exampletask/exampletask.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: exampletask.h,v $
 *-----------------------------------------------------------------------*/
