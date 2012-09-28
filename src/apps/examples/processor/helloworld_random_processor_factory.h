/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_HELLOWORLD_RANDOM_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_HELLOWORLD_RANDOM_PROCESSOR_FACTORY_H
#ifdef ENABLE_EXAMPLES

#include "sys/processors/processor_factory.h"

namespace shawn
{
    class SimulationController;
}


namespace helloworld
{

	//--------------------------------------------------------------------------
	/**This class is used to create HelloworldRandomProcessor. It is added
	 * to the SimulationController.
	**/
   class HelloworldRandomProcessorFactory
      : public shawn::ProcessorFactory
   {
	public:
	   HelloworldRandomProcessorFactory(shawn::SimulationController& sc);
	   virtual ~HelloworldRandomProcessorFactory();

	   virtual std::string name( void ) const throw();
	   virtual std::string description( void ) const throw();
	   virtual shawn::Processor* create( void ) throw();

	   static void register_factory( shawn::SimulationController& ) throw();
	private:
      shawn::SimulationController* sc_;

	};

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/helloworld/helloworld_processor_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: helloworld_processor_factory.h,v $
 *-----------------------------------------------------------------------*/
