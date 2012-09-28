/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __SHAWN_TUBSAPPS_VIS_PDF_WRITER_H
#define __SHAWN_TUBSAPPS_VIS_PDF_WRITER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_needs_cairo.h"
#include "apps/vis/writer/vis_writer.h"

namespace vis
{
	/** \brief PDF Writer class.
    * This is the PDF output class, which is used to save the visualization output to a PDF
	 * file.
	 * 
	 * @sa vis::Writer
	 * @sa vis::PNGWriter
	 */
   class PDFWriter
      : public Writer
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      PDFWriter();
      virtual ~PDFWriter();
      ///@}
   protected:
	   /**
       * Creates a new cairo surface. If there is already a surface, it is 
       * dropped automatically.
       */
      virtual void make_surface( int, int ) throw();
	   /**
       * Writes the current frame to the output file.
       */
      virtual void write_frame_to_file( cairo_t* cr )
         throw( std::runtime_error );

   };

}
#endif
#endif
