/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/uniform_auto_point_gen.h"
#include "apps/topology/topology/topology_2d.h"
#include "apps/topology/topology/topology_25d.h"
#include "apps/topology/topology/topology_3d.h"
#include "sys/misc/box.h"

using namespace shawn;
using namespace std;

namespace topology
{

   UniformAutoPointGenerator::
   UniformAutoPointGenerator()
   {}
   // ----------------------------------------------------------------------
   UniformAutoPointGenerator::
   ~UniformAutoPointGenerator()
   {}
   // ----------------------------------------------------------------------
   void
   UniformAutoPointGenerator::
   init( const PointGeneratorHandle& p2,
         const PointGeneratorHandle& p25 ,
         const PointGeneratorHandle& p3 )
   {
      assert( p2.is_not_null() ); 
      assert( p25.is_not_null() );
      assert( p3.is_not_null() );
      pg2_  = p2;
      pg25_ = p25;
      pg3_  = p3;
   }
   // ----------------------------------------------------------------------
   void
   UniformAutoPointGenerator::
   pre_generate( shawn::SimulationController& sc,
                 reading::ConstBoolReadingHandle rh ) 
      throw( std::runtime_error )
   {
      assert( pg2_.is_not_null() );
      assert( pg25_.is_not_null() );
      assert( pg3_.is_not_null() );
      
      if( dynamic_cast<const Topology2D*>(rh.get()) != NULL )
         active_=pg2_;
      else if( dynamic_cast<const Topology25D*>(rh.get()) != NULL )
         active_=pg25_;
      else if( dynamic_cast<const Topology3D*>(rh.get()) != NULL )
         active_=pg3_;
      else {
         active_=pg3_;
         WARN( logger(),
               string("Don't know type of topology '") +
               rh->name() + 
               string("' -- falling back to ") +
               active_->name());
      }
      active_->pre_generate(sc,rh);
   }
   // ----------------------------------------------------------------------
   bool
   UniformAutoPointGenerator::
   generate_point( shawn::Vec& v )
      throw()
   {
      assert( active_.is_not_null() );
      return active_->generate_point(v);
   }
   // ----------------------------------------------------------------------
   void
   UniformAutoPointGenerator::
   post_generate( void )
      throw( std::runtime_error )
   {
      assert( active_.is_not_null() );
      active_->post_generate();
   }
   // ----------------------------------------------------------------------
   bool
   UniformAutoPointGenerator::
   is_infinite_process( void )
      const throw()
   {
      if( active_.is_null() )
         return true;
      else
         return active_->is_infinite_process();
   }
   // ----------------------------------------------------------------------
   std::string
   UniformAutoPointGenerator::
   name( void )
      const throw()
   { return "uniform_auto"; }
   // ----------------------------------------------------------------------
   std::string
   UniformAutoPointGenerator::
   description( void )
      const throw()
   { return "runs uniform_2d, uniform_25d, or uniform_3d depending on the topology"; }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_auto_point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_auto_point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
