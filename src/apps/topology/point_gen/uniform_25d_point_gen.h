/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_UNIFORM_25D_POINT_GEN_H
#define __SHAWN_APPS_TOPOLOGY_UNIFORM_25D_POINT_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/random_process_point_gen.h"
#include "sys/misc/box.h"
#include "apps/reading/readings/simple_reading.h"

namespace topology
{

   class Uniform25DPointGenerator
      : public RandomProcessPointGenerator
   {
   public:
      Uniform25DPointGenerator();
      virtual ~Uniform25DPointGenerator();

      virtual void pre_generate( shawn::SimulationController&,
                                 reading::ConstBoolReadingHandle ) throw( std::runtime_error );

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual shawn::Box extract_box( reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual bool make_feasible( shawn::Vec& ) throw();

   private:
      reading::ConstDoubleReadingHandle lower_;
      reading::ConstDoubleReadingHandle upper_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_25d_point_gen.h,v $
 * Version $Revision: 348 $
 * Date    $Date: 2009-05-07 10:15:59 +0200 (Thu, 07 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: uniform_25d_point_gen.h,v $
 *-----------------------------------------------------------------------*/
