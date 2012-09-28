/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/properties/vis_property_base.h"
#include <limits>

namespace vis
{

   PropertyBase::
   PropertyBase()
      : start_ ( -std::numeric_limits<double>::max() ),
        end_   (  std::numeric_limits<double>::max() )
   {}
   // ----------------------------------------------------------------------
   PropertyBase::
   ~PropertyBase()
   {}
   // ----------------------------------------------------------------------
   double
   PropertyBase::
   start_time( void )
      const throw()
   { return start_; }
   // ----------------------------------------------------------------------
   double
   PropertyBase::
   end_time( void )
      const throw()
   { return end_; }
   // ----------------------------------------------------------------------
   double
   PropertyBase::
   priority( void )
      const throw()
   { return prio_; }
   // ----------------------------------------------------------------------
   void
   PropertyBase::
   set_start( double s )
      throw()
   { start_ = s; }
   // ----------------------------------------------------------------------
   void
   PropertyBase::
   set_end( double e )
      throw()
   { end_ = e; }
   // ----------------------------------------------------------------------
   void
   PropertyBase::
   set_priority( double p )
      throw()
   { prio_ = p; }


}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/properties/vis_property_base.cpp,v $
 * Version $Revision: 1.3 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_property_base.cpp,v $
 * Revision 1.3  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
