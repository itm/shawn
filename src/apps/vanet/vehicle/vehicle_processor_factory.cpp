/**
 * @file vehicle_processor_factory.cpp
 * Implements vanet::VehicleProcessorFactory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 539 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/vehicle/vehicle_processor_factory.cpp $
 * $Id: vehicle_processor_factory.cpp 539 2011-04-13 01:26:31Z andylz $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#include "vehicle_processor_factory.h"
#ifdef ENABLE_VANET

#include "value_type_int_sensor.h"
#include "vehicle_processor.h"
#include "../dissemination/disseminator.h"
#include "../knowledge_base/knowledge_base.h"
#include "sys/processors/processor_keeper.h"
#include "sys/simulation/simulation_controller.h"

using namespace std;
using namespace shawn;

namespace vanet
{

// ----------------------------------------------------------------------

void
VehicleProcessorFactory::register_factory( SimulationController& sc ) throw ()
{
   sc.processor_keeper_w().add(new VehicleProcessorFactory(sc));
}

// ----------------------------------------------------------------------

VehicleProcessorFactory::VehicleProcessorFactory(
         const SimulationController& sc ) :
   simulationController_(sc)
{
   DEBUG(logger(), "VehicleProcessorFactory ctor");
}

// ----------------------------------------------------------------------

VehicleProcessorFactory::~VehicleProcessorFactory()
{
   DEBUG(logger(), "VehicleProcessorFactory dtor");
   for ( map<string, VehicleProcessor::Components>::iterator componentsIt =
            VehicleProcessor::labelComponentsMap.begin(); componentsIt
            != VehicleProcessor::labelComponentsMap.end(); ++componentsIt )
   {
      delete componentsIt->second.knowledgeBase;
      delete componentsIt->second.disseminator;
      delete componentsIt->second.recommendationDisseminator;
      delete componentsIt->second.sensor;
   }
}

// ----------------------------------------------------------------------

std::string
VehicleProcessorFactory::name( void ) const throw ()
{
   return "vehicle";
}

// ----------------------------------------------------------------------

std::string
VehicleProcessorFactory::description( void ) const throw ()
{
   return "Processor for a vehicle in a VANET";
}

// ----------------------------------------------------------------------

shawn::Processor*
VehicleProcessorFactory::create( void ) throw ()
{
   return new VehicleProcessor();
}

}

#endif
