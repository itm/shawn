/**
 * @file log_movement_processor.h
 * Defines the class sumo::LogMovementProcessor.
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

#ifndef __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_H
#define __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

#include "sys/processor.h"
#include <iosfwd>

namespace sumo
{

/**
 * A processor that prints the position of its owning node into a log file.
 *
 * The log file is "named movement.log".
 */
class LogMovementProcessor : public shawn::Processor
{
public:
   /**
    * Initialises the processor.
    *
    * This method tests whether the log file is already open. If not, it opens
    * the log file for output and prints the head line.
    */
   LogMovementProcessor();

   /**
    * Cleans up the processor.
    */
   virtual
   ~LogMovementProcessor();

   /**
    * Writes the current position of the node into the log file.
    *
    * The log file format is:
    * <pre>  simulation round, node label, position vector</pre>
    */
   virtual void
   work() throw ();

private:
   /**
    * The output stream to the log file.
    *
    * There is only one stream for all processors.
    */
   static std::ofstream log_stream_;
};

} /* namespace sumo */

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_H */
