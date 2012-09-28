/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_RANDOM_PROCESS_POINT_GEN_H
#define __SHAWN_APPS_TOPOLOGY_RANDOM_PROCESS_POINT_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/point_gen.h"
#include "sys/misc/box.h"

namespace topology
{

   class RandomProcessPointGenerator
      : public PointGenerator
   {
   public:
      static const int WARN_FAIL_COUNT;

      RandomProcessPointGenerator();
      virtual ~RandomProcessPointGenerator();

      virtual void pre_generate( shawn::SimulationController&,
                                 reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual bool generate_point( shawn::Vec& ) throw();
      virtual bool is_infinite_process( void ) const throw();

   protected:

      virtual shawn::Box extract_box( reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual shawn::Vec random_point( void ) throw();
      virtual bool make_feasible( shawn::Vec& ) throw();

      inline const shawn::Box& box( void ) const throw()
      { return bbox_; }

   private:
      shawn::Box bbox_;
      int fail_count_;
      bool check_fail_count_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/random_process_point_gen.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: random_process_point_gen.h,v $
 *-----------------------------------------------------------------------*/
