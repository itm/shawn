/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_camera_property_set.h"
#include "apps/vis/properties/vis_property_names.h"
#include "apps/vis/elements/vis_camera.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"

namespace vis
{

   CameraPropertySet::
   CameraPropertySet()
   {}
   // ----------------------------------------------------------------------
   CameraPropertySet::
   ~CameraPropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   CameraPropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      assert( dynamic_cast<const Camera*>(&e) != NULL );
      PropertySet::init(e);

      IMPL_PROPERTY( position, shawn::Vec );
      IMPL_PROPERTY( position_shift, shawn::Vec );
      IMPL_PROPERTY( background, shawn::Vec );
      IMPL_PROPERTY( width, double );
      IMPL_PROPERTY( height, double );
      IMPL_PROPERTY( scale, double );
      IMPL_PROPERTY( freeze, double );

      stack_freeze_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
   }


}

#endif
