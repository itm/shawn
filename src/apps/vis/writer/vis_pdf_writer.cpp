/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_pdf_writer.h"
#include "apps/vis/base/visualization.h"
#include "sys/util/defutils.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>
#include "cairo.h"
#include "cairo-pdf.h"

namespace vis
{

   PDFWriter::
   PDFWriter()
   {}
   // ----------------------------------------------------------------------
   PDFWriter::
   ~PDFWriter()
   {
      if( surface_ != NULL ) drop_surface();
   }
   // ----------------------------------------------------------------------
   void
   PDFWriter::
   write_frame_to_file( cairo_t* cr )
      throw( std::runtime_error )
   {
            cairo_show_page(cr);
   }
   // ----------------------------------------------------------------------
   void
   PDFWriter::
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
			tFilename+=".pdf";
			surface_ = cairo_pdf_surface_create(tFilename.c_str(), w, h);
            surface_width_ = w;
            surface_height_ = h;
         }
   }

}
#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/tubsapps/vis/writer/vis_png_writer.cpp,v $
 * Version $Revision: 1.8 $
 * Date    $Date: 2006/03/15 14:14:02 $
 *-----------------------------------------------------------------------
 * $Log: vis_png_writer.cpp,v $
 * Revision 1.8  2006/03/15 14:14:02  pfister
 * *** empty log message ***
 *
 * Revision 1.7  2006/02/25 15:21:41  ali
 * *** empty log message ***
 *
 * Revision 1.6  2006/02/22 09:18:57  ali
 * *** empty log message ***
 *
 * Revision 1.5  2006/02/19 21:34:29  ali
 * *** empty log message ***
 *
 * Revision 1.4  2006/02/14 21:53:10  ali
 * *** empty log message ***
 *
 * Revision 1.3  2006/02/04 20:19:46  ali
 * *** empty log message ***
 *
 * Revision 1.2  2006/02/01 17:07:29  ali
 * *** empty log message ***
 *
 * Revision 1.1  2006/01/31 12:44:00  ali
 * *** empty log message ***
 *
 *-----------------------------------------------------------------------*/
