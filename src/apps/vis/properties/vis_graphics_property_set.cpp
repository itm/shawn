/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_graphics_property_set.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/elements/vis_drawable_graphics.h"

namespace vis
{

   GraphicsPropertySet::
   GraphicsPropertySet()
   {}
   // ----------------------------------------------------------------------
   GraphicsPropertySet::
   ~GraphicsPropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   GraphicsPropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      DrawablePropertySet::init(e);

      IMPL_PROPERTY( position, shawn::Vec );
      IMPL_PROPERTY( anchor, shawn::Vec );
      IMPL_PROPERTY( scale, double );
      IMPL_PROPERTY( scale_is_network, double );
      IMPL_PROPERTY( position_is_network, double );
      IMPL_PROPERTY( blend, double );

      stack_scale_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(1.0)));
      stack_blend_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
      stack_scale_is_network_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
      stack_position_is_network_->add_t(auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
      stack_priority_->add_t(auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(100)));
      stack_anchor_->add_t(auto_init_property(new PropertyConstantVecTask::PropertyConstantVec(shawn::Vec(0,0))));
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_graphics_property_set.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_graphics_property_set.cpp,v $
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
