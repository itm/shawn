/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_ELEVATION_TASK_H
#define __SHAWN_APPS_TOPOLOGY_ELEVATION_XYZ_FILE_ELEVATION_TASK_H

#include "_apps_enable_cmake.h"
#ifdef ENABLE_TOPOLOGY

#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task.h"
#include "sys/vec.h"
#include <string>

namespace topology
{
   class XYZFile;

   class XYZFileElevationTask
      : public shawn::SimulationTask
   {
   public:
      static const std::string KEY_SCALE;
      static const std::string KEY_BOX;

      XYZFileElevationTask();
      virtual ~XYZFileElevationTask();

      virtual std::string name( void ) const throw();
      virtual std::string description( void ) const throw();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:
      virtual void xy_place_box( shawn::SimulationController&,
                                 const XYZFile&,
                                 shawn::Vec&, shawn::Vec& )
         throw( std::runtime_error );
      virtual void xy_place_scale( shawn::SimulationController&,
                                 const XYZFile&,
                                 shawn::Vec&, shawn::Vec& )
         throw( std::runtime_error );
      virtual void z_place_box( shawn::SimulationController&,
                                const XYZFile&,
                                shawn::Vec&, shawn::Vec& )
         throw( std::runtime_error );
      virtual void z_place_scale( shawn::SimulationController&,
                                const XYZFile&,
                                shawn::Vec&, shawn::Vec& )
         throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/topology/elevation/xyz_file_elevation_task.h,v $
 * Version $Revision: 197 $
 * Date    $Date: 2008-04-29 17:40:51 +0200 (Tue, 29 Apr 2008) $
 *-----------------------------------------------------------------------
 * $Log: xyz_file_elevation_task.h,v $
 *-----------------------------------------------------------------------*/
