/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_TRANSITION_H
#define __SHAWN_TUBSAPPS_VIS_TRANSITION_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include <string>
#include <stdexcept>

namespace vis
{
   // a function f:[0,1]->[0,1] with f(0)=0, f(1)=1
   // and an interesting transition in between
   typedef double(*Transition)(double);

   Transition transition( const std::string& ) throw( std::runtime_error );

   template<typename T>
   inline T transition_between( Transition trans,
                                const T& v1, const T& v2,
                                double t1, double t2,
                                double t )
   { 
      double lambda = trans((t-t1)/(t2-t1));
      return (v1*(1.0-lambda)) + (v2*lambda);
   }


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_transition.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_transition.h,v $
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

