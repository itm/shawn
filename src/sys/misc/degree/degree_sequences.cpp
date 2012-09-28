/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/degree/degree_sequences.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"

#include <fstream>

using namespace std;

namespace shawn
{

   SimulationTaskDegreeSequence::
   SimulationTaskDegreeSequence()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskDegreeSequence::
   ~SimulationTaskDegreeSequence()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskDegreeSequence::
   run( SimulationController& sc ) 
      throw( std::runtime_error )
   {
      require_world(sc);

      std::string fn = sc.environment().required_string_param("ds_out_file");
      
      ofstream out(fn.c_str());
      if( !out )
         throw std::runtime_error(std::string("cannot write to ")+fn);

      for( World::const_node_iterator
              it    = sc.world().begin_nodes(),
              endit = sc.world().end_nodes();
           it != endit; ++it )
         {
            const Node& v = *it;
            out << v.real_position().x() << "\t"
                << v.real_position().y() << "\t"
                << v.degree() << "\t"
                << v.id() << "\t"
                << v.label() << endl;
         }
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDegreeSequence::
   name( void )
      const throw()
   {
      return std::string( "degree_sequence" );
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDegreeSequence::
   description( void ) 
      const throw()
   {
      return std::string( "writes degree sequence to $ds_out_file" );
   }


}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/degree/degree_sequences.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: degree_sequences.cpp,v $
 *-----------------------------------------------------------------------*/
