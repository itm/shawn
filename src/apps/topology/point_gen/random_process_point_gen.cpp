/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/random_process_point_gen.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/box.h"

using namespace shawn;
using namespace std;
using namespace reading;


namespace topology
{

   const int RandomProcessPointGenerator::WARN_FAIL_COUNT = 1000;
   // ----------------------------------------------------------------------
   RandomProcessPointGenerator::
   RandomProcessPointGenerator()
   {}
   // ----------------------------------------------------------------------
   RandomProcessPointGenerator::
   ~RandomProcessPointGenerator()
   {}
   // ----------------------------------------------------------------------
   void
   RandomProcessPointGenerator::
   pre_generate( shawn::SimulationController& sc,
                 reading::ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      PointGenerator::pre_generate(sc,brh);
      bbox_=extract_box(brh);
      check_fail_count_=true;
   }
   // ----------------------------------------------------------------------
   bool
   RandomProcessPointGenerator::
   generate_point( shawn::Vec& v )
      throw()
   {
      if( check_fail_count_ ) fail_count_=0;
      while(1) {
         v=random_point();
         if( make_feasible(v) )
            return true;
         if( check_fail_count_ && (fail_count_<WARN_FAIL_COUNT) )
            if( ++fail_count_ == WARN_FAIL_COUNT ) {
               WARN( logger(),
                     "generated " << WARN_FAIL_COUNT 
                     << " points without a single hit: misconfiguration?" );
            }
      }
      return false;//cannot be reached
   }
   // ----------------------------------------------------------------------
   bool
   RandomProcessPointGenerator::
   is_infinite_process( void )
      const throw()
   { return true; }
   // ----------------------------------------------------------------------
   Box
   RandomProcessPointGenerator::
   extract_box( ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      return brh->domain();
   }
   // ----------------------------------------------------------------------
   shawn::Vec
   RandomProcessPointGenerator::
   random_point( void )
      throw()
   {
      return Vec( box().lower().x() + ( uniform_random_0i_1i() *(box().upper().x()-box().lower().x()) ),
                  box().lower().y() + ( uniform_random_0i_1i() *(box().upper().y()-box().lower().y()) ),
                  box().lower().z() + ( uniform_random_0i_1i() *(box().upper().z()-box().lower().z()) ) );
   }
   // ----------------------------------------------------------------------
   bool
   RandomProcessPointGenerator::
   make_feasible( shawn::Vec& v )
      throw()
   {
      return topology().value(v);
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/random_process_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: random_process_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
