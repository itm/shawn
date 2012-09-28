/**
 * @file vehicle_processor_factory.h
 * Defines vanet::VehicleProcessorFactory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/vehicle_processor_factory.h $
 * $Id: vehicle_processor_factory.h 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_VANET_PROCESSOR_FACTORY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/processors/processor_factory.h"
#include <map>
#include <list>

namespace shawn
{
class SimulationController;
}

namespace reading
{
class Sensor;
}

namespace vanet
{

class KnowledgeBase;
class Disseminator;

class VehicleProcessorFactory : public shawn::ProcessorFactory
{
public:
   VehicleProcessorFactory( const shawn::SimulationController& sc );

   virtual
   ~VehicleProcessorFactory();

   virtual std::string
   name( void ) const throw ();

   virtual std::string
   description( void ) const throw ();

   virtual shawn::Processor*
   create( void ) throw ();

   static void
   register_factory( shawn::SimulationController& ) throw ();

private:
   friend class StatisticsTask;

   const shawn::SimulationController& simulationController_;
};

}

#endif
#endif
