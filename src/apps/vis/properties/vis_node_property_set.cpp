/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_node_property_set.h"
#include "apps/vis/properties/vis_property_names.h"
#include "apps/vis/properties/vec/vis_property_real_node_position.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/properties/int/vis_property_constant_int.h"
#include "apps/vis/elements/vis_drawable_node.h"

namespace vis
{

   NodePropertySet::
   NodePropertySet()
   {}
   // ----------------------------------------------------------------------
   NodePropertySet::
   ~NodePropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   NodePropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      DrawablePropertySet::init(e);

      IMPL_PROPERTY( position, shawn::Vec );
      IMPL_PROPERTY( foreground, shawn::Vec );
      IMPL_PROPERTY( background, shawn::Vec );
      IMPL_PROPERTY( size, double );
      IMPL_PROPERTY( line_width, double );
      IMPL_PROPERTY( blend, double );
	   IMPL_PROPERTY( shape, int );

      const DrawableNode* v = dynamic_cast<const DrawableNode*>(&e);
      stack_position_->
         add_t( auto_init_property( new PropertyRealNodePositionTask::PropertyRealNodePosition
                                    ( v==NULL ? NULL : &v->node() )));

      stack_foreground_->add_t( auto_init_property( 
         new PropertyConstantVecTask::PropertyConstantVec(shawn::Vec(0,0,0))));
      stack_background_->add_t( auto_init_property( 
         new PropertyConstantVecTask::PropertyConstantVec(shawn::Vec(.7,.7,0))));
      stack_size_->add_t( auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(.15 )));
      stack_line_width_->add_t( auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(.02 )));
      stack_blend_->add_t( auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
      stack_priority_->add_t( auto_init_property( 
         new PropertyConstantDoubleTask::PropertyConstantDouble(1)));
      stack_shape_->add_t( auto_init_property( 
         new PropertyConstantIntTask::PropertyConstantInt(1)));
   }
}

#endif
