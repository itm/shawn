/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_READING

#include <limits>
#include "apps/reading/test/create_random_double_reading.h"
#include "apps/reading/test/random_double_test_processor.h"
#include "apps/reading/readings/reading_keeper.h"
#include "apps/reading/readings/simple_reading_random.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"
#include "sys/taggings/basic_tags.h"
#include "sys/world.h"
#include "sys/node.h"
#include "sys/vec.h"


namespace reading
{

   SimulationTaskReadingDoubleTestCreate::
   SimulationTaskReadingDoubleTestCreate()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskReadingDoubleTestCreate::
   ~SimulationTaskReadingDoubleTestCreate()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskReadingDoubleTestCreate::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {

//      shawn::Node *node = NULL;
//      shawn::Processor *proc = NULL;

//      for( shawn::World::node_iterator
//               it = sc.world_w().begin_nodes_w();
//               it != sc.world_w().end_nodes_w();
//               ++it )
//      {
//         node = &(*it);
//         for( shawn::Node::ProcessorList::iterator
//               pit = node->begin_processors_w();
//               pit != node->end_processors_w();
//               ++pit )
//         {
//            proc = (*pit).get();
//            reading::DoubleReadingTestProcessor *tp = dynamic_cast<DoubleReadingTestProcessor*>(proc);
//            const DoubleReadingRandomFactory *dr = dynamic_cast<const DoubleReadingRandomFactory*>(sc.reading_keeper().find("double_reading_random").get());
//            dr->create("test");
//            if(proc && dr)
//            {
//               tp->attach_reading(*dr);
//            }
//         }
//      }
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskReadingDoubleTestCreate::
   name( void )
      const throw()
   {
      return std::string( "create_reading_double_test_sensors" );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskReadingDoubleTestCreate::
   description( void )
      const throw()
   {
      return std::string( "..." );
   }

}// namespace reading

#endif
