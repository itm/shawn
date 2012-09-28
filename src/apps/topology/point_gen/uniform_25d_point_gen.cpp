/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/uniform_25d_point_gen.h"
#include "apps/topology/topology/topology_25d.h"
#include "sys/misc/random/basic_random.h"
#include "sys/misc/box.h"

using namespace shawn;
using namespace std;
using namespace reading;

namespace topology
{

   Uniform25DPointGenerator::
   Uniform25DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Uniform25DPointGenerator::
   ~Uniform25DPointGenerator()
   {}
   // ----------------------------------------------------------------------
   Box
   Uniform25DPointGenerator::
   extract_box( ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      Box b=brh->domain();
      return Box( Vec(b.lower().x(),b.lower().y(),0.0),
                  Vec(b.upper().x(),b.upper().y(),0.0) );
   }
   // ----------------------------------------------------------------------
   bool
   Uniform25DPointGenerator::
   make_feasible( shawn::Vec& v )
      throw()
   {
      assert( lower_ != NULL ); assert( upper_ != NULL );
      double l = lower_->value(v);
      double u = upper_->value(v);
      if( u<l ) return false;

      v = Vec(v.x(),v.y(),  l + uniform_random_0i_1i()*(u-l) );
      return true;
   }
   // ----------------------------------------------------------------------
   std::string
   Uniform25DPointGenerator::
   name( void )
      const throw()
   { return "uniform_25d"; }
   // ----------------------------------------------------------------------
   std::string
   Uniform25DPointGenerator::
   description( void )
      const throw()
   { return "pseudo-uniform random distribution for a 25D topology, based on simple sampling"; }
   // ----------------------------------------------------------------------
   void
   Uniform25DPointGenerator::
   pre_generate( shawn::SimulationController& sc,
                 reading::ConstBoolReadingHandle br )
      throw( std::runtime_error )
   {
      RandomProcessPointGenerator::pre_generate(sc,br);
      assert( br.is_not_null() );
      const Topology25D* topo = dynamic_cast<const Topology25D*>(br.get());
      if( topo == NULL )
         throw std::runtime_error(std::string("cannot use uniform_25d on '") +
                                  br->name() +
                                  std::string("': it is no 2.5D topology"));
      lower_ = &topo->elevation();
      upper_ = &topo->upper_elevation();
      assert( lower_ != NULL );
      assert( upper_ != NULL );
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_25d_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_25d_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
