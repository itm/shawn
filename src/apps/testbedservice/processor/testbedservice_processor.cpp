/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TESTBEDSERVICE
#include "apps/testbedservice/processor/testbedservice_processor.h"
#include "apps/testbedservice/testbedservice_task.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/simulation/simulation_task_keeper.h"
#include "sys/node.h"
#include "sys/world.h"


namespace testbedservice
{
   TestbedServiceProcessor::
   TestbedServiceProcessor()
      : controller_ ( 0 ),
         node_      ( 0 )
   {}
   // ----------------------------------------------------------------------
   TestbedServiceProcessor::
   ~TestbedServiceProcessor()
   {}
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   testbedservice_proc_boot()
      throw()
   {
      shawn::SimulationTaskKeeper& stk =
         node_w().world_w().simulation_controller_w().simulation_task_keeper_w();

      TestbedServiceTask *tst = dynamic_cast<TestbedServiceTask*>(
                                    stk.find_w( "testbedservice" ).get() );
      if ( tst )
      {
         controller_ = &tst->testbedservice_client_w();
      }
      else
      {
         std::cerr << "'TestbedserviceKeeper' not found." << std::endl;
         abort();
      }

      std::cerr << "'TestbedServiceClient' successfully loaded." << std::endl;
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   process_message( TestbedserviceMessage& message )
      throw()
   {
      // should be overwritten by derived processors - otherwise text message
      // is just ignored
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   send_text_message( std::string& message )
      throw()
   {
//       std::stringstream ss;
//       ss << "urn:wisebed:uzl:" << 500 + node().id();
//       std::string source = ss.str();

      send_message( strlen(message.c_str()), (uint8_t*)message.c_str() );
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   send_message( int length, uint8_t *buffer )
      throw()
   {
      std::string source = node().label();
      controller().send_message( source, length, buffer );
   }
   // ----------------------------------------------------------------------
   void
   TestbedServiceProcessor::
   flash_program( FlashProgram& program )
      throw()
   {}

}
#endif
