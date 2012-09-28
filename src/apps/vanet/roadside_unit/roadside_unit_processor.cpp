/**
 * @file roadside_unit_processor.cpp
 * Implements the class vanet::RoadsideUnitProcessor.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/roadside_unit/roadside_unit_processor.cpp $
 * $Id: roadside_unit_processor.cpp 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "roadside_unit_processor.h"
#ifdef ENABLE_VANET
#include "../vanet_message.h"
#include "sys/node.h"
#include <iostream>

using namespace std;
using namespace shawn;

namespace vanet
{

RoadsideUnitProcessor::RoadsideUnitProcessor()
{
   DEBUG(logger(), "RoadsideUnitProcessor ctor");
}

RoadsideUnitProcessor::~RoadsideUnitProcessor()
{
   DEBUG(logger(), "RoadsideUnitProcessor dtor");
}

// ----------------------------------------------------------------------

void
RoadsideUnitProcessor::
boot( void )
   throw()
{
}

// ----------------------------------------------------------------------

bool
RoadsideUnitProcessor::
process_message( const ConstMessageHandle& mh )
   throw()
{
   return false;
}

// ----------------------------------------------------------------------

void
RoadsideUnitProcessor::
work( void )
   throw()
{
}

}

#endif /* ENABLE_VANET */
