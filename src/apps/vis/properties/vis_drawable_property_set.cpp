/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_drawable_property_set.h"
#include "apps/vis/properties/vis_property_names.h"
#include "apps/vis/properties/double/vis_property_constant_double.h"

namespace vis
{

   DrawablePropertySet::
   DrawablePropertySet()
   {}
   // ----------------------------------------------------------------------
   DrawablePropertySet::
   ~DrawablePropertySet()
   {}
   // ----------------------------------------------------------------------
   void
   DrawablePropertySet::
   init( const Element& e )
      throw( std::runtime_error )
   {
      PropertySet::init(e);

      IMPL_PROPERTY( priority, double );
   }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_drawable_property_set.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/02/04 20:19:46 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_property_set.cpp,v $
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
