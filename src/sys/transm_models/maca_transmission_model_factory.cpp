/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/maca_transmission_model.h"
#include "sys/transm_models/maca_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   MacaTransmissionModelFactory::
   MacaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   MacaTransmissionModelFactory::
   ~MacaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   MacaTransmissionModelFactory::
   name( void )
      const throw()
   {
      return std::string("maca");
   }
   // ----------------------------------------------------------------------
   std::string
   MacaTransmissionModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A MacaTransmissionModel (\"maca\") avoids packet loss by sending RTS and CTS.");
   }
   // ----------------------------------------------------------------------
   TransmissionModel*
   MacaTransmissionModelFactory::
   create( const SimulationController& )
      throw()
   {
      return new MacaTransmissionModel;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/maca_transmission_model_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: maca_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
