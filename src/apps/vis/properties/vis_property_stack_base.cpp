/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_stack_base.h"
#include "apps/vis/properties/vis_property_stack.h"

#include <sstream>

namespace vis
{

   PropertyStackBase::
   PropertyStackBase()
   {}
   // ----------------------------------------------------------------------
   PropertyStackBase::
   ~PropertyStackBase()
   {}
   // ----------------------------------------------------------------------
   std::string 
   PropertyStackBase::
   error_no_property( double t )
      throw()
   {
      std::ostringstream oss;
      oss << "no property is active at time " << t;
      return oss.str();
   }

   PropertyStack<int> TEST_REMOVE;

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_stack_base.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/29 21:02:01 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_stack_base.cpp,v $
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
