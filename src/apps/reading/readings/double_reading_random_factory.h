/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __DOUBLE_READING_RANDOM_FACTORY_H
#define __DOUBLE_READING_RANDOM_FACTORY_H
#include "../buildfiles/_apps_enable_cmake.h"

#ifdef ENABLE_READING
#include <string>
#include <map>
#include "sys/util/keeper_managed.h"
#include "sys/util/refcnt_pointer.h"
#include "sys/simulation/simulation_controller.h"
#include "apps/reading/readings/reading_factory.h"
#include "apps/reading/readings/simple_reading_random.h"

namespace reading
{
   class DoubleReadingRandomFactory
         : public ReadingFactory
   {
   public:
      DoubleReadingRandomFactory( void );
      virtual ~DoubleReadingRandomFactory();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual DoubleReadingRandom* create( std::string name );
   };
   DECLARE_HANDLES(DoubleReadingRandomFactory);
}
#endif
#endif
