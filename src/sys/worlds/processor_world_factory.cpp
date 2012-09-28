/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/worlds/processor_world_factory.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/comm_models/manual_communication_model.h"
#include "sys/processor.h"
#include <sstream>

namespace shawn
{

   ProcessorWorldFactory::
   ProcessorWorldFactory()
      : count_( -1 )
   {}
   // ----------------------------------------------------------------------
   ProcessorWorldFactory::
   ~ProcessorWorldFactory()
   {}
   // ----------------------------------------------------------------------
   void
   ProcessorWorldFactory::
   set_node_count( int c )
      throw()
   {
      count_ = c;
   }
   // ----------------------------------------------------------------------
   int
   ProcessorWorldFactory::
   node_count( void )
      const throw()
   {
      return count_;
   }
   // ----------------------------------------------------------------------
   void
   ProcessorWorldFactory::
   add_processor_factory( const ProcessorFactoryHandle& pfh )
      throw()
   {
      processor_factories_.push_back( pfh );
   }
   // ----------------------------------------------------------------------
   Node*
   ProcessorWorldFactory::
   new_node( const Vec& pos, const std::string& l )
      const throw()
   {
      Node* n = new Node;

      n->set_real_position( pos );

      if( !l.empty() )
         n->set_label( l );

      for( std::list<ProcessorFactoryHandle>::const_iterator
              it    = processor_factories_.begin(),
              endit = processor_factories_.end();
           it != endit; ++it )
      {
         ProcessorHandle ph = (**it).create();
#ifdef ENABLE_LOGGING
         ph->set_logger_name( "processors." + (**it).name() );
#endif
         n->add_processor( ph );
      }

      return n;
   }
   // ----------------------------------------------------------------------
   void
   ProcessorWorldFactory::
   fill_world( World& w )
      throw()
   {
      //Node* last = NULL;
      for(int i=0; i<node_count(); ++i )
         w.add_node( *new_node( new_position(), "" ) );
   }
   // ----------------------------------------------------------------------
   Vec
   ProcessorWorldFactory::
   new_position( void )
      const throw()
   {
      return Vec(0.0,0.0);
   }
}

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/processor_world_factory.cpp,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: processor_world_factory.cpp,v $
 *-----------------------------------------------------------------------*/
