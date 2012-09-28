/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/node_gen/default_node_gen.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/processor.h"
#include "sys/processors/processor_factory.h"
#include "sys/processors/processor_keeper.h"
#include "sys/misc/uuid.h"
#include "sys/misc/tokenize.h"
#include "sys/simulation/simulation_controller.h"

#include <sstream>

namespace topology
{

   DefaultNodeGenerator::
   DefaultNodeGenerator()
   {}
   // ----------------------------------------------------------------------
   DefaultNodeGenerator::
   ~DefaultNodeGenerator()
   {}
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   pre_generate( shawn::SimulationController&  sc,
                 reading::ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      NodeGenerator::pre_generate(sc,brh);
      init_processor_factories( sc, proc_factories_ );
      uuid_ = shawn::UUIDGenerator::uuid();
      nodes_so_far_=0;

      if( !sc.has_world() )
         throw std::runtime_error("DefaultNodeGenerator needs a world!");
   }
   // ----------------------------------------------------------------------
   shawn::Node*
   DefaultNodeGenerator::
   generate_node( const shawn::Vec& pos )
      throw( std::runtime_error )
   {
      shawn::Node* v = allocate_node();
      set_position(*v,pos);
      label_node(*v);
      add_processors(*v,proc_factories_);
      assert( simulation_controller_w().has_world() );
      add_to_world(*v,simulation_controller_w().world_w());
      return v;
   }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   post_generate( void )
      throw( std::runtime_error )
   {
      proc_factories_.clear();
      NodeGenerator::post_generate();
   }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   init_processor_factories( shawn::SimulationController& sc,
                             ProcessorFactoryList& pfl )
      throw( std::runtime_error )
   {
      shawn::StrTok tok( sc.environment().optional_string_param( "processors", "" ), ", " );
      for ( shawn::StrTok::iterator it = tok.begin(), end = tok.end();
            it != end;
            ++it )
      {
         shawn::ProcessorFactoryHandle pfh = sc.processor_keeper_w().find_w( *it );
         assert( pfh != NULL ); // not found throws...
         pfl.push_back( pfh );
      }
   }
   // ----------------------------------------------------------------------
   shawn::Node*
   DefaultNodeGenerator::
   allocate_node( void )
      throw( std::runtime_error )
   { return new shawn::Node; }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   set_position( shawn::Node& v, const shawn::Vec& pos )
      throw( std::runtime_error )
   { v.set_real_position(pos); }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   label_node( shawn::Node& v )
      throw( std::runtime_error )
   {
      std::ostringstream oss;
      oss << "v" << nodes_so_far_++ << "_" << uuid_;
      v.set_label(oss.str());
   }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   add_processors( shawn::Node& v, ProcessorFactoryList& pfl )
      throw( std::runtime_error )
   {
      for ( ProcessorFactoryListIterator it = pfl.begin(), end = pfl.end();
            it != end;
            ++it )
      {
         shawn::ProcessorHandle ph = (**it).create();
#ifdef ENABLE_LOGGING
         ph->set_logger_name( "processors." + (**it).name() );
#endif
         v.add_processor( ph );
      }
   }
   // ----------------------------------------------------------------------
   void
   DefaultNodeGenerator::
   add_to_world( shawn::Node& v, shawn::World& w )
      throw( std::runtime_error )
   {
      w.add_node(v);
   }
   // ----------------------------------------------------------------------
   std::string
   DefaultNodeGenerator::
   name( void )
      const throw()
   { return "default"; }
   // ----------------------------------------------------------------------
   std::string
   DefaultNodeGenerator::
   description( void )
      const throw()
   { return "default node generator"; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/node_gen/default_node_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: default_node_gen.cpp,v $
 *-----------------------------------------------------------------------*/
