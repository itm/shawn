/**
 * @file roadside_unit_processor_factory.cpp
 * Implements the class vanet::RoadsideUnitProcessorFactory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/roadside_unit/roadside_unit_processor_factory.cpp $
 * $Id: roadside_unit_processor_factory.cpp 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "roadside_unit_processor_factory.h"
#ifdef ENABLE_VANET

#include "roadside_unit_processor.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/processors/processor_keeper.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace vanet
{

RoadsideUnitProcessorFactory::RoadsideUnitProcessorFactory()
{
   DEBUG(logger(), "RoadsideUnitProcessorFactory ctor");
}

// ----------------------------------------------------------------------

RoadsideUnitProcessorFactory::~RoadsideUnitProcessorFactory()
{
   DEBUG(logger(), "RoadsideUnitProcessorFactory dtor");
}

// ----------------------------------------------------------------------

string
RoadsideUnitProcessorFactory::
name( void )
   const throw()
{
   return "roadside_unit";
}

// ----------------------------------------------------------------------

string
RoadsideUnitProcessorFactory::
description( void )
   const throw()
{
   return "Processor for a roadside unit in a VANET";
}

// ----------------------------------------------------------------------

Processor*
RoadsideUnitProcessorFactory::
create( void )
   throw()
{
   return new RoadsideUnitProcessor();
}

// ----------------------------------------------------------------------

void
RoadsideUnitProcessorFactory::
register_factory( SimulationController& sc )
   throw()
{
   sc.processor_keeper_w().add( new RoadsideUnitProcessorFactory );
}

}

#endif /* ENABLE_VANET */
