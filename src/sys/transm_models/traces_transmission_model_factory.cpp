/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/traces_transmission_model.h"
#include "sys/transm_models/traces_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

namespace shawn
{

   TracesTransmissionModelFactory::
   TracesTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   TracesTransmissionModelFactory::
   ~TracesTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   TracesTransmissionModelFactory::
   name( void )
      const throw()
   {
      return std::string("traces");
   }
   // ----------------------------------------------------------------------
   std::string
   TracesTransmissionModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A TracesTransmissionModel (\"Traces\")");
   }
   // ----------------------------------------------------------------------
   TransmissionModel*
   TracesTransmissionModelFactory::
   create( const SimulationController& sc)
      throw()
   {
      std::string filename = sc.environment().required_string_param("traces");
      return new TracesTransmissionModel(filename);
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/Traces_transmission_model_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fr, 08. Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: Traces_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
