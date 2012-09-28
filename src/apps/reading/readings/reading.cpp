/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "apps/reading/readings/reading.h"

namespace reading
{
   Reading::Reading()
   {}
   // ----------------------------------------------------------------------
   Reading::~Reading()
   {
   	world_ = NULL;
	delete world_;
   }
   // ----------------------------------------------------------------------
   std::string
   Reading::
   name( void )
      const throw()
   {
      return "reading";
   }
   // ----------------------------------------------------------------------
   std::string
   Reading::
   description( void )
      const throw()
   {
      return "XXX";
   }
   // ----------------------------------------------------------------------
   void Reading::set_world( shawn::World& w ) throw()
   {
        world_ = &w;
   }
   // ----------------------------------------------------------------------
   void
   Reading::
   init( void )
      throw()
   {
      assert( world_ != NULL );
   }
   // ----------------------------------------------------------------------
   bool
   Reading::
   has_world( void )
   const throw()
    {
        return world_ != NULL;
    }
   // ----------------------------------------------------------------------
   const shawn::World& Reading::world( void )const throw()
   {
        return *world_;
   }
   // ----------------------------------------------------------------------
   shawn::World& Reading::world_w( void ) throw()
   {
        return *world_;
   }
}

#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/reading.cpp,v $
 * Version $Revision: 287 $
 * Date    $Date: 2008-11-16 11:35:44 +0100 (So, 16. Nov 2008) $
 *-----------------------------------------------------------------------
 * $Log: reading.cpp,v $
 *-----------------------------------------------------------------------*/
