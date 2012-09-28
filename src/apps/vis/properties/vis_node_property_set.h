/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_NODE_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_NODE_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include "apps/vis/properties/vis_drawable_property_set.h"

namespace vis
{


   class NodePropertySet
      : public DrawablePropertySet
   {
   public:

      NodePropertySet();
      virtual ~NodePropertySet();

      virtual void init( const Element& ) throw( std::runtime_error );

      DECL_PROPERTY( position, shawn::Vec );
      DECL_PROPERTY( foreground, shawn::Vec );
      DECL_PROPERTY( background, shawn::Vec );
      DECL_PROPERTY( size, double );
      DECL_PROPERTY( line_width, double );
      DECL_PROPERTY( blend, double );
	   DECL_PROPERTY( shape, int );
   };


}

#endif
#endif
