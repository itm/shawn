/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_GRAPHICS_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_GRAPHICS_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include "apps/vis/properties/vis_drawable_property_set.h"

namespace vis
{

	/**
	 * This class defines the properties of a DrawableGraphics object. See vis::PropertySet
	 * for further informations on creating, extending and using property sets.
	 */
   class GraphicsPropertySet
      : public DrawablePropertySet
   {
   public:
	  ///@name construction/deconstruction
	  ///@{
      GraphicsPropertySet();
      virtual ~GraphicsPropertySet();
	  ///@}
	  /// Initializes the property set
      virtual void init( const Element& ) throw( std::runtime_error );

	  /// Position of the image
      DECL_PROPERTY( position, shawn::Vec );
	  /// Anchor
      DECL_PROPERTY( anchor, shawn::Vec );
	  /// Scales the image size
      DECL_PROPERTY( scale, double );
      DECL_PROPERTY( scale_is_network, double );
      DECL_PROPERTY( position_is_network, double );
	  /// Blends image with background
      DECL_PROPERTY( blend, double );
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_graphics_property_set.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_graphics_property_set.h,v $
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

