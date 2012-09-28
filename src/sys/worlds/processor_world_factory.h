/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_WORLDS_PROCESSOR_WORLD_FACTORY_H
#define __SHAWN_SYS_WORLDS_PROCESSOR_WORLD_FACTORY_H

#include "sys/world_factory.h"
#include "sys/processors/processor_factory.h"

#include <list>

namespace shawn
{
   class Vec;
   class Node;
   class ProcessorFactory;

   class ProcessorWorldFactory
      : public WorldFactory
   {
   public:
      ProcessorWorldFactory();
      virtual ~ProcessorWorldFactory();

      virtual void set_node_count( int ) throw();
      virtual int node_count( void ) const throw();

      virtual void add_processor_factory( const ProcessorFactoryHandle& ) throw();

      virtual void fill_world( World& ) throw();

   protected:
      virtual Vec new_position( void ) const throw();

      virtual Node* new_node( const Vec&, const std::string& label="" ) const throw();


   private:
      int count_;
      std::list<ProcessorFactoryHandle> processor_factories_;
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/worlds/processor_world_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: processor_world_factory.h,v $
 *-----------------------------------------------------------------------*/
