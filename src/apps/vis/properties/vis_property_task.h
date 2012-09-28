/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PROPERTY_TASK_H
#define __SHAWN_TUBSAPPS_VIS_PROPERTY_TASK_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/base/vis_task.h"
#include "apps/vis/elements/vis_element.h"

namespace vis
{

   class PropertyTask
      : public VisualizationTask
   {
   public:
      PropertyTask();
      virtual ~PropertyTask();

      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );

   protected:

      virtual bool keep_property( shawn::SimulationController& sc ) 
         throw( std::runtime_error );
      virtual ConstPropertyHandle create_property( shawn::SimulationController& sc )
         throw( std::runtime_error ) = 0;

      virtual void fill_default_params( PropertyBase&, shawn::SimulationController& sc ) throw( std::runtime_error );
      virtual double param_start( shawn::SimulationController& sc ) throw( std::runtime_error );
      virtual double param_end( shawn::SimulationController& sc ) throw( std::runtime_error );
      virtual double param_prio( shawn::SimulationController& sc ) throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_task.h,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_task.h,v $
 * Revision 1.3  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/04 09:10:07  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

