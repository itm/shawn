/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_DEFAULT_NODE_GEN_H
#define __SHAWN_APPS_TOPOLOGY_DEFAULT_NODE_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/node_gen/node_gen.h"
#include "sys/processors/processor_factory.h"

#include <list>

namespace topology
{
   class Vec;
   class Node;
   class ProcessorFactory;

   class DefaultNodeGenerator
      : public NodeGenerator
   {
   public:
      typedef std::list<shawn::ProcessorFactoryHandle> ProcessorFactoryList;
      typedef std::list<shawn::ProcessorFactoryHandle>::iterator ProcessorFactoryListIterator;

      DefaultNodeGenerator();
      virtual ~DefaultNodeGenerator();

      virtual void pre_generate( shawn::SimulationController&,
                                 reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual shawn::Node* generate_node( const shawn::Vec& ) throw( std::runtime_error );
      virtual void post_generate( void ) throw( std::runtime_error );

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:

      virtual void init_processor_factories( shawn::SimulationController&,
                                             ProcessorFactoryList& )
         throw( std::runtime_error );
      virtual shawn::Node* allocate_node( void )
         throw( std::runtime_error );
      virtual void set_position( shawn::Node&, const shawn::Vec& )
         throw( std::runtime_error );
      virtual void label_node( shawn::Node& )
         throw( std::runtime_error );
      virtual void add_processors( shawn::Node&, ProcessorFactoryList& )
         throw( std::runtime_error );
      virtual void add_to_world( shawn::Node&, shawn::World& )
         throw( std::runtime_error );

   private:
      ProcessorFactoryList proc_factories_;
      int nodes_so_far_;
      std::string uuid_;
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/node_gen/default_node_gen.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: default_node_gen.h,v $
 *-----------------------------------------------------------------------*/
