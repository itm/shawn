/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/readings/double_reading_random_factory.h"

namespace reading
{
   DoubleReadingRandomFactory::
   DoubleReadingRandomFactory( void )
   {}
   // ----------------------------------------------------------------------
   DoubleReadingRandomFactory::
   ~DoubleReadingRandomFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   DoubleReadingRandomFactory::
   name( void )
      const throw()
   {
      return "double_reading_random";
   }
   // ----------------------------------------------------------------------
   std::string
   DoubleReadingRandomFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }

   DoubleReadingRandom*
   DoubleReadingRandomFactory::
   create( std::string name )
   {
      Reading *base_reading = find_reading(name);
      DoubleReadingRandom *out_reading = NULL;

      if(base_reading == NULL)
      {
         out_reading = new DoubleReadingRandom();
         reading_map_[name] = out_reading;
         out_reading->set_world(*world_);
         out_reading->init();
      }
      else
      {
         out_reading = dynamic_cast<DoubleReadingRandom *>(base_reading);
      }

      return out_reading;
   }
}

#endif
