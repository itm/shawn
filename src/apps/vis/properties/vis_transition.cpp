/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vis_transition.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace vis
{

   double transition_sin( double x )
   { return (sin(M_PI*(x - .5))+1)/2.0; }
   double transition_sqr( double x )
   { return x*x; }
   double transition_linear( double x )
   { return x; }



   Transition transition( const std::string& n )
      throw( std::runtime_error )
   {
      if( n=="sin" )
         return transition_sin;
      if( n=="linear" )
         return transition_linear;
      if( n=="sqr" )
         return transition_sqr;

      throw std::runtime_error(std::string("no such transition: ")+n);
   }



}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_transition.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_transition.cpp,v $
 * Revision 1.2  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
