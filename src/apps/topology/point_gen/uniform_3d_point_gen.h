/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_UNIFORM_3D_POINT_GEN_H
#define __SHAWN_APPS_TOPOLOGY_UNIFORM_3D_POINT_GEN_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/random_process_point_gen.h"
#include "sys/misc/box.h"

namespace topology
{

   class Uniform3DPointGenerator
      : public RandomProcessPointGenerator
   {
   public:
      Uniform3DPointGenerator();
      virtual ~Uniform3DPointGenerator();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

   protected:
      virtual shawn::Box extract_box( reading::ConstBoolReadingHandle ) throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/uniform_3d_point_gen.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: uniform_3d_point_gen.h,v $
 *-----------------------------------------------------------------------*/
