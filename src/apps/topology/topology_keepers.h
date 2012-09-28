/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_APPS_TOPOLOGY_KEEPERS_H
#define __SHAWN_APPS_TOPOLOGY_KEEPERS_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY
#include "apps/reading/readings/reading_keeper.h"

namespace shawn {
   class SimulationController;
}

namespace reading {
   //class ReadingKeeper;
}


namespace topology
{
   class PointGeneratorKeeper;
   class NodeGeneratorKeeper;

   /** \return the keeper that contains all elevations. Because
    *  elevations are simply DoubleReading instances, this happens to be
    *  the ReadingKeeper. */
   const reading::ReadingKeeper& elevation_keeper( const shawn::SimulationController& ) throw();
   /** \return the keeper that contains all elevations. Because
    *  elevations are simply DoubleReading instances, this happens to be
    *  the ReadingKeeper. */
   reading::ReadingKeeper& elevation_keeper_w( shawn::SimulationController& ) throw();
   
   /** \return the keeper that contains all topologies. Because
    *  topologies are simply BoolReading instances, this happens to be
    *  the ReadingKeeper. */
   const reading::ReadingKeeper& topology_keeper( const shawn::SimulationController& ) throw();
   /** \return the keeper that contains all topologies. Because
    *  topologies are simply BoolReading instances, this happens to be
    *  the ReadingKeeper. */
   reading::ReadingKeeper& topology_keeper_w( shawn::SimulationController& ) throw();

   /** \return the keeper that contains all topology::PointGenerator instances */
   const PointGeneratorKeeper& point_gen_keeper( shawn::SimulationController& ) throw();

   /** \return the keeper that contains all topology::PointGenerator instances */
   PointGeneratorKeeper& point_gen_keeper_w( shawn::SimulationController& ) throw();

   /** \return the keeper that contains all topology::NodeGenerator instances */
   const NodeGeneratorKeeper& node_gen_keeper( shawn::SimulationController& ) throw();
   /** \return the keeper that contains all topology::NodeGenerator instances */
   NodeGeneratorKeeper& node_gen_keeper_w( shawn::SimulationController& ) throw();
   
}
#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/topology_keepers.h,v $
 * Version $Revision: 352 $
 * Date    $Date: 2009-05-28 12:13:50 +0200 (Thu, 28 May 2009) $
 *-----------------------------------------------------------------------
 * $Log: topology_keepers.h,v $
 *-----------------------------------------------------------------------*/
