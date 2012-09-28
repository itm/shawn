/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_simple_camera_task.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"
#include "apps/vis/properties/vec/vis_property_constant_vec.h"

#include "sys/world.h"

using namespace shawn;

namespace vis
{

   SimpleCameraTask::
   SimpleCameraTask()
   {}
   // ----------------------------------------------------------------------
   SimpleCameraTask::
   ~SimpleCameraTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   SimpleCameraTask::
   name( void )
      const throw()
   { return "vis_simple_camera"; }
   // ----------------------------------------------------------------------
   std::string
   SimpleCameraTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   SimpleCameraTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      CameraPropertySet& cam = visualization_w().camera_w().cam_properties_w();
      double prio = sc.environment().optional_double_param( "prio", 0.0 );
      Property<Vec>* pv;
      Property<double>* pd;

      shawn::Vec ll = visualization().world().lower_left();
      shawn::Vec ur = visualization().world().upper_right();

      double net_width = std::max( ur.x() - ll.x(), 1.0 )*1.05;
      double net_height = std::max( ur.y() - ll.y(), 1.0 )*1.05;

      double img_width = 360;
      double img_height = 288;

      pd=new PropertyConstantDoubleTask::PropertyConstantDouble(img_width);
      pd->set_priority(prio);
      cam.stack_width_w().add_t(pd);
      pd=new PropertyConstantDoubleTask::PropertyConstantDouble(img_height);
      pd->set_priority(prio);
      cam.stack_height_w().add_t(pd);
	   img_width = cam.width(0.0);
	   img_height = cam.height(0.0);

      pv=new PropertyConstantVecTask::PropertyConstantVec((ll+ur)/2.0);

      INFO( logger(),
            "using camera position " << (ll+ur)/2.0 );

      pv->set_priority(prio);
      cam.stack_position_w().add_t(pv);
      pv=new PropertyConstantVecTask::PropertyConstantVec(Vec(0.0,0.0,0.0));
      pv->set_priority(prio);
      cam.stack_position_shift_w().add_t(pv);

      double scale = std::min( img_width / net_width,
                               img_height / net_height );
      pd=new PropertyConstantDoubleTask::PropertyConstantDouble(scale);
      pd->set_priority(prio);
      cam.stack_scale_w().add_t(pd);

      INFO( logger(),
            "using camera scale " << scale );

      pv=new PropertyConstantVecTask::PropertyConstantVec(Vec(1.0,1.0,1.0));
      pv->set_priority(prio);
      cam.stack_background_w().add_t(pv);
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/tasks/vis_simple_camera_task.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/04 09:10:07 $
 *-----------------------------------------------------------------------
 * $Log: vis_simple_camera_task.cpp,v $
 * Revision 1.2  2006/02/04 09:10:07  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
