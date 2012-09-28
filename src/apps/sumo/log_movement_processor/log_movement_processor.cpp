/**
 * @file log_movement_processor.cpp
 * Implements the class sumo::LogMovementProcessor.
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

#include "log_movement_processor.h"
#ifdef ENABLE_SUMO

#include "sys/node.h"
#include <fstream>

using namespace std;

namespace sumo
{

ofstream LogMovementProcessor::log_stream_;

// -----------------------------------------------------------------------------

LogMovementProcessor::LogMovementProcessor()
{
   if ( !log_stream_.is_open() )
   {
      log_stream_.open("movement.log");
      log_stream_ << "simulation round, node label, position vector" << endl;
   }
}

LogMovementProcessor::~LogMovementProcessor()
{
}

void
LogMovementProcessor::work() throw ()
{
   log_stream_ << owner().simulation_round() << ", " << owner().label() << ": "
            << owner().movement().position() << endl;
}

} /* namespace sumo */

#endif /* ENABLE_SUMO */
