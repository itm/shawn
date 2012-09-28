/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/topology/rectangle_topology.h"
#include <sstream>

using namespace shawn;

namespace topology
{

   RectangleTopology::
   RectangleTopology( const Box& b,
                      const std::string& name )
      : box_  ( b ),
        name_ ( name )
   {
      if( name_.empty() )
         {
            std::ostringstream oss;
            oss << "rectangle_topology_" 
                <<        b.lower().x()
                << "_" << b.upper().x()
                << "_" << b.lower().y()
                << "_" << b.upper().y();
            name_=oss.str();
         }
      
      std::ostringstream oss;
      oss << "rectangular 2D topology on "
          << "["   << b.lower().x()
          << ","   << b.upper().x()
          << "] x [" << b.lower().y()
          << ","   << b.upper().y()
          << "]";
      description_=oss.str();
   }
   // ----------------------------------------------------------------------
   RectangleTopology::
   ~RectangleTopology()
   {}
   // ----------------------------------------------------------------------
   std::string
   RectangleTopology::
   name( void )
      const throw()
   { return name_; }
   // ----------------------------------------------------------------------
   std::string
   RectangleTopology::
   description( void )
      const throw()
   { return description_; }
   // ----------------------------------------------------------------------
   bool
   RectangleTopology::
   value( const Vec& v )
      const throw()
   { return box_.contains(v); }
   // ----------------------------------------------------------------------
   Box
   RectangleTopology::
   domain( void )
      const throw()
   { return box_; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology/rectangle_topology.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: rectangle_topology.cpp,v $
 *-----------------------------------------------------------------------*/
