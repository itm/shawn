/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_edge_property_set.h"
#include "apps/vis/properties/vis_property_names.h"
#include "apps/vis/properties/vec/vis_property_real_node_position.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/elements/vis_drawable_edge.h"

namespace vis
{

   EdgePropertySet::
   EdgePropertySet()
   {}
   // ----------------------------------------------------------------------
   EdgePropertySet::
   ~EdgePropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   EdgePropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      DrawablePropertySet::init(e);

      IMPL_PROPERTY( color, shawn::Vec );
      IMPL_PROPERTY( line_width, double );
      IMPL_PROPERTY( blend, double );

      const DrawableEdge* ed = dynamic_cast<const DrawableEdge*>(&e);
      if( ed==NULL )
         throw std::runtime_error("EdgePropertySet only works for DrawableEdge instances");

      stack_color_->add_t( auto_init_property( new PropertyConstantVecTask::PropertyConstantVec(shawn::Vec(0,0,0))));
      stack_line_width_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(.02 )));
      stack_blend_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0)));
      stack_priority_->add_t( auto_init_property( new PropertyConstantDoubleTask::PropertyConstantDouble(0.2)));

   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_edge_property_set.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/05 20:22:35 $
 *-----------------------------------------------------------------------
 * $Log: vis_edge_property_set.cpp,v $
 * Revision 1.2  2006/02/05 20:22:35  ali
 * more vis
 *
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
