/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_edge.h"

namespace vis
{

   DrawableEdge::
   DrawableEdge( const std::string& prefix,
                 const shawn::Node& v1,
                 const shawn::Node& v2 )
      : Drawable( prefix + std::string(".") + v1.label() + std::string("_") + v2.label() ),
        src_(&v1), tgt_(&v2)
   {
      //std::cout << "EDGE " << name() << std::endl;
   }
   // ----------------------------------------------------------------------
   DrawableEdge::
   ~DrawableEdge()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableEdge::
   init( void )
      throw()
   {
      Drawable::init();
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_edge.cpp,v $
 * Version $Revision: 1.2 $
 * Date    $Date: 2006/02/19 21:34:29 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_edge.cpp,v $
 * Revision 1.2  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
