/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE

#include "apps/testbedservice/virtual_links/virtual_link_transmission_model_factory.h"
#include "apps/testbedservice/virtual_links/virtual_link_transmission_model.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace testbedservice
{

   VirtualLinkTransmissionModelFactory::
   VirtualLinkTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   VirtualLinkTransmissionModelFactory::
   ~VirtualLinkTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   VirtualLinkTransmissionModelFactory::
   name( void )
      const throw()
   {
      return std::string("virtual_link");
   }
   // ----------------------------------------------------------------------
   std::string
   VirtualLinkTransmissionModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("Inject 'virtual' messages via a webservices.");
   }
   // ----------------------------------------------------------------------
   shawn::TransmissionModel*
   VirtualLinkTransmissionModelFactory::
   create( const shawn::SimulationController& sc)
      throw()
   {
       return new VirtualLinkTransmissionModel();
   }

}

#endif
