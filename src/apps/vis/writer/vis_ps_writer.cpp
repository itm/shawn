/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_ps_writer.h"
#include "apps/vis/base/visualization.h"
#include "sys/util/defutils.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include "cairo.h"
#include "cairo-ps.h"

namespace vis
{

   PsWriter::
   PsWriter()
   {}
   // ----------------------------------------------------------------------
   PsWriter::
   ~PsWriter()
   {
      if( surface_ != NULL ) cairo_surface_destroy(surface_);
   }
   // ----------------------------------------------------------------------
   void
   PsWriter::
   write_frame_to_file( cairo_t* cr )
      throw( std::runtime_error )
   {
            cairo_show_page(cr);
            cairo_surface_flush(surface_);


   }
   // ----------------------------------------------------------------------
   void
   PsWriter::
   make_surface( int w, int h )
      throw()
   {
      bool do_alloc=false;

      if( (surface_ != NULL) &&
          ( (surface_width_ != w) ||
            (surface_height_ != h) ) )
         drop_surface();

      if( surface_ == NULL )

         {
			std::string tFilename(file_base_);
			tFilename+=".ps";
			surface_ = cairo_ps_surface_create(tFilename.c_str(), w, h);
            surface_width_ = w;
            surface_height_ = h;
         }
   }

}
#endif
