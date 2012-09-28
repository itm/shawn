/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_EDGE_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_EDGE_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "sys/vec.h"
#include "apps/vis/properties/vis_drawable_property_set.h"

namespace vis
{

	/**
	 * Defines the properties of DrawableEdge objects. See vis::PropertySet
	 * for further informations on creating, extending and using property sets.
	 */
   class EdgePropertySet
      : public DrawablePropertySet
   {
   public:

      EdgePropertySet();
      virtual ~EdgePropertySet();

      virtual void init( const Element& ) throw( std::runtime_error );

	  /** Line color of the edge. Use
	   *  "vis_constant_vec x=r y=g z=b elem_regex=edge_regex prop=color" to change the line
	   *  color, where r,g and b are double values between 0 and 1, defining the desired color
	   *  in RGB color space and "edge_regex" is a regex about edge names that should be affected.
	   *
	   *  Use a priority definition additionally (prio=some_integer) to override previously set colors on the
	   *  same edge.
	   */
      DECL_PROPERTY( color, shawn::Vec );
	  /**
	   * Sets the line's thickness. Use for example
	   * "vis_constant_double value=0.05 elem_regex=edge.default.* prop=line_width"
	   * to set this value for all edges.
	   */
      DECL_PROPERTY( line_width, double );
	  /**
	   * Blends the line to it's background. Use for example
	   * "vis_constant_double value=0.5 elem_regex=edge.default.* prop=blend"
	   * to set this value for all edges.
	   */
      DECL_PROPERTY( blend, double );
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_edge_property_set.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/05 20:22:35 $
 *-----------------------------------------------------------------------
 * $Log: vis_edge_property_set.h,v $
 * Revision 1.2  2006/02/05 20:22:35  ali
 * more vis
 *
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

