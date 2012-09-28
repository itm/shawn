/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_CREATE_GRAPHICS_H
#define __SHAWN_TUBSAPPS_VIS_CREATE_GRAPHICS_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_task.h"

namespace vis
{

	/**
	 * This visualization task creates a drawable graphics object. See vis::DrawableGraphics and
	 * vis::GraphicsPropertySet for more details on graphics and their properties.
	 * 
	 * Use a CreateGraphicsTask to create a drawable graphics object by calling
	 * "vis_create_graphics name=object_name file=filename.png".
	 *
	 * @sa vis::DrawableGraphics
	 * @sa vis::GraphicsPropertySet
	 */
   class CreateGraphicsTask
      : public VisualizationTask
   {
   public:
	   ///@name Constructor/Destructor
	   ///@{
      CreateGraphicsTask();
      virtual ~CreateGraphicsTask();
	   ///@}

      ///@name Getter
      ///@{
      /**
       * The name of the task.
       */
      virtual std::string name( void ) const throw();
      /**
       * A short description of the task.
       */
      virtual std::string description( void ) const throw();
      ///@}

      /**
       * Runs the task. This ist the task main method.
       */
      virtual void run( shawn::SimulationController& sc )
         throw( std::runtime_error );
   };

}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/tasks/vis_create_graphics.h,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/02/14 21:53:10 $
 *-----------------------------------------------------------------------
 * $Log: vis_create_graphics.h,v $
 * Revision 1.1  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

