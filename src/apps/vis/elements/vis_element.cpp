/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_element.h"
#include <string>

namespace vis
{

   Element::
   Element( const std::string& n )
      : name_  ( n ),
        vis_   ( NULL )
   {}
   // ----------------------------------------------------------------------
   Element::
   ~Element()
   {}
   // ----------------------------------------------------------------------
   void
   Element::
   init( void )
      throw()
   {}
   // ----------------------------------------------------------------------
   void
   Element::
   add_property( const std::string& n, 
                 const ConstPropertyHandle& cph )
      throw( std::runtime_error )
   {
      PropertyStackHandle ps = properties_w().property_stack_w(n);
      if( ps.is_null() )
         throw std::runtime_error(std::string("no such property: ") + n );
      ps->add( cph );
   }
   // ----------------------------------------------------------------------
   void
   Element::
   set_visualization( Visualization& v )
      throw()
   { vis_ = &v; }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_element.cpp,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_element.cpp,v $
 * Revision 1.4  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/04 09:10:07  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/29 21:02:01  ali
 * began vis
 *
 *-----------------------------------------------------------------------*/
