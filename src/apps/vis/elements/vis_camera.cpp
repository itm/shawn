/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_camera.h"

namespace vis
{
   const std::string Camera::NAME("cam");
   // ----------------------------------------------------------------------
   Camera::
   Camera()
      : Element( NAME ),
        props_ ( NULL )
   {}
   // ----------------------------------------------------------------------
   Camera::
   ~Camera()
   {}
   // ----------------------------------------------------------------------
   void
   Camera::
   init( void )
      throw()
   {
      props_ = new CameraPropertySet;
      props_->init(*this);
      Element::init();
   }
   // ----------------------------------------------------------------------
   const PropertySet&
   Camera::
   properties( void )
      const throw()
   { 
      assert( props_.is_not_null() );
      return *props_;
   }
   // ----------------------------------------------------------------------
   PropertySet& 
   Camera::
   properties_w( void )
      throw()
   {
      assert( props_.is_not_null() );
      return *props_;
   }

   

}

#endif
