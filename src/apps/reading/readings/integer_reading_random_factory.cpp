/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/readings/integer_reading_random_factory.h"

namespace reading
{
   IntegerReadingRandomFactory::
   IntegerReadingRandomFactory( void )
   {}
   // ----------------------------------------------------------------------
   IntegerReadingRandomFactory::
   ~IntegerReadingRandomFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   IntegerReadingRandomFactory::
   name( void )
      const throw()
   {
      return "integer_reading_random";
   }
   // ----------------------------------------------------------------------
   std::string
   IntegerReadingRandomFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }

   IntegerReadingRandom*
   IntegerReadingRandomFactory::
   create( std::string name )
   {
      Reading *base_reading = find_reading(name);
      IntegerReadingRandom *out_reading = NULL;

      if(base_reading == NULL)
      {
         out_reading = new IntegerReadingRandom();
         reading_map_[name] = out_reading;
         out_reading->set_world(*world_);
         out_reading->init();
      }
      else
      {
         out_reading = dynamic_cast<IntegerReadingRandom *>(base_reading);
      }

      return out_reading;
   }
}

#endif
