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

using namespace shawn;

namespace vis
{

   PropertySmoothDoubleTask::PropertySmoothDouble::
   PropertySmoothDouble( double v1, double v2, double tmid, Transition tr )
      : v1_(v1), v2_(v2), tmid_(tmid), trans_(tr)
   {}
   // ----------------------------------------------------------------------
   PropertySmoothDoubleTask::PropertySmoothDouble::
   ~PropertySmoothDouble()
   {}
   // ----------------------------------------------------------------------
   double
   PropertySmoothDoubleTask::PropertySmoothDouble::
   value( double t,
          const PropertyStack<double>&,
          const Element& ) 
      const throw()
   { 
      if( t>tmid_ )
         return v2_;
      else
         return transition_between( trans_,v1_,v2_, start_time(),tmid_, t );
   }
   // ----------------------------------------------------------------------
   PropertySmoothDoubleTask::
   PropertySmoothDoubleTask()
   {}
   // ----------------------------------------------------------------------
   PropertySmoothDoubleTask::
   ~PropertySmoothDoubleTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertySmoothDoubleTask::
   name( void )
      const throw()
   {
      return "vis_smooth_double";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertySmoothDoubleTask::
   description( void )
      const throw()
   {
      return "create a smooth value property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertySmoothDoubleTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double val1 = sc.environment().required_double_param("start_value");
      double val2 = sc.environment().required_double_param("end_value");
      double tim1 = param_start(sc);
      double tim3 = param_end(sc);
      double tim2 = sc.environment().optional_double_param("reach_time",tim3);
      Transition tr = transition(sc.environment().required_string_param("transition"));
      PropertySmoothDouble* pc = new PropertySmoothDouble(val1,val2,tim2,tr);

      pc->set_start( tim1 );
      pc->set_end( tim3 );
      pc->set_priority( param_prio(sc) );

      return pc;
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/double/vis_property_smooth_double.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_smooth_double.cpp,v $
 * Revision 1.2  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
