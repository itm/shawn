/**
 * @file log_movement_processor_factory.cpp
 * Implements the class sumo::LogMovementProcessorFactory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 416 $
 * $HeadURL: https://www.ldv.ei.tum.de/repos/vertrauenstuds/shawn-apps/trunk/sumo/sumo_node_movement.h $
 * $Id: sumo_node_movement.h 416 2010-09-01 22:12:09Z wbam $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "log_movement_processor_factory.h"
#ifdef ENABLE_SUMO

#include "log_movement_processor.h"

using namespace shawn;

namespace sumo
{

LogMovementProcessorFactory::LogMovementProcessorFactory()
{
}

LogMovementProcessorFactory::~LogMovementProcessorFactory()
{
}

std::string
LogMovementProcessorFactory::name() const throw ()
{
   return "log_movement";
}

std::string
LogMovementProcessorFactory::description() const throw ()
{
   return "A simple processor to log the movements of the node for debugging";
}

Processor*
LogMovementProcessorFactory::create() throw ()
{
   return new LogMovementProcessor();
}

} /* namespace sumo */

#endif /* ENABLE_SUMO */
