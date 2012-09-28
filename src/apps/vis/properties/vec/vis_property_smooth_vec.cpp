/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/double/vis_property_smooth_double.h"
#include "apps/vis/properties/vec/vis_property_smooth_vec.h"

using namespace shawn;

namespace vis
{

   PropertySmoothVecTask::PropertySmoothVec::
   PropertySmoothVec( const shawn::Vec& v1, const shawn::Vec& v2, double tmid, Transition tr )
      : v1_(v1), v2_(v2), tmid_(tmid), trans_(tr)
   {}
   // ----------------------------------------------------------------------
   PropertySmoothVecTask::PropertySmoothVec::
   ~PropertySmoothVec()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   PropertySmoothVecTask::PropertySmoothVec::
   value( double t,
          const PropertyStack<shawn::Vec>&,
          const Element& ) 
      const throw()
   { 
      if( t>tmid_ )
         return v2_;
      else {
         return transition_between( trans_,v1_,v2_, start_time(),tmid_, t );
      }
   }
   // ----------------------------------------------------------------------
   PropertySmoothVecTask::
   PropertySmoothVecTask()
   {}
   // ----------------------------------------------------------------------
   PropertySmoothVecTask::
   ~PropertySmoothVecTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertySmoothVecTask::
   name( void )
      const throw()
   {
      return "vis_smooth_vec";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertySmoothVecTask::
   description( void )
      const throw()
   {
      return "create a smooth value property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertySmoothVecTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      shawn::Vec val1( sc.environment().required_double_param("start_x"),
                       sc.environment().required_double_param("start_y"),
                       sc.environment().optional_double_param("start_z",0.0) );
      shawn::Vec val2( sc.environment().required_double_param("end_x"),
                       sc.environment().required_double_param("end_y"),
                       sc.environment().optional_double_param("end_z", 0.0) );
      double tim1 = param_start(sc);
      double tim3 = param_end(sc);
      double tim2 = sc.environment().optional_double_param("reach_time",tim3);
      Transition tr = transition(sc.environment().required_string_param("transition"));
      PropertySmoothVec* pc = new PropertySmoothVec(val1,val2,tim2,tr);

      pc->set_start( tim1 );
      pc->set_end( tim3 );
      pc->set_priority( param_prio(sc) );

      return pc;
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vec/vis_property_smooth_vec.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_smooth_vec.cpp,v $
 * Revision 1.2  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
