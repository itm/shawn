/**
 * @file log_movement_processor_factory.h
 * Defines the class sumo::LogMovementProcessorFactory.
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

#ifndef __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_FACTORY_H

#include "sys/processors/processor_factory.h"
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

namespace sumo
{

/**
 * The factory class for the LogMovementProcessor.
 */
class LogMovementProcessorFactory : public shawn::ProcessorFactory
{
public:
   /**
    * Initialises the LogMovementProcessorFactory.
    */
   LogMovementProcessorFactory();

   /**
    * Cleans up the LogMovementProcessorFactory.
    */
   virtual
   ~LogMovementProcessorFactory();

   /**
    * Returns the name of the LogMovementProcessor.
    */
   virtual std::string
   name() const throw ();

   /**
    * Returns a description of the LogMovementProcessor.
    */
   virtual std::string
   description() const throw ();

   /**
    * Creates a new LogMovementProcessor object.
    */
   virtual shawn::Processor*
   create() throw ();
};

} /* namespace sumo */

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_LOG_MOVEMENT_PROCESSOR_FACTORY_H */
