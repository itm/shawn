/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS
#include "apps/vis/writer/vis_writer_factory.h"
#include "apps/vis/writer/vis_writer_keeper.h"
#include "apps/vis/writer/vis_png_writer.h"
#include "apps/vis/writer/vis_pdf_writer.h"

namespace vis
{
   WriterFactory::
   WriterFactory( void )
   {}

   WriterFactory::~WriterFactory()
   {}

   // ----------------------------------------------------------------------
   std::string
   WriterFactory::
   name( void )
      const throw()
   { 
	   return "vis_writer"; 
   }
   
   // ----------------------------------------------------------------------
   std::string 
   WriterFactory::
   description( void )
      const throw()
   {
      return "Vis filewriter objects";
   }

}

#endif
