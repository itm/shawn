/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/misc/localization/dump_location.h"
#include "sys/world.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_environment.h"

#include <fstream>

namespace shawn
{

   SimulationTaskDumpLocation::
   SimulationTaskDumpLocation()
   {}
   // ----------------------------------------------------------------------
   SimulationTaskDumpLocation::
   ~SimulationTaskDumpLocation()
   {}
   // ----------------------------------------------------------------------
   void
   SimulationTaskDumpLocation::
   run( SimulationController& sc )
      throw( std::runtime_error )
   {
      require_world( sc );

      std::string fname  = sc.environment().required_string_param("dloc_out_file");
      dump_estimates_    = sc.environment().optional_int_param("dloc_print_estimates", 1) != 0;
      skip_unpositioned_ = sc.environment().optional_int_param("dloc_skip_unpositioned", 0) != 0;
      colsep_            = sc.environment().optional_string_param("dloc_col_sep", "\t");
      unpositioned_mark_ = sc.environment().optional_string_param("dloc_unpositioned_mark", 
							std::string("?")+colsep_+std::string("?")+colsep_+std::string("?"));

      std::ofstream out( fname.c_str() );
      if( !out )
         throw std::runtime_error(std::string("cannot write file '") + fname + std::string("'"));

      for( World::const_node_iterator it = sc.world().begin_nodes(), endit = sc.world().end_nodes(); it != endit; ++it )
         if( (!skip_unpositioned_) || (!dump_estimates_) || it->has_est_position() )
            {
               print_label(out,*it);
               out << colsep_;
               print_pos(out,*it);
               out << std::endl;
            }
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDumpLocation::
   name( void )
      const throw()
   {
      return "dump_location";
   }
   // ----------------------------------------------------------------------
   std::string
   SimulationTaskDumpLocation::
   description( void ) 
      const throw()
   {
      return "dump estimated positions (or real ones if !($dloc_print_estimates)) to $dloc_out_file";
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskDumpLocation::
   print_label( std::ostream& os, const Node& v )
      throw()
   {
      os<<v.label();
   }
   // ----------------------------------------------------------------------
   void
   SimulationTaskDumpLocation::
   print_pos( std::ostream& os, const Node& v )
      throw()
   {
      Vec pos;

      if( dump_estimates_ )
         {
            if( v.has_est_position() )
               pos=v.est_position();
            else
               {
                  os << unpositioned_mark_;
                  return;
               }
         }
         else
            pos = v.real_position();
      os << pos.x() << colsep_ << pos.y() << colsep_ << pos.z();
   }

}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/localization/dump_location.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: dump_location.cpp,v $
 *-----------------------------------------------------------------------*/
