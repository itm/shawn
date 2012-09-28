/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_DRAWABLE_PROPERTY_SET_H
#define __SHAWN_TUBSAPPS_VIS_DRAWABLE_PROPERTY_SET_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_set.h"
#include "apps/vis/properties/vis_property_stack.h"

namespace vis
{

   class DrawablePropertySet
      : public PropertySet
   {
   public:

      DrawablePropertySet();
      virtual ~DrawablePropertySet();

      virtual void init( const Element& ) throw( std::runtime_error );

      DECL_PROPERTY( priority, double );
   };


}

#endif
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_drawable_property_set.h,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/04 09:10:07 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_property_set.h,v $
 * Revision 1.2  2006/02/04 09:10:07  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/

