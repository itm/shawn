/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_png_writer.h"
#include "apps/vis/base/visualization.h"
#include "sys/util/defutils.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include "cairo.h"

namespace vis
{

   PNGWriter::
   PNGWriter()
      : buffer_         ( NULL ),
        buffer_size_    ( -1 )        
   {}
   // ----------------------------------------------------------------------
   PNGWriter::
   ~PNGWriter()
   {
      if( surface_ != NULL ) drop_surface();
      if( buffer_ != NULL ) delete[] buffer_;
   }
   // ----------------------------------------------------------------------
   void
   PNGWriter::
   write_frame_to_file( cairo_t* cr )
      throw( std::runtime_error )
   {
			std::string fn=next_file_name();
			cairo_surface_write_to_png( surface_, fn.c_str() );
   }
   // ----------------------------------------------------------------------
   std::string
   PNGWriter::
   next_file_name( void )
      throw()
   {
      if( multi_frame_ )
         {
            std::ostringstream oss;
            oss << file_base_
                << "-"
                << std::setw(5) << std::setfill('0') << next_frame_number()
                << ".png";
            return oss.str();
         }
      else
         return file_base_ + std::string(".png");
   }
   // ----------------------------------------------------------------------
   void
   PNGWriter::
   make_buffer( int w, int h )
      throw()
   {
      int sz = w * h * 4;
      if( buffer_ == NULL )
         {
            buffer_ = new unsigned char[sz];
            buffer_size_ = sz;
         }
      else if( buffer_size_ < sz )
         {
            if( surface_ != NULL )
               drop_surface();

            delete [] buffer_;
            buffer_ = new unsigned char[sz];
            buffer_size_ = sz;
         }
   }
   // ----------------------------------------------------------------------
   void
   PNGWriter::
   make_surface( int w, int h )
      throw()
   {
      make_buffer( w, h );
      bool do_alloc=false;

      if( (surface_ != NULL) &&
          ( (surface_width_ != w) ||
            (surface_height_ != h) ) )
         drop_surface();


      if( surface_ == NULL )

         {
            assert( buffer_!=NULL );
            assert( buffer_size_ >= w*h*4 );
            surface_ =
               cairo_image_surface_create_for_data ( buffer_,
                                                     CAIRO_FORMAT_ARGB32,
                                                     w,h,
                                                      w*4);
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
