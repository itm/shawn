/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_comradius_property_set.h"
#include "apps/vis/properties/vis_property_names.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/elements/vis_drawable_comradius.h"

namespace vis
{

   ComradiusPropertySet::
   ComradiusPropertySet()
   {}
   // ----------------------------------------------------------------------
   ComradiusPropertySet::
   ~ComradiusPropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   ComradiusPropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      DrawablePropertySet::init(e);

      IMPL_PROPERTY( position, shawn::Vec );
      IMPL_PROPERTY( blend, double );

      stack_blend_->add_t( auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
     // stack_position_->add_t( auto_init_property( new PropertyConstantVecTask::PropertyConstantVec(shawn::Vec(0,0,0))));
      stack_priority_->add_t(auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(0.1)));
   }


}

#endif
