/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_UNIFORM_AUTO_POINT_GEN_H
#define __SHAWN_APPS_TOPOLOGY_UNIFORM_AUTO_POINT_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/point_gen.h"

namespace topology
{

   class UniformAutoPointGenerator
      : public PointGenerator
   {
   public:
      UniformAutoPointGenerator();
      virtual ~UniformAutoPointGenerator();

      virtual void init( const PointGeneratorHandle& p2,
                         const PointGeneratorHandle& p25 ,
                         const PointGeneratorHandle& p3 );
      
      virtual void pre_generate( shawn::SimulationController&,
                                 reading::ConstBoolReadingHandle ) throw( std::runtime_error );
      virtual bool generate_point( shawn::Vec& ) throw();
      virtual void post_generate( void ) throw( std::runtime_error );
      virtual bool is_infinite_process( void ) const throw();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   private:
      PointGeneratorHandle pg2_;
      PointGeneratorHandle pg25_;
      PointGeneratorHandle pg3_;
      PointGeneratorHandle active_;
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_auto_point_gen.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_auto_point_gen.h,v $
 *-----------------------------------------------------------------------*/
