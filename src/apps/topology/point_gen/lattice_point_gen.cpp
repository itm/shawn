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
#include "apps/topology/point_gen/lattice_point_gen.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/box.h"
#include "sys/simulation/simulation_controller.h"

using namespace shawn;
using namespace std;
using namespace reading;



namespace topology
{

   const int LatticePointGenerator::WARN_FAIL_COUNT = 1000;
   // ----------------------------------------------------------------------
   LatticePointGenerator::
	   LatticePointGenerator() : spacing_(1.0), cur_x_(0), cur_y_(0), cur_z_(0)
   {}
   // ----------------------------------------------------------------------
   LatticePointGenerator::
   ~LatticePointGenerator()
   {}
   // ----------------------------------------------------------------------
   void
   LatticePointGenerator::
   pre_generate( shawn::SimulationController& sc,
                 reading::ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      PointGenerator::pre_generate(sc,brh);
	  spacing_ = (float) sc.environment().optional_double_param("spacing", 1.0);

      bbox_=extract_box(brh);
      check_fail_count_=true;
	  cur_x_ = cur_y_ = cur_z_ = 0;
//	  mycount = 0;
   }
   // ----------------------------------------------------------------------
   bool
   LatticePointGenerator::
   generate_point( shawn::Vec& v )
      throw()
   {
//	   ++mycount;
//	   cout << "Calling generate point" << mycount << endl;
	  if( check_fail_count_ ) fail_count_=0;
      while(1) {
		  if (box().lower().z()+cur_z_*spacing_>box().upper().z()) {
//				cout << "returned false" << endl;		
			  return false;
		  }
         v=random_point();
		 if( make_feasible(v) ) {
			return true;
		 }
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
   LatticePointGenerator::
   is_infinite_process( void )
      const throw()
   { return true; }
   // ----------------------------------------------------------------------
   Box
   LatticePointGenerator::
   extract_box( ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      return brh->domain();
   }
   // ----------------------------------------------------------------------
   shawn::Vec
   LatticePointGenerator::
   random_point( void )
      throw()
   {	
	   Vec v( box().lower().x() + cur_x_*spacing_,
                  box().lower().y() + cur_y_*spacing_,
                  box().lower().z() + cur_z_*spacing_ );
	   cur_x_++;
	   if (box().lower().x()+cur_x_*spacing_>box().upper().x()) {
			cur_x_=0;
			cur_y_++;
	   }
	   if (box().lower().y()+cur_y_*spacing_>box().upper().y()) {
			cur_x_=0;
			cur_y_=0;
			cur_z_++;
	   }
	   return v;
   }
   // ----------------------------------------------------------------------
   bool
   LatticePointGenerator::
   make_feasible( shawn::Vec& v )
      throw()
   {
      return topology().value(v);
   }
   std::string
   LatticePointGenerator::
   name( void )
      const throw()
   { return "lattice"; }
   // ----------------------------------------------------------------------
   std::string
   LatticePointGenerator::
   description( void )
      const throw()
   { return "lattice distribution for a 2D topology"; }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/lattice_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: lattice_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
