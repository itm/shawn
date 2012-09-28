/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#ifndef __VIS_PS_WRITER_H
#define __VIS_PS_WRITER_H
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/base/vis_needs_cairo.h"
#include "apps/vis/writer/vis_writer.h"

namespace vis
{
	/** \brief PostScript Writer class.
    * This is the PostScript output class, which is used to save the
    * visualization output to a PS file.
	 * 
	 * @sa vis::Writer
	 * @sa vis::PNGWriter
    * @sa vis::PSWriter
	 */
   class PsWriter
      : public Writer
   {
   public:
      ///@name Contructor/Destructor
      ///@{
      PsWriter();
      virtual ~PsWriter();
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
