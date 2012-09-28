/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/point_gen.h"
#include "sys/simulation/simulation_controller.h"

namespace topology
{
   
   PointGenerator::
   PointGenerator()
      : topology_ ( NULL ),
        simc_     ( NULL )
   {}
   // ----------------------------------------------------------------------
   PointGenerator::
   ~PointGenerator()
   {}
   // ----------------------------------------------------------------------
   const reading::BoolReading&
   PointGenerator::
   topology( void ) 
      const throw()
   {
      assert( topology_.is_not_null() );
      return *topology_;
   }
   // ----------------------------------------------------------------------
   shawn::SimulationController&
   PointGenerator::
   simulation_controller_w(void)
      throw()
   {
      assert( simc_ != NULL );
      return *simc_;
   }
   // ----------------------------------------------------------------------
   const shawn::SimulationController&
   PointGenerator::
   simulation_controller(void)
      const throw()
   {
      assert( simc_ != NULL );
      return *simc_;
   }
   // ----------------------------------------------------------------------
   void
   PointGenerator::
   pre_generate( shawn::SimulationController& sc,
                 reading::ConstBoolReadingHandle brh )
      throw( std::runtime_error )
   {
      simc_ = &sc;
      topology_=brh;
   }
   // ----------------------------------------------------------------------
   void
   PointGenerator::
   post_generate( void )
      throw( std::runtime_error )
   {}

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/point_gen.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: point_gen.cpp,v $
 *-----------------------------------------------------------------------*/
