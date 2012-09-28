/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/cuboid_topology.h"
#include <sstream>

using namespace shawn;

namespace topology
{

   CuboidTopology::
   CuboidTopology( const Box& b,
                   const std::string& name )
      : box_  ( b ),
        name_ ( name )
   {
      if( name_.empty() )
         {
            std::ostringstream oss;
            oss << "cuboid_topology_" 
                <<        b.lower().x()
                << "_" << b.upper().x()
                << "_" << b.lower().y()
                << "_" << b.upper().x();
            name_=oss.str();
         }
      
      std::ostringstream oss;
      oss << "coboidal 3D topology on "
          << "["     << b.lower().x()
          << ","     << b.upper().x()
          << "] x [" << b.lower().y()
          << ","     << b.upper().y()
          << "] x [" << b.lower().z()
          << ","     << b.upper().z()
          << "]";
      description_=oss.str();
   }
   // ----------------------------------------------------------------------
   CuboidTopology::
   ~CuboidTopology()
   {}
   // ----------------------------------------------------------------------
   std::string
   CuboidTopology::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   CuboidTopology::
   description( void )
      const throw()
   { return description_; }
   // ----------------------------------------------------------------------
   bool
   CuboidTopology::
   value( const Vec& v )
      const throw()
   { return box_.contains(v); }
   // ----------------------------------------------------------------------
   Box
   CuboidTopology::
   domain( void )
      const throw()
   { return box_; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/cuboid_topology.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: cuboid_topology.cpp,v $
 *-----------------------------------------------------------------------*/
