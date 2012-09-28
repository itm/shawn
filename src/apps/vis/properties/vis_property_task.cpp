/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/properties/vis_property_task.h"
#include "apps/vis/elements/vis_auto_elements.h"

using namespace shawn;

namespace vis
{

   PropertyTask::
   PropertyTask()
   {}
   // ----------------------------------------------------------------------
   PropertyTask::
   ~PropertyTask()
   {}
   // ----------------------------------------------------------------------
   void
   PropertyTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);
      AutoElements elems(sc,visualization_w());
      ConstPropertyHandle cph = NULL;
      bool keep = keep_property(sc);
      std::string prop = sc.environment().required_string_param( "prop" );
      bool something = false;

      while( elems.top().is_not_null() )
         {
            if( (!keep) || cph.is_null() )
               cph = create_property(sc);
            assert( cph.is_not_null() );
            elems.top()->add_property(prop, cph );
            elems.pop();
            something=true;
         }

      if( !something ) {
         WARN( logger(),
               "attached property to no elements");
      }
   }
   // ----------------------------------------------------------------------
   void
   PropertyTask::
   fill_default_params( PropertyBase& p, shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      p.set_start( param_start(sc) );
      p.set_end( param_end(sc) );
      p.set_priority( param_prio(sc) );
   }
   // ----------------------------------------------------------------------
   double
   PropertyTask::
   param_start( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      return sc.environment().optional_double_param( "start",
                                                     -std::numeric_limits<double>::max() );
   }
   // ----------------------------------------------------------------------
   double
   PropertyTask::
   param_end( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      return sc.environment().optional_double_param( "end",
                                                     std::numeric_limits<double>::max() );
   }
   // ----------------------------------------------------------------------
   double
   PropertyTask::
   param_prio( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      return sc.environment().required_double_param( "prio" );
   }
   // ----------------------------------------------------------------------
   bool
   PropertyTask::
   keep_property( shawn::SimulationController& sc )
      throw( std::runtime_error )
   { return true; }
      
}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_task.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_task.cpp,v $
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
