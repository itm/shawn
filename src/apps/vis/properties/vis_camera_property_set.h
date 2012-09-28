/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_CAMERA_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_CAMERA_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include "apps/vis/properties/vis_property_set.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace vis
{
	/**
	 * This property set class defines the properties of a vis::Camera object. See vis::PropertySet
	 * for further informations on creating, extending and using property sets.
	 */
   class CameraPropertySet
      : public PropertySet
   {
   public:

      CameraPropertySet();
      virtual ~CameraPropertySet();

      virtual void init( const Element& ) throw( std::runtime_error );

	  ///@name Property declarations
	  ///@{
	  /**
	   * Sets the position of the camera.
	   */
      DECL_PROPERTY( position, shawn::Vec );
      DECL_PROPERTY( position_shift, shawn::Vec );
	  /**
	   * Defines the background color of the resulting visual output.
	   */
      DECL_PROPERTY( background, shawn::Vec );
	  /// The width of the camera.
      DECL_PROPERTY( width, double );
	  /// The height of the camera
      DECL_PROPERTY( height, double );
      DECL_PROPERTY( scale, double );
      DECL_PROPERTY( freeze, double );
	  ///@}
   };


}

#endif
#endif
