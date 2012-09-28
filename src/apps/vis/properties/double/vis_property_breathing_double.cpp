/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/double/vis_property_breathing_double.h"

using namespace shawn;
using namespace std;

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vis
{

   PropertyBreathingDoubleTask::PropertyBreathingDouble::
   PropertyBreathingDouble( double t_start, double t_length,
                            double amplitude, double speed,
                            double v_start, double v_breath, double v_end )
   {
      t0_=t_start; l_=t_length; a_=amplitude; v0_=v_start; D1_=v_breath-v_start;
      D2_=v_breath-v_end; v2_=v_end;

      double kbase = (l_*speed - D1_ - D2_)/(a_*M_PI);
      if( kbase < 1.0 )
         k_ = 1;
      else
		  k_ = int(((kbase-1.0)/2.0)+.5)*2 + 1;
      s_ = (D1_+D2_+a_*double(k_)*M_PI)/l_;
   }
   // ----------------------------------------------------------------------
   PropertyBreathingDoubleTask::PropertyBreathingDouble::
   ~PropertyBreathingDouble()
   {}
   // ----------------------------------------------------------------------
   double
   PropertyBreathingDoubleTask::PropertyBreathingDouble::
   value( double t,
          const PropertyStack<double>&,
          const Element& ) 
      const throw()
   {
      double x=t-t0_;
      if( x<0 )
         return v0_;
      else if( x<=D1_/s_ )
         return v0_ + s_*x;
      else if( x<l_-(D2_/s_) )
         return v0_ + D1_ + a_*sin((s_*x-D1_)/a_);
      else if( x<=l_ )
         return v2_ + s_*(l_-x);
      else
         return v2_;
   }
   // ----------------------------------------------------------------------
   PropertyBreathingDoubleTask::
   PropertyBreathingDoubleTask()
   {}
   // ----------------------------------------------------------------------
   PropertyBreathingDoubleTask::
   ~PropertyBreathingDoubleTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   PropertyBreathingDoubleTask::
   name( void )
      const throw()
   {
      return "vis_breathing_double";
   }
   // ----------------------------------------------------------------------
   std::string
   PropertyBreathingDoubleTask::
   description( void )
      const throw()
   {
      return "create a breathing value property";
   }
   // ----------------------------------------------------------------------
   ConstPropertyHandle
   PropertyBreathingDoubleTask::
   create_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      double t0  = param_start(sc);
      double t1  = param_end(sc);
      double amp = sc.environment().required_double_param("amp");
      double vs  = sc.environment().required_double_param("start_value");
      double vb  = sc.environment().required_double_param("breath_value");
      double spd = sc.environment().required_double_param("speed");
      double ve = sc.environment().optional_double_param("end_value",vs);
      PropertyBreathingDouble* pc = 
         new PropertyBreathingDouble( t0, t1-t0,
                                      amp, spd,
                                      vs, vb, ve );

      pc->set_priority( param_prio(sc) );
      pc->set_start( t0 );
      pc->set_end( t1 );
      return pc;
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/double/vis_property_breathing_double.cpp,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_breathing_double.cpp,v $
 * Revision 1.4  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/28 14:02:42  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/05 20:22:35  ali
 * more vis
 *
 *-----------------------------------------------------------------------*/
