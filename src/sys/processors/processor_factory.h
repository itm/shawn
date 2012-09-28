/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_PROCESSOR_FACTORY_H
#define __SHAWN_SYS_PROCESSOR_FACTORY_H

#include <string>

#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"


namespace shawn
{
   class Processor;
   class ProcessorKeeper;

   DECLARE_HANDLES(ProcessorFactory);

   class ProcessorFactory
      : public KeeperManaged
   {
   public:
      virtual ~ProcessorFactory();
      virtual Processor* create( void ) throw() = 0;
   };


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/processors/processor_factory.h,v $
 * Version $Revision: 38 $
 * Date    $Date: 2007-06-08 14:30:12 +0200 (Fri, 08 Jun 2007) $
 *-----------------------------------------------------------------------
 * $Log: processor_factory.h,v $
 *-----------------------------------------------------------------------*/
