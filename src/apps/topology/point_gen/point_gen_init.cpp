/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "apps/topology/point_gen/point_gen_init.h"
#include "apps/topology/point_gen/point_gen_keeper.h"
#include "apps/topology/point_gen/uniform_2d_point_gen.h"
#include "apps/topology/point_gen/uniform_25d_point_gen.h"
#include "apps/topology/point_gen/uniform_3d_point_gen.h"
#include "apps/topology/point_gen/uniform_auto_point_gen.h"
#include "apps/topology/point_gen/lattice_point_gen.h"
#include "sys/simulation/simulation_controller.h"

#include <iostream>

namespace topology
{

  void init_topology_point_gen( shawn::SimulationController& sc )
  {
     std::cout << "init_topology_point_gen" << std::endl;
     PointGeneratorKeeper* pg = new PointGeneratorKeeper;
     sc.add_keeper( pg );

     PointGenerator* p2 = new Uniform2DPointGenerator;
     pg->add( p2 );
     PointGenerator* p25 = new Uniform25DPointGenerator;
     pg->add( p25 );
     PointGenerator* p3 = new Uniform3DPointGenerator;
     pg->add( p3 );

     UniformAutoPointGenerator* pauto = new UniformAutoPointGenerator;
     pauto->init(p2,p25,p3);
     pg->add(pauto);

     PointGenerator* l = new LatticePointGenerator;
     pg->add( l );
  }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/point_gen/point_gen_init.cpp,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: point_gen_init.cpp,v $
 *-----------------------------------------------------------------------*/
