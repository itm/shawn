/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_CAMERA_H
#define __SHAWN_TUBSAPPS_VIS_CAMERA_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/util/refcnt_pointer.h"
#include "sys/util/refcnt_pointable.h"
#include "sys/util/defutils.h"

#include "apps/vis/properties/vis_camera_property_set.h"
#include "apps/vis/elements/vis_element.h"

namespace vis
{

	/** \brief Camera configuration class.
	 * This class represents the virtual camera. It's values affect aspect ratio,
    * resolution, position and scale. It can be configured by the
    * SimpleCameraTask or by setting the properties individually in the
    * configuration file. See Shawn Wiki for tutorials.
    *
	 * @sa vis:CameraPropertySet
	 */
   class Camera
      : public Element
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      Camera();
      virtual ~Camera();
      ///@}

      /**
       * Base-initialization of the camera.
       */
      virtual void init( void ) throw();

      ///@name Getters
      ///@{
      /**
       * Returns the property set (constant).
       */
      virtual const PropertySet& properties( void ) const throw();
      /**
       * Returns the property set.
       */
      virtual PropertySet& properties_w( void ) throw();
      
      /**
       * Returns the property set CameraPropertySet-typed.
       */
      inline CameraPropertySet& cam_properties_w( void ) throw()
      { assert( props_.is_not_null() ); return *props_; }
      /**
       * Returns the property set CameraPropertySet-typed (constant).
       */
      inline const CameraPropertySet& cam_properties( void ) const throw()
      { assert( props_.is_not_null() ); return *props_; }

      /**
       * Returns the position of the camera.
       */
      inline shawn::Vec position( double t ) const throw( std::runtime_error )
      { return cam_properties().position(t); }
      /**
       * Returns the position shift, which is added to the base position vector.
       */
      inline shawn::Vec position_shift( double t ) const throw( std::runtime_error )
      { return cam_properties().position_shift(t); }
      /**
       * Returns the background color.
       */
      inline shawn::Vec background( double t ) const throw( std::runtime_error )
      { return cam_properties().background(t); }
      /**
       * Returns the width of the resolution.
       */
      inline double width( double t ) const throw( std::runtime_error )
      { return cam_properties().width(t); }
      /**
       *  Returns the height of the resolution.
       */
      inline double height( double t ) const throw( std::runtime_error )
      { return cam_properties().height(t); }
      /**
       * Returns the scalation factor, with which the world is scaled.
       */
      inline double scale( double t ) const throw( std::runtime_error )
      { return cam_properties().scale(t); }
      ///@}

      /// Name of the camera.
      static const std::string NAME;

   private:
      /// RefPointer to the property set.
      shawn::RefcntPointer<CameraPropertySet> props_;
   };


}

#endif
#endif
