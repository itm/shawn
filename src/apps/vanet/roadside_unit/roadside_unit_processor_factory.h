/**
 * @file roadside_unit_processor_factory.h
 * Defines the class vanet::RoadsideUnitProcessorFactory.
 *
 * @author Walter Bamberger
 *
 * $Revision: 533 $
 * $HeadURL: https://shawn.svn.sourceforge.net/svnroot/shawn/src/apps/vanet/roadside_unit/roadside_unit_processor_factory.h $
 * $Id: roadside_unit_processor_factory.h 533 2011-04-11 18:50:13Z simsiem $
 *
 * @par License
 * This file is part of the network simulator Shawn.<br>
 * Copyright (C) 2010, 2011 Technische Unversitaet Muenchen (www.tum.de)<br>
 * All rights reserved.<br>
 * Shawn is free software; you can redistribute it and/or modify it
 * under the terms of the BSD License. Refer to the shawn-licence.txt
 * file in the root of the Shawn source tree for further details.
 */

#ifndef __SHAWN_APPS_VANET_ROADSIDE_UNIT_PROCESSOR_FACTORY_H
#define __SHAWN_APPS_VANET_ROADSIDE_UNIT_PROCESSOR_FACTORY_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_VANET

#include "sys/processors/processor_factory.h"

namespace shawn
{
    class SimulationController;
}

namespace vanet
{

class RoadsideUnitProcessorFactory : public shawn::ProcessorFactory
{
public:
   RoadsideUnitProcessorFactory();
   virtual
   ~RoadsideUnitProcessorFactory();

   virtual std::string name( void ) const throw();
   virtual std::string description( void ) const throw();
   virtual shawn::Processor* create( void ) throw();

   static void register_factory( shawn::SimulationController& ) throw();
};

}

#endif /* ENABLE_VANET */
#endif /* __SHAWN_APPS_VANET_ROADSIDE_UNIT_PROCESSOR_FACTORY_H */
