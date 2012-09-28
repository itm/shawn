/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_READING
#include "apps/reading/readings/reading_factory.h"

namespace reading
{
   ReadingFactory::
   ReadingFactory( void )
   {}
   // ----------------------------------------------------------------------
   ReadingFactory::
   ~ReadingFactory()
   {}
   // ----------------------------------------------------------------------
   std::string
   ReadingFactory::
   name( void )
      const throw()
   {
      return "reading";
   }
   // ----------------------------------------------------------------------
   std::string
   ReadingFactory::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   Reading*
   ReadingFactory::
   find_reading( std::string name )
   {
      ReadingMap::const_iterator it = reading_map_.find(name);
         return it==reading_map_.end()
            ? NULL
            : it->second;
   }
   // ----------------------------------------------------------------------
   void
   ReadingFactory::
   set_world( shawn::World* world )
   {
      world_ = world;
   }

}

#endif
