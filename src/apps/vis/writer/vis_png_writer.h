/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PNG_WRITER_H
#define __SHAWN_TUBSAPPS_VIS_PNG_WRITER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_needs_cairo.h"
#include "apps/vis/writer/vis_writer.h"

namespace vis
{
   /** \brief PNG writer class. 
    * This is the PNG output class, which is used to save the visualization output to a PNG
	 * image file.
	 * 
	 * @sa vis::Writer
	 * @sa vis::PDFWriter
	 */
   class PNGWriter
      : public Writer
   {
   public:
      ///@name Constructor/Destructor
      ///@{
      PNGWriter();
      virtual ~PNGWriter();
      ///@}

   protected:
	   /**
       * Creates a buffer which is used to draw in before saving the image to
       * an output file.
       */
      virtual void make_buffer( int, int ) throw();
	   /**
       * Creates a new cairo surface. If there is already a surface, it is 
       * dropped.
       */
      virtual void make_surface( int, int ) throw();
	   /**
       * Returns the number of the next frame (for generating multiple frames).
       */
      virtual std::string next_file_name( void ) throw();
	   /**
	    * Writes the current frame to the output file.
       */
      virtual void write_frame_to_file( cairo_t* cr )
         throw( std::runtime_error );

   private:   
	   /// Image buffer.
      unsigned char*       buffer_;    
	   /// Size of the image buffer.
      int                  buffer_size_; 
	  
   };

}

#endif
#endif
