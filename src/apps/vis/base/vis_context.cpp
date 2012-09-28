/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_context.h"

namespace vis
{

   Context::
   Context( const shawn::Vec& trans,
            double scale,
            double w, double h,
            int dr )
      : trans_  ( trans.x(), trans.y()+(h/scale) ),
        scale_  ( scale ),
        width_  ( w ),
        height_ ( h ),
        draft_  ( dr )
   {}
   // ----------------------------------------------------------------------
   Context::
   ~Context()
   {}
   // ----------------------------------------------------------------------
   shawn::Vec
   Context::
   abs_screen_to_network( const shawn::Vec& v )
      const throw()
   {
      return (v/scale())-translation();
   }




}

#endif
