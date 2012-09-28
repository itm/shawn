/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_POINT_GEN_POINT_GEN_H
#define __SHAWN_APPS_TOPOLOGY_POINT_GEN_POINT_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/reading/readings/simple_reading.h"
#include "sys/vec.h"
#include <stdexcept>

namespace reading
{ class BoolReading; class SimulationController; }

namespace shawn
{ class SimulationController; }

namespace topology
{
   DECLARE_HANDLES( PointGenerator );

   class PointGenerator
      : public shawn::KeeperManaged
   {
   public:
      PointGenerator();
      virtual ~PointGenerator();

      virtual void pre_generate( shawn::SimulationController&,
		  reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual bool generate_point( shawn::Vec& ) throw() = 0;
      virtual void post_generate( void ) throw( std::runtime_error );
      virtual bool is_infinite_process( void ) const throw() = 0;

   protected:

      virtual const reading::BoolReading& topology( void ) const throw();
      virtual shawn::SimulationController& simulation_controller_w(void) throw();
      virtual const shawn::SimulationController& simulation_controller(void) const throw();

   private:
      reading::ConstBoolReadingHandle topology_;
      shawn::SimulationController* simc_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/point_gen.h,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: point_gen.h,v $
 *-----------------------------------------------------------------------*/
