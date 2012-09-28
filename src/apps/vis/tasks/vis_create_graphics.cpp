/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/tasks/vis_create_graphics.h"
#include "apps/vis/elements/vis_drawable_graphics.h"


using namespace shawn;

namespace vis
{

   CreateGraphicsTask::
   CreateGraphicsTask()
   {}
   // ----------------------------------------------------------------------
   CreateGraphicsTask::
   ~CreateGraphicsTask()
   {}
   // ----------------------------------------------------------------------
   std::string
   CreateGraphicsTask::
   name( void )
      const throw()
   { return "vis_graphics"; }
   // ----------------------------------------------------------------------
   std::string
   CreateGraphicsTask::
   description( void )
      const throw()
   { return "XXX"; }
   // ----------------------------------------------------------------------
   void
   CreateGraphicsTask::
   run( shawn::SimulationController& sc )
      throw( std::runtime_error )
   {
      VisualizationTask::run(sc);

      DrawableGraphics* dg =
         new DrawableGraphics( sc.environment().required_string_param("name"),
                               sc.environment().required_string_param("file") );
      dg->init();
      visualization_w().add_element( dg );
   }


}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/tasks/vis_create_graphics.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_create_graphics.cpp,v $
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
