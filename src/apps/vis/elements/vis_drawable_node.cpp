/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/elements/vis_drawable_node.h"

namespace vis
{

   DrawableNode::
   DrawableNode( const std::string& prefix,
                 const shawn::Node& v )
      : Drawable( prefix + std::string(".") + v.label() ),
        node_ (&v)
   {}
   // ----------------------------------------------------------------------
   DrawableNode::
   ~DrawableNode()
   {}
   // ----------------------------------------------------------------------
   void
   DrawableNode::
   init( void )
      throw()
   {
      Drawable::init();
   }

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/elements/vis_drawable_node.cpp,v $
 * Version $Revision: 1.1 $
 * Date    $Date: 2006/01/31 12:44:41 $
 *-----------------------------------------------------------------------
 * $Log: vis_drawable_node.cpp,v $
 * Revision 1.1  2006/01/31 12:44:41  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
