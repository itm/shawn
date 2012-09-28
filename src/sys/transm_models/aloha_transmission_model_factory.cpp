/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/transm_models/aloha_transmission_model.h"
#include "sys/transm_models/aloha_transmission_model_factory.h"
#include "sys/transm_models/transmission_model_keeper.h"
#include "sys/simulation/simulation_controller.h"

namespace shawn
{

   AlohaTransmissionModelFactory::
   AlohaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   AlohaTransmissionModelFactory::
   ~AlohaTransmissionModelFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   AlohaTransmissionModelFactory::
   name( void )
      const throw()
   {
      return std::string("aloha");
   }
   // ----------------------------------------------------------------------
   std::string
   AlohaTransmissionModelFactory::
   description( void ) 
      const throw()
   {
      return std::string("A AlohaTransmissionModel (\"aloha\") delivers messages without delay but there is packet loss");
   }
   // ----------------------------------------------------------------------
   TransmissionModel*
   AlohaTransmissionModelFactory::
   create( const SimulationController& )
      throw()
   {
      return new AlohaTransmissionModel;
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/transm_models/aloha_transmission_model_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: aloha_transmission_model_factory.cpp,v $
 *-----------------------------------------------------------------------*/
